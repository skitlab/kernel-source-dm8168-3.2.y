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

#include "pm.h"
#include "powerdomain.h"
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
static int _pwrdms_set_suspend_state(struct powerdomain *pwrdm, void *unused)
{
	/* alwon*_pwrdm fail this check and return */
	if (!pwrdm->pwrsts)
		return 0;

	return pwrdm_set_next_pwrst(pwrdm, PWRDM_POWER_OFF);
}

static int ti81xx_pm_suspend(void)
{
	int ret = 0;
	/* update pwrst->next_state */
	ret = pwrdm_for_each(_pwrdms_set_suspend_state, NULL);
	if (ret) {
		pr_err("Failed to set pwrdm suspend states\n");
		return ret;
	}
	/* do wfi */
	do_wfi();
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

	ret = pwrdm_for_each(pwrdms_setup, NULL);
	if (ret) {
		pr_err("Failed to setup powerdomains\n");
		goto err1;
	}
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
