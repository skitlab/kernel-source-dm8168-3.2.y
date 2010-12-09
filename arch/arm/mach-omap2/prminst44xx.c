/*
 * OMAP4 PRM instance functions
 *
 * Copyright (C) 2009 Nokia Corporation
 * Paul Walmsley
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/io.h>

#include <plat/common.h>

#include "prm44xx.h"
#include "prminst44xx.h"
#include "prm-regbits-44xx.h"
#include "prcm44xx.h"
#include "prcm_mpu44xx.h"

static u32 (*_prm_read_fns[OMAP4_MAX_PRCM_PARTITIONS])(s16, u16)  = {
	[OMAP4430_INVALID_PRCM_PARTITION]	= NULL,
	[OMAP4430_PRM_PARTITION]		= &omap4_prm_read_inst_reg,
	[OMAP4430_CM1_PARTITION]		= NULL,
	[OMAP4430_CM2_PARTITION]		= NULL,
	[OMAP4430_SCRM_PARTITION]		= NULL,
	[OMAP4430_PRCM_MPU_PARTITION]		= &omap4_prcm_mpu_read_inst_reg
};

static void (*_prm_write_fns[OMAP4_MAX_PRCM_PARTITIONS])(u32, s16, u16)  = {
	[OMAP4430_INVALID_PRCM_PARTITION]	= NULL,
	[OMAP4430_PRM_PARTITION]		= &omap4_prm_write_inst_reg,
	[OMAP4430_CM1_PARTITION]		= NULL,
	[OMAP4430_CM2_PARTITION]		= NULL,
	[OMAP4430_SCRM_PARTITION]		= NULL,
	[OMAP4430_PRCM_MPU_PARTITION]		= &omap4_prcm_mpu_write_inst_reg
};

/* Read a register in a PRM instance */
u32 omap4_prminst_read_inst_reg(u8 part, s16 inst, u16 idx)
{
	BUG_ON(part >= OMAP4_MAX_PRCM_PARTITIONS ||
	       part == OMAP4430_INVALID_PRCM_PARTITION ||
	       !_prm_read_fns[part]);
	return _prm_read_fns[part](inst, idx);
}

/* Write into a register in a PRM instance */
void omap4_prminst_write_inst_reg(u32 val, u8 part, s16 inst, u16 idx)
{
	BUG_ON(part >= OMAP4_MAX_PRCM_PARTITIONS ||
	       part == OMAP4430_INVALID_PRCM_PARTITION ||
	       !_prm_write_fns[part]);
	_prm_write_fns[part](val, inst, idx);
}

/* Read-modify-write a register in PRM. Caller must lock */
u32 omap4_prminst_rmw_inst_reg_bits(u32 mask, u32 bits, u8 part, s16 inst,
				   s16 idx)
{
	u32 v;

	v = omap4_prminst_read_inst_reg(part, inst, idx);
	v &= ~mask;
	v |= bits;
	omap4_prminst_write_inst_reg(v, part, inst, idx);

	return v;
}
