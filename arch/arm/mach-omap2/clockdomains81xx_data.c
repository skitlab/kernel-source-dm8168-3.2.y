/*
 * arch/arm/mach-omap2/clockdomains81xx.h
 *
 * TI81XX Clock Domain data.
 *
 * Copyright (C) 2010 Texas Instruments, Inc. - http://www.ti.com/
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

#ifndef __ARCH_ARM_MACH_OMAP2_CLOCKDOMAINS81XX_H
#define __ARCH_ARM_MACH_OMAP2_CLOCKDOMAINS81XX_H

#include <linux/kernel.h>
#include <linux/io.h>

#include "clockdomain.h"

#include "cm81xx.h"
#include "cm-regbits-81xx.h"

/*
 * TODO:
 * - Add other domains as required
 * - Fill up associated powerdomans (especially ALWON powerdomains are NULL at
 *   the moment
 * - Consider dependencies across domains (probably not applicable till now)
 */

/*
 * Common TI81XX
 */

static struct clockdomain alwon_l3_slow_81xx_clkdm = {
	.name		= "alwon_l3_slow_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_L3_SLOW_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
#if defined(CONFIG_MACH_TI8148EVM)
	.flags		= CLKDM_CAN_SWSUP,
#else
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
#endif
};

static struct clockdomain alwon_l3_med_81xx_clkdm = {
	.name		= "alwon_l3_med_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_L3_MED_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
#if defined(CONFIG_MACH_TI8148EVM)
	.flags		= CLKDM_CAN_SWSUP,
#else
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
#endif
};

static struct clockdomain alwon_l3_fast_81xx_clkdm = {
	.name		= "alwon_l3_fast_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_L3_FAST_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
#if defined(CONFIG_MACH_TI8148EVM)
	.flags		= CLKDM_CAN_SWSUP,
#else
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
#endif
};

static struct clockdomain alwon_ethernet_81xx_clkdm = {
	.name		= "alwon_ethernet_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ETHERNET_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
#if defined(CONFIG_MACH_TI8148EVM)
	.flags		= CLKDM_CAN_SWSUP,
#else
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
#endif
};

/* OCMC clock domain */
static struct clockdomain mmu_81xx_clkdm = {
	.name		= "mmu_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_MMU_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
#if defined(CONFIG_MACH_TI8148EVM)
	.flags		= CLKDM_CAN_SWSUP,
#else
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
#endif
};

static struct clockdomain mmu_cfg_81xx_clkdm = {
	.name		= "mmu_cfg_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_MMUCFG_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
#if defined(CONFIG_MACH_TI8148EVM)
	.flags		= CLKDM_CAN_SWSUP,
#else
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
#endif
};

/*
 * TI816X only
 */

static struct clockdomain alwon_mpu_816x_clkdm = {
	.name		= "alwon_mpu_clkdm",
	.pwrdm		= { .name = "alwon_pwrdm" },
	.cm_inst	= TI81XX_CM_ALWON_MOD,
	.clkdm_offs	= TI81XX_CM_ALWON_MPU_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
};

static struct clockdomain active_gem_816x_clkdm = {
	.name		= "active_gem_clkdm",
	.pwrdm		= { .name = "active_pwrdm" },
	.cm_inst	= TI816X_CM_ACTIVE_MOD,
	.clkdm_offs	= TI816X_CM_ACTIVE_GEM_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
};

static struct clockdomain ivahd0_816x_clkdm = {
	.name		= "ivahd0_clkdm",
	.pwrdm		= { .name = "ivahd0_pwrdm" },
	.cm_inst	= TI816X_CM_IVAHD0_MOD,
	.clkdm_offs	= TI816X_CM_IVAHD0_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
};

static struct clockdomain ivahd1_816x_clkdm = {
	.name		= "ivahd1_clkdm",
	.pwrdm		= { .name = "ivahd1_pwrdm" },
	.cm_inst	= TI816X_CM_IVAHD1_MOD,
	.clkdm_offs	= TI816X_CM_IVAHD1_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
};

static struct clockdomain ivahd2_816x_clkdm = {
	.name		= "ivahd2_clkdm",
	.pwrdm		= { .name = "ivahd2_pwrdm" },
	.cm_inst	= TI816X_CM_IVAHD2_MOD,
	.clkdm_offs	= TI816X_CM_IVAHD2_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
};

static struct clockdomain sgx_816x_clkdm = {
	.name		= "sgx_clkdm",
	.pwrdm		= { .name = "sgx_pwrdm" },
	.cm_inst	= TI816X_CM_SGX_MOD,
	.clkdm_offs	= TI816X_CM_SGX_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
};

static struct clockdomain default_l3_med_816x_clkdm = {
	.name		= "default_l3_med_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI816X_CM_DEFAULT_MOD,
	.clkdm_offs	= TI816X_CM_DEFAULT_L3_MED_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
};

static struct clockdomain default_ducati_816x_clkdm = {
	.name		= "default_ducati_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI816X_CM_DEFAULT_MOD,
	.clkdm_offs	= TI816X_CM_DEFAULT_DUCATI_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
};

static struct clockdomain default_pcie_816x_clkdm = {
	.name		= "default_pcie_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI816X_CM_DEFAULT_MOD,
	.clkdm_offs	= TI816X_CM_DEFAULT_PCI_CLKDM,
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
};

static struct clockdomain default_usb_816x_clkdm = {
	.name		= "default_usb_clkdm",
	.pwrdm		= { .name = "default_pwrdm" },
	.cm_inst	= TI816X_CM_DEFAULT_MOD,
	.clkdm_offs	= TI816X_CM_DEFAULT_L3_SLOW_CLKDM, /* FIXME */
	.clktrctrl_mask	= TI81XX_CLKTRCTRL_MASK,
	.flags		= CLKDM_CAN_HWSUP_SWSUP,
};

static struct clockdomain *clockdomains_ti81xx[] __initdata = {
	&wkup_common_clkdm,
	&cm_common_clkdm,
	&prm_common_clkdm,

	&alwon_mpu_816x_clkdm,
	&alwon_l3_slow_81xx_clkdm,
	&alwon_l3_med_81xx_clkdm,
	&alwon_l3_fast_81xx_clkdm,
	&alwon_ethernet_81xx_clkdm,
	&mmu_81xx_clkdm,
	&mmu_cfg_81xx_clkdm,
	&active_gem_816x_clkdm,
	&ivahd0_816x_clkdm,
	&ivahd1_816x_clkdm,
	&ivahd2_816x_clkdm,
	&sgx_816x_clkdm,
	&default_l3_med_816x_clkdm,
	&default_ducati_816x_clkdm,
	&default_pcie_816x_clkdm,
	&default_usb_816x_clkdm,
	NULL,
};

void __init ti81xx_clockdomains_init(void)
{
	clkdm_register_platform_funcs(&omap3_clkdm_operations);
	clkdm_register_clkdms(clockdomains_ti81xx);
	clkdm_complete_init();
}
#endif
