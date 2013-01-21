/*
 * TI816X Power Domain data.
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

#ifndef __ARCH_ARM_MACH_OMAP2_POWERDOMAINS81XX_H
#define __ARCH_ARM_MACH_OMAP2_POWERDOMAINS81XX_H

#include <linux/kernel.h>
#include <linux/init.h>

#include "powerdomain.h"

#include "prcm-common.h"
#include "prcm44xx.h"
#include "prm-regbits-44xx.h"
#include "prm44xx.h"
#include "prcm_mpu44xx.h"
#include "prm2xxx_3xxx.h"


#ifdef CONFIG_SOC_OMAPTI81XX

/*
 * TI81XX common
 */

static struct powerdomain alwon_81xx_pwrdm = {
	.name		  = "alwon_pwrdm",
	.prcm_offs	  = TI81XX_PRM_ALWON_MOD,
	.voltdm           = { .name = "core" },
};

/*
 * TI816X only
 */

static struct powerdomain active_816x_pwrdm = {
	.name		  = "active_pwrdm",
	.prcm_offs	  = TI816X_PRM_ACTIVE_MOD,
	.pwrsts		  = PWRSTS_OFF_ON,
	.voltdm           = { .name = "core" },
};

static struct powerdomain default_816x_pwrdm = {
	.name		  = "default_pwrdm",
	.prcm_offs	  = TI81XX_PRM_DEFAULT_MOD,
	.pwrsts		  = PWRSTS_OFF_ON,
	.voltdm           = { .name = "core" },
};

static struct powerdomain ivahd0_816x_pwrdm = {
	.name		  = "ivahd0_pwrdm",
	.prcm_offs	  = TI816X_PRM_IVAHD0_MOD,
	.pwrsts		  = PWRSTS_OFF_ON,
	.voltdm           = { .name = "mpu_iva" },
};

static struct powerdomain ivahd1_816x_pwrdm = {
	.name		  = "ivahd1_pwrdm",
	.prcm_offs	  = TI816X_PRM_IVAHD1_MOD,
	.pwrsts		  = PWRSTS_OFF_ON,
	.voltdm           = { .name = "mpu_iva" },
};

static struct powerdomain ivahd2_816x_pwrdm = {
	.name		  = "ivahd2_pwrdm",
	.prcm_offs	  = TI816X_PRM_IVAHD2_MOD,
	.pwrsts		  = PWRSTS_OFF_ON,
	.voltdm           = { .name = "mpu_iva" },
};

/* FIXME: PRM reg offsets look different compared to other modules */
static struct powerdomain sgx_816x_pwrdm = {
	.name		  = "sgx_pwrdm",
	.prcm_offs	  = TI816X_PRM_SGX_MOD,
	.pwrsts		  = PWRSTS_OFF_ON,
	.voltdm           = { .name = "core" },
};

static struct powerdomain *powerdomains_ti81xx[] __initdata = {
	&wkup_omap2_pwrdm,
	&iva2_pwrdm,
	&mpu_3xxx_pwrdm,
	&core_3xxx_pre_es3_1_pwrdm,

	&alwon_81xx_pwrdm,
	&active_816x_pwrdm,
	&default_816x_pwrdm,
	&ivahd0_816x_pwrdm,
	&ivahd1_816x_pwrdm,
	&ivahd2_816x_pwrdm,
	&sgx_816x_pwrdm,
	NULL
};

void __init ti81xx_powerdomains_init(void)
{
	pwrdm_register_platform_funcs(&ti81xx_pwrdm_operations);
	pwrdm_register_pwrdms(powerdomains_ti81xx);
	pwrdm_complete_init();
}


#endif /* CONFIG_SOC_OMAPTI81XX */

#endif
