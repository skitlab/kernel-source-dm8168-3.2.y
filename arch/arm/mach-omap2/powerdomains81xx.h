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

#include "prcm-common.h"
#include "prm2xxx_3xxx.h"

#if defined(CONFIG_ARCH_TI816X)

static struct powerdomain alwon_816x_pwrdm = {
	.name		  = "alwon_pwrdm",
	.prcm_offs	  = TI816X_PRM_ALWON_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_TI816X),
};

static struct powerdomain active_816x_pwrdm = {
	.name		  = "active_pwrdm",
	.prcm_offs	  = TI816X_PRM_ACTIVE_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_TI816X),
	.pwrsts		  = PWRSTS_OFF_ON,
};

static struct powerdomain default_816x_pwrdm = {
	.name		  = "default_pwrdm",
	.prcm_offs	  = TI816X_PRM_DEFAULT_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_TI816X),
	.pwrsts		  = PWRSTS_OFF_ON,
};

static struct powerdomain ivahd0_816x_pwrdm = {
	.name		  = "ivahd0_pwrdm",
	.prcm_offs	  = TI816X_PRM_IVAHD0_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_TI816X),
	.pwrsts		  = PWRSTS_OFF_ON,
};

static struct powerdomain ivahd1_816x_pwrdm = {
	.name		  = "ivahd1_pwrdm",
	.prcm_offs	  = TI816X_PRM_IVAHD1_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_TI816X),
	.pwrsts		  = PWRSTS_OFF_ON,
};

static struct powerdomain ivahd2_816x_pwrdm = {
	.name		  = "ivahd2_pwrdm",
	.prcm_offs	  = TI816X_PRM_IVAHD2_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_TI816X),
	.pwrsts		  = PWRSTS_OFF_ON,
};

/* FIXME: PRM reg offsets look different compared to other modules */
static struct powerdomain sgx_816x_pwrdm = {
	.name		  = "sgx_pwrdm",
	.prcm_offs	  = TI816X_PRM_SGX_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_TI816X),
	.pwrsts		  = PWRSTS_OFF_ON,
};

/*
 * TODO: Add remaining data and pds if any
 *
 */

#endif

#endif
