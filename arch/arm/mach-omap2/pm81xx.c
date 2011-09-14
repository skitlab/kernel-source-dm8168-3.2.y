/*
 * TI81XX Power Management
 *
 * This module implements TI81XX specific Power management Routines
 *
 * Copyright (C) {2011} Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/pm.h>
#include <linux/suspend.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <asm/io.h>

#include "pm.h"
#include "powerdomain.h"
#include "clockdomain.h"
#include "cm81xx.h"
#include "cm-regbits-81xx.h"
#include "prm2xxx_3xxx.h"
#include <mach/omap4-common.h>
#include <plat/serial.h>

struct power_state {
	struct powerdomain *pwrdm;
	u32 next_state;
#ifdef CONFIG_SUSPEND
	u32 saved_state;
#endif
	struct list_head node;
};

static LIST_HEAD(pwrst_list);

#ifdef CONFIG_SUSPEND
static int ti81xx_pwrdms_set_suspend_state(struct powerdomain *pwrdm)
{
	/* alwon*_pwrdm fail this check and return */
	if (!pwrdm->pwrsts)
		return 0;

	pwrdm_set_next_pwrst(pwrdm, PWRDM_POWER_OFF);
	return pwrdm_wait_transition(pwrdm);
}

static int ti81xx_pm_enter_ddr_self_refresh(void)
{
	return 0;
}

static int ti81xx_pm_suspend(void)
{
	struct power_state *pwrst;
	int ret = 0;

	/* TBD: Keep DDR in self refresh mode here */
	ti81xx_pm_enter_ddr_self_refresh();
	/* Read current next_pwrsts */
	list_for_each_entry(pwrst, &pwrst_list, node) {
		pwrst->saved_state = pwrdm_read_next_pwrst(pwrst->pwrdm);
	}

	/* update pwrst->next_state */
	list_for_each_entry(pwrst, &pwrst_list, node) {
		if (ti81xx_pwrdms_set_suspend_state(pwrst->pwrdm)) {
			pr_err("Failed to set pwrdm suspend states\n");
			return ret;
		}
	}
	/* Execute WFI on ARM */
	do_wfi();

	/* resume */
	list_for_each_entry(pwrst, &pwrst_list, node)
		pwrdm_set_next_pwrst(pwrst->pwrdm, pwrst->saved_state);
	return ret;
}

static int ti81xx_pm_enter(suspend_state_t suspend_state)
{
	int ret = 0;

	switch (suspend_state) {
	case PM_SUSPEND_STANDBY:
	case PM_SUSPEND_MEM:
		ret = ti81xx_pm_suspend();
		break;
	default:
		ret = -EINVAL;
	}
	return ret;
}

static int ti81xx_pm_begin(suspend_state_t state)
{
	disable_hlt();
	omap_uart_enable_irqs(0);
	return 0;
}

static void ti81xx_pm_end(void)
{
	enable_hlt();
	omap_uart_enable_irqs(1);
	return;
}

static const struct platform_suspend_ops ti81xx_pm_ops[] = {
	{
		.begin		= ti81xx_pm_begin,
		.end		= ti81xx_pm_end,
		.enter		= ti81xx_pm_enter,
		.valid		= suspend_valid_only_mem,
	}
};
#endif /* CONFIG_SUSPEND */

static int __init pwrdms_setup(struct powerdomain *pwrdm, void *unused)
{
	struct power_state *pwrst;

	if (!pwrdm->pwrsts)
		return 0;

	pwrst = kmalloc(sizeof(struct power_state), GFP_ATOMIC);
	if (!pwrst)
		return -ENOMEM;
	pwrst->pwrdm = pwrdm;
	pwrst->next_state = PWRDM_POWER_OFF;
	list_add(&pwrst->node, &pwrst_list);

	return pwrdm_set_next_pwrst(pwrst->pwrdm, pwrst->next_state);
}

/*
 * Enable hw supervised mode for all clockdomains if it's
 * supported. Initiate sleep transition for other clockdomains, if
 * they are not used
 */
static int __init clkdms_setup(struct clockdomain *clkdm, void *unused)
{
	if (clkdm->flags & CLKDM_CAN_ENABLE_AUTO)
		omap2_clkdm_allow_idle(clkdm);
	else if (clkdm->flags & CLKDM_CAN_FORCE_SLEEP &&
		atomic_read(&clkdm->usecount) == 0)
		omap2_clkdm_sleep(clkdm);
	return 0;
}

static void __init prcm_setup_regs(void)
{
	/* Clear reset status register */
	omap2_prm_write_mod_reg(0xffffffff, TI81XX_PRM_DEVICE_MOD,
							TI81XX_RM_RSTST);
	omap2_prm_write_mod_reg(0xffffffff, TI81XX_PRM_ALWON_MOD,
							TI81XX_RM_RSTST);
	omap2_prm_write_mod_reg(0xffffffff, TI814X_PRM_DSP_MOD,
							TI81XX_RM_RSTST);
	omap2_prm_write_mod_reg(0xffffffff, TI814X_PRM_ALWON2_MOD,
							TI81XX_RM_RSTST);
	omap2_prm_write_mod_reg(0xffffffff, TI814X_PRM_HDVICP_MOD,
							TI81XX_RM_RSTST);
	omap2_prm_write_mod_reg(0xffffffff, TI814X_PRM_ISP_MOD,
							TI81XX_RM_RSTST);
	omap2_prm_write_mod_reg(0xffffffff, TI814X_PRM_HDVPSS_MOD,
							TI81XX_RM_RSTST);
	omap2_prm_write_mod_reg(0xffffffff, TI814X_PRM_GFX_MOD,
							TI81XX_RM_RSTST);
}

/**
 * ti81xx_pm_init - Init routine for TI81XX PM
 *
 * Initializes all powerdomain and clockdomain target states
 * and all PRCM settings.
 */
static int __init ti81xx_pm_init(void)
{
	struct power_state *pwrst, *tmp;
	int ret;

	pr_info("Power Management for TI81XX.\n");

	if (!cpu_is_ti814x())
		return -ENODEV;

	prcm_setup_regs();
	ret = pwrdm_for_each(pwrdms_setup, NULL);
	if (ret) {
		pr_err("Failed to setup powerdomains\n");
		goto err1;
	}
	clkdm_for_each(clkdms_setup, NULL);
#ifdef CONFIG_SUSPEND
	suspend_set_ops(ti81xx_pm_ops);
#endif
	return ret;
err1:
	list_for_each_entry_safe(pwrst, tmp, &pwrst_list, node) {
		list_del(&pwrst->node);
		kfree(pwrst);
	}
	return ret;
}
late_initcall(ti81xx_pm_init);
