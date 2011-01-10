/*
 * Clock data for ti814x.
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

#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/clk.h>

#include <plat/clkdev_omap.h>

#include "control.h"
#include "clock.h"
#include "clock81xx.h"
#include "cm.h"
#include "cm81xx.h"
#include "cm-regbits-81xx.h"
#include "prm.h"

/*
 * Notes:
 *
 * - Various leaf modules clocks don't have separate interface and functional
 *   clock gating controls. Moreover, interface clock parents (SYSCLKs) do not
 *   have enable/disable controls. Thus, we can probably remove interface clocks
 *   and use dummy clocks instead. Only issue could be related to disabling
 *   SYSCLK parents (PLL). At present, the 'ick' are provided with clkops_null
 *   operations for enable/disable (since they are taken care by 'fck's).
 *
 * - Skipping PLL data and configuration for now. All the PLL (root) clocks are
 *   referred with their default rates.
 *
 * - Use 'null' operations for few SYSCLKs (such as 4, 5, 6 etc.), which do not
 *   have software control for enable/disable (control registers are RO).
 *
 * - Numbering for modules such as UART, I2C etc., which have multiple
 *   instances, starts from 1 while register definitions are maintained starting
 *   from 0 as per spec. This is followed to avoid confusion with omaps, where
 *   the numbering start from 1. Some exceptions include Timers, IVAHDs.
 *
 * - The IDLEST bit and register for many clocks (e.g., mmchs1_fck) do not match
 *   with the default implementation as part of clkops_omap2_dflt_wait so for
 *   now, we use ti814x clock ops for idlest wait.
 *
 * - Note that the above is not applicable for some modules such as Ducati, GEM
 *   and IVAHD clocks as they do not actually get enabled even after clk_enable
 *   as "wait for enable" throws errors. For such modules, we retain
 *   clkops_omap2_dflt and rely on module owners to poll for enable status
 *   before accessing respective module.
 */

/************************** Dummy Clocks *********************************/
static struct clk rcosc_32k_ck = {
	.name		= "rcosc_32k_ck",
	.ops		= &clkops_null,
	.rate		= 32768,
	.flags		= RATE_IN_TI814X,
};

static struct clk sys_32k_clkin_ck = {
	.name		= "sys_32k_clkin_ck",
	.ops		= &clkops_null,
	.rate		= 32768,
	.flags		= RATE_IN_TI814X,
};

static struct clk tclkin_ck = {
	.name		= "tclkin_ck",
	.ops		= &clkops_null,
	.rate		= 32768,	/* TODO: Check */
	.flags		= RATE_IN_TI814X,
};

static struct clk osc0_clkin_ck = {
	.name		= "osc0_clkin_ck",
	.ops		= &clkops_null,
	.rate		= 20000000,
	.flags		= RATE_IN_TI814X,
};

static struct clk osc1_clkin_ck = {
	.name		= "osc1_clkin_ck",
	.ops		= &clkops_null,
	.rate		= 22579000, /* Typically at 27M or 22.579MHz */
	.flags		= RATE_IN_TI814X,
};

static struct clk rtc_divider_out_ck = {
	.name		= "rtc_divider_out_ck",
	.ops		= &clkops_null,
	.rate		= 32768,
	.flags		= RATE_IN_TI814X,
};

static struct clk osc1_x1_ck = {
	.name		= "osc1_x1_ck",
	.ops		= &clkops_null,
	.rate		= 20000000, /* ?? */
	.flags		= RATE_IN_TI814X,
};

static struct clk xref0_ck = {
	.name		= "xref0_ck",
	.ops		= &clkops_null,
	.rate		= 27000000,
	.flags		= RATE_IN_TI814X,
};

static struct clk xref1_ck = {
	.name		= "xref1_ck",
	.ops		= &clkops_null,
	.rate		= 27000000,
	.flags		= RATE_IN_TI814X,
};

static struct clk xref2_ck = {
	.name		= "xref2_ck",
	.ops		= &clkops_null,
	.rate		= 27000000,
	.flags		= RATE_IN_TI814X,
};

static struct clk tsi0_dck_ck = {
	.name		= "tsi0_dck_ck",
	.ops		= &clkops_null,
	.rate		= 82000000,
	.flags		= RATE_IN_TI814X,
};

static struct clk tsi1_dck_ck = {
	.name		= "tsi1_dck_ck",
	.ops		= &clkops_null,
	.rate		= 82000000,
	.flags		= RATE_IN_TI814X,
};

static struct clk logic0_ck = {
	.name		= "logic0_ck",
	.ops		= &clkops_null,
	.rate		= 0,
	.flags		= RATE_IN_TI814X,
};

static struct clk gndclks_ck = {
	.name		= "gndclks_ck",
	.ops		= &clkops_null,
	.rate		= 0, /* ?? */
	.flags		= RATE_IN_TI814X,
};

static struct clk external_ck = {
	.name		= "external_ck",
	.ops		= &clkops_null,
	.rate		= 50000000,
	.flags		= RATE_IN_TI814X,
};

/* ATL dummy output clocks  */
static struct clk atl0_clk_ck = {
	.name		= "atl0_clk_ck",
	.ops		= &clkops_null,
	.rate		= 22579000, /* ?? */
	.flags		= RATE_IN_TI814X,
};

static struct clk atl1_clk_ck = {
	.name		= "atl1_clk_ck",
	.ops		= &clkops_null,
	.rate		= 22579000,
	.flags		= RATE_IN_TI814X,
};

static struct clk atl2_clk_ck = {
	.name		= "atl2_clk_ck",
	.ops		= &clkops_null,
	.rate		= 22579000,
	.flags		= RATE_IN_TI814X,
};

static struct clk atl3_clk_ck = {
	.name		= "atl3_clk_ck",
	.ops		= &clkops_null,
	.rate		= 22579000,
	.flags		= RATE_IN_TI814X,
};

/* ATL dummy refsync clocks  */
static struct clk refsync0_ck = {
	.name		= "refsync0_ck",
	.ops		= &clkops_null,
	.rate		= 27000000,
	.flags		= RATE_IN_TI814X,
};

static struct clk refsync1_ck = {
	.name		= "refsync1_ck",
	.ops		= &clkops_null,
	.rate		= 27000000,
	.flags		= RATE_IN_TI814X,
};

static struct clk refsync2_ck = {
	.name		= "refsync2_ck",
	.ops		= &clkops_null,
	.rate		= 27000000,
	.flags		= RATE_IN_TI814X,
};

static struct clk refsync3_ck = {
	.name		= "refsync3_ck",
	.ops		= &clkops_null,
	.rate		= 27000000,
	.flags		= RATE_IN_TI814X,
};

static struct clk refsync4_ck = {
	.name		= "refsync4_ck",
	.ops		= &clkops_null,
	.rate		= 27000000,
	.flags		= RATE_IN_TI814X,
};

static struct clk refsync5_ck = {
	.name		= "refsync5_ck",
	.ops		= &clkops_null,
	.rate		= 27000000,
	.flags		= RATE_IN_TI814X,
};

static struct clk refsync6_ck = {
	.name		= "refsync6_ck",
	.ops		= &clkops_null,
	.rate		= 27000000,
	.flags		= RATE_IN_TI814X,
};

static struct clk refsync7_ck = {
	.name		= "refsync7_ck",
	.ops		= &clkops_null,
	.rate		= 27000000,
	.flags		= RATE_IN_TI814X,
};

static struct clk refsync8_ck = {
	.name		= "refsync8_ck",
	.ops		= &clkops_null,
	.rate		= 27000000,
	.flags		= RATE_IN_TI814X,
};

static struct clk refsync9_ck = {
	.name		= "refsync9_ck",
	.ops		= &clkops_null,
	.rate		= 27000000,
	.flags		= RATE_IN_TI814X,
};

static struct clk refsync10_ck = {
	.name		= "refsync10_ck",
	.ops		= &clkops_null,
	.rate		= 27000000,
	.flags		= RATE_IN_TI814X,
};

/********************* MUX Inputs ************************************/
static const struct clksel_rate div_1_0_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_TI814X },
	{ .div = 0 },
};

static const struct clksel_rate div_1_1_rates[] = {
	{ .div = 1, .val = 1, .flags = RATE_IN_TI814X },
	{ .div = 0 },
};

static const struct clksel_rate div_1_2_rates[] = {
	{ .div = 1, .val = 2, .flags = RATE_IN_TI814X },
	{ .div = 0 },
};

static const struct clksel_rate div_1_3_rates[] = {
	{ .div = 1, .val = 3, .flags = RATE_IN_TI814X },
	{ .div = 0 },
};

static const struct clksel_rate div_1_4_rates[] = {
	{ .div = 1, .val = 4, .flags = RATE_IN_TI814X },
	{ .div = 0 },
};

static const struct clksel_rate div_1_5_rates[] = {
	{ .div = 1, .val = 5, .flags = RATE_IN_TI814X },
	{ .div = 0 },
};

static const struct clksel_rate div_1_6_rates[] = {
	{ .div = 1, .val = 6, .flags = RATE_IN_TI814X },
	{ .div = 0 },
};

static const struct clksel_rate div_1_7_rates[] = {
	{ .div = 1, .val = 7, .flags = RATE_IN_TI814X },
	{ .div = 0 },
};

/************************ Clock divisions ********************************/
/* 1/2 Division */
static const struct clksel_rate div_2_1_rates[] = {
	{ .div = 2, .val = 1, .flags = RATE_IN_TI814X },
	{ .div = 0 },
};

/* 1/5 Division of Clock */
static const struct clksel_rate div_5_1_rates[] = {
	{ .div = 5, .val = 1, .flags = RATE_IN_TI814X },
	{ .div = 0 },
};

/* 1/10 Division of Clock */
static const struct clksel_rate div_10_1_rates[] = {
	{ .div = 10, .val = 1, .flags = RATE_IN_TI814X },
	{ .div = 0 },
};

/* Divide by 8 clock rates with default clock is 1/1*/
static const struct clksel_rate div8_default1_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_TI814X },
	{ .div = 2, .val = 1, .flags = RATE_IN_TI814X },
	{ .div = 3, .val = 2, .flags = RATE_IN_TI814X },
	{ .div = 4, .val = 3, .flags = RATE_IN_TI814X },
	{ .div = 5, .val = 4, .flags = RATE_IN_TI814X },
	{ .div = 6, .val = 5, .flags = RATE_IN_TI814X },
	{ .div = 7, .val = 6, .flags = RATE_IN_TI814X },
	{ .div = 8, .val = 7, .flags = RATE_IN_TI814X },
	{ .div = 0 },
};

/* Divide by 8 clock rates with default clock is 1/3*/
static const struct clksel_rate div8_default3_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_TI814X },
	{ .div = 2, .val = 1, .flags = RATE_IN_TI814X },
	{ .div = 3, .val = 2, .flags = RATE_IN_TI814X },
	{ .div = 4, .val = 3, .flags = RATE_IN_TI814X },
	{ .div = 5, .val = 4, .flags = RATE_IN_TI814X },
	{ .div = 6, .val = 5, .flags = RATE_IN_TI814X },
	{ .div = 7, .val = 6, .flags = RATE_IN_TI814X },
	{ .div = 8, .val = 7, .flags = RATE_IN_TI814X },
	{ .div = 0 },
};

static const struct clksel_rate div2_sysclk6_rates[] = {
	{ .div = 1, .val = 0, .flags = RATE_IN_TI814X },
	{ .div = 2, .val = 1, .flags = RATE_IN_TI814X },
	{ .div = 0 },
};
/************************** Clocking Started *********************************/

/* DCAN0 Func Clock(final) No PRCM */
static struct clk dcan0_fck = {
	.name		= "dcon0_fck",
	.parent		= &osc0_clkin_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* DCAN1 Func Clock(final) No PRCM */
static struct clk dcan1_fck = {
	.name		= "dcon1_fck",
	.parent		= &osc0_clkin_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* SmartReflex0 Sclk No PRCM */
static struct clk sr0_fck = {
	.name		= "sr0_fck",
	.parent		= &osc0_clkin_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* SmartReflex1 Sclk No PRCM */
static struct clk sr1_fck = {
	.name		= "sr1_fck",
	.parent		= &osc0_clkin_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* SmartReflex2 Sclk No PRCM */
static struct clk sr2_fck = {
	.name		= "sr2_fck",
	.parent		= &osc0_clkin_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* SmartReflex3 Sclk No PRCM */
static struct clk sr3_fck = {
	.name		= "sr3_fck",
	.parent		= &osc0_clkin_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/*********************************Main Module*********************************/
/* ARM DPLL/Modena */
static const struct clksel arm_dpll_mux_sel[] = {
	{ .parent = &osc0_clkin_ck, .rates = div_1_0_rates },
	{ .parent = &rtc_divider_out_ck, .rates = div_1_1_rates },
	{ .parent = NULL}
};

/* Clock input select for ARM_DPLL */
static struct clk arm_dpll_clkin_ck = {
	.name		= "arm_dpll_clkin_ck",
	.parent		= &osc0_clkin_ck,
	.clksel		= arm_dpll_mux_sel,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

/* ARM_DPLL clock out(DPLL out) */
static struct clk arm_dpll_ck = {
	.name		= "arm_dpll_ck",
	.ops		= &clkops_null,
	.rate		= 600000000,
	.flags		= RATE_IN_TI814X,
};

/* CortexA8 Func Clock (final) */
static struct clk mpu_ck = {
	.name		= "mpu_ck",
	.parent		= &arm_dpll_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* DSP_DPLL for clock1(in)*/
static struct clk dsp_dpll_clk1_ck = {
	.name		= "dsp_dpll_clk1_ck",
	.ops		= &clkops_null,
	.rate		= 500000000,
	.flags		= RATE_IN_TI814X,
};

/* DSP Func Clock(final) bypass PRCM */
static struct clk dsp_fck = {
	.name		= "dsp_fck",
	.parent		= &dsp_dpll_clk1_ck,
	.init		= &omap2_init_clksel_parent,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI814X_CM_DSP_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "dsp_clkdm",
	.recalc		= &followparent_recalc,
};

/* GFX_DPLL for clock3(PRCM in) */
static struct clk gfx_dpll_clk2_ck = {
	.name		= "gfx_dpll_clk2_ck",
	.ops		= &clkops_null,
	.rate		= 200000000,
	.flags		= RATE_IN_TI814X,
};

static const struct clksel sysclk23_div[] = {
	{ .parent = &gfx_dpll_clk2_ck, .rates = div8_default3_rates },
	{ .parent = NULL },
};

/* SYSCLK23(PRCM out) */
static struct clk sysclk23_ck = {
	.name		= "sysclk23_ck",
	.parent		= &gfx_dpll_clk2_ck,
	.clksel		= sysclk23_div,
	.init		= &omap2_init_clksel_parent,
	.ops		= &clkops_null,
	.clksel_reg	= TI81XX_CM_DPLL_SYSCLK23_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_1_MASK,
	.recalc		= &omap2_clksel_recalc,
};

/* GFX Func Clocks (CORE clk final) */
static struct clk gfx_fck = {
	.name		= "gfx_fck",
	.parent		= &sysclk23_ck,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI814X_CM_GFX_GFX_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "gfx_clkdm",
	.recalc		= &followparent_recalc,
};

/* GFX SYSCLK Clock (final) */
static struct clk gfx_sys_fck = {
	.name		= "gfx_sys_fck",
	.parent		= &sysclk23_ck,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI814X_CM_GFX_GFX_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "gfx_clkdm",
	.recalc		= &followparent_recalc,
};

/* GFX MEMCLK Clock (final) */
static struct clk gfx_mem_fck = {
	.name		= "gfx_mem_fck",
	.parent		= &sysclk23_ck,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI814X_CM_GFX_GFX_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "gfx_clkdm",
	.recalc		= &followparent_recalc,
};

/* HDVICP_DPLL for clock3(PRCM in) */
static struct clk hdvicp_dpll_clk3_ck = {
	.name		= "hdvicp_dpll_clk3_ck",
	.ops		= &clkops_null,
	.rate		= 266000000,
	.flags		= RATE_IN_TI814X,
};

static const struct clksel sysclk3_div[] = {
	{ .parent = &hdvicp_dpll_clk3_ck, .rates = div8_default1_rates },
	{ .parent = NULL },
};

/* SYSCLK3(PRCM out) */
static struct clk sysclk3_ck = {
	.name		= "sysclk3_ck",
	.parent		= &hdvicp_dpll_clk3_ck,
	.clksel		= sysclk3_div,
	.init		= &omap2_init_clksel_parent,
	.ops		= &clkops_null,
	.clksel_reg	= TI81XX_CM_DPLL_SYSCLK3_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_2_MASK,
	.recalc		= &omap2_clksel_recalc,
};

/* HDVICP Funct Clock (final) */
static struct clk hdvicp_fck = {
	.name		= "hdvicp_fck",
	.parent		= &sysclk3_ck,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI814X_CM_HDVICP_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "hdvicp_clkdm",
	.recalc		= &followparent_recalc,
};

static const struct clksel osc_l3dpll_clk_mux_sel[] = {
	{ .parent = &osc0_clkin_ck, .rates = div_1_0_rates },
	{ .parent = &osc1_clkin_ck, .rates = div_1_1_rates },
	{ .parent = NULL}
};

/* Clock input select for L3 DPLL */
static struct clk l3_dpll_clkin_ck = {
	.name		= "l3_dpll_clkin_ck",
	.parent		= &osc0_clkin_ck,
	.clksel		= osc_l3dpll_clk_mux_sel,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

/* L3 DPLL for clock4(PRCM in) */
static struct clk l3_dpll_clk4_ck = {
	.name		= "l3_dpll_clk4_ck",
	.ops		= &clkops_null,
	.rate		= 200000000, /* 220000000 in PG2 */
	.flags		= RATE_IN_TI814X,
};

/* SYSCLK4(PRCM out) */
static struct clk sysclk4_ck = {
	.name		= "sysclk4_ck",
	.parent		= &l3_dpll_clk4_ck,
	.init		= &omap2_init_clksel_parent,
	.ops		= &clkops_null,
	.clksel_reg	= TI81XX_CM_DPLL_SYSCLK4_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_0_MASK,
	.recalc		= &omap2_clksel_recalc,
};

/* L3 Fast Clock(final) */
static struct clk l3_fast_ick = {
	.name		= "l3_fast_ick",
	.parent		= &sysclk4_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_L3_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* HDVICP Clock(final) */
static struct clk hdvicp_ick = {
	.name		= "hdvicp_ick",
	.parent		= &sysclk4_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "hdvicp_clkdm",
	.recalc		= &followparent_recalc,
};

/* expansion slot(master) Clock(final) */
static struct clk exp_slot_ick = {
	.name		= "exp_slot_ick",
	.parent		= &sysclk4_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "hdvicp_clkdm",
	.recalc		= &followparent_recalc,
};

/* MMU Clock(final) */
static struct clk mmu_ick = {
	.name		= "mmu_ick",
	.parent		= &sysclk4_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_MMUDATA_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "mmu_clkdm",
	.recalc		= &followparent_recalc,
};

/* DSP Clock (final) */
static struct clk dsp_ick = {
	.name		= "dsp_ick",
	.parent		= &sysclk4_ck,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI814X_CM_DSP_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "dsp_clkdm",
	.recalc		= &followparent_recalc,
};

/* ISS Clock (final) */
static struct clk iss_ick = {
	.name		= "iss_ick",
	.parent		= &sysclk4_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* TPTC0 Clock(final) */
static struct clk tptc0_ick = {
	.name		= "tptc0_ick",
	.parent		= &sysclk4_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_TPTC0_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.recalc		= &followparent_recalc,
};

/* TPTC1 Clock(final) */
static struct clk tptc1_ick = {
	.name		= "tptc1_ick",
	.parent		= &sysclk4_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_TPTC1_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.recalc		= &followparent_recalc,
};

/* TPTC2 Clock(final) */
static struct clk tptc2_ick = {
	.name		= "tptc2_ick",
	.parent		= &sysclk4_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_TPTC2_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.recalc		= &followparent_recalc,
};

/* TPTC3 Clock(final) */
static struct clk tptc3_ick = {
	.name		= "tptc3_ick",
	.parent		= &sysclk4_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_TPTC3_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.recalc		= &followparent_recalc,
};

/* HDVICP sl2 Clock (final) */
static struct clk hdvicp_sl2_ick = {
	.name		= "hdvicp_sl2_ick",
	.parent		= &sysclk4_ck,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI814X_CM_HDVICP_SL2_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "hdvicp_clkdm",
	.recalc		= &followparent_recalc,
};

/* TPCC Clock(final) */
static struct clk tpcc_ick = {
	.name		= "tpcc_ick",
	.parent		= &sysclk4_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_TPCC_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.recalc		= &followparent_recalc,
};

static const struct clksel sysclk5_div[] = {
	{ .parent = &sysclk4_ck, .rates = div_1_0_rates },
	{ .parent = NULL },
};

/* SYSCLK5(PRCM out) */
static struct clk sysclk5_ck = {
	.name		= "sysclk5_ck",
	.parent		= &sysclk4_ck,
	.clksel		= sysclk5_div,
	.init		= &omap2_init_clksel_parent,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* L3 Mid Clock(final) */
static struct clk l3_med_ick = {
	.name		= "l3_med_ick",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_L3_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon2_l3_med_clkdm",
	.recalc		= &followparent_recalc,
};

/* L4 Fast Clock(final) */
static struct clk l4_fast_ick = {
	.name		= "l4_fast_ick",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_L4HS_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* gfx_sys Clock (final) */
static struct clk gfx_sys_ick = {
	.name		= "gfx_sys_ick",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "gfx_clkdm",
	.recalc		= &followparent_recalc,
};

/* ISS_Ducati Clock (final) */
static struct clk gfx_mem_ick = {
	.name		= "gfx_mem_ick",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_omap2_dflt,
	.clkdm_name	= "gfx_clkdm",
	.recalc		= &followparent_recalc,
};

/* ISS_Ducati Clock (final) */
static struct clk ducati_ick = {
	.name		= "ducati_ick",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI814X_CM_ALWON2_MC_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "isp_clkdm",
	.recalc		= &followparent_recalc,
};

/* Security SS bridge Clock (final) */
static struct clk securityss_bdg_ick = {
	.name		= "securityss_bdg_ick",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI81XX_CM_ALWON_SECSS_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* 3PGSW Clock(final) */
static struct clk tpgsw_ick = {
	.name		= "tpgsw_ick",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* SATA Clock(final) */
static struct clk sata_ick = {
	.name		= "sata_ick",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI814X_CM_ALWON2_SATA_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon2_l3_med_clkdm",
	.recalc		= &followparent_recalc,
};

/* PCIe Clock(final) */
static struct clk pcie_ck = {
	.name		= "pcie_ck",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_ti81xx_pcie,
	.enable_reg	= TI814X_CM_ALWON2_PCI_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon2_pcie_clkdm",
	.recalc		= &followparent_recalc,
};

/* VCP2 Clock(final) */
static struct clk vcp2_fck = {
	.name		= "vcp2_fck",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI814X_CM_ALWON_VCP_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_fast_clkdm",
	.recalc		= &followparent_recalc,
};

/* MLB Clock(final) */
static struct clk mlb_ick = {
	.name		= "mlb_ick",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI814X_CM_ALWON_MLB_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* FDIF Clock(final) */
static struct clk fdif_ick = {
	.name		= "fdif_ick",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI814X_CM_ISP_FDIF_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "isp_clkdm",
	.recalc		= &followparent_recalc,
};

/* DAP Clock(final) */
static struct clk dap_ick = {
	.name		= "dap_ick",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* OCMC RAM Func/OCP clk (final) */
static struct clk ocmc_ram_ick = {
	.name		= "ocmc_ram_ick",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* MMCHS2 Clock(final) */
static struct clk mmchs2_ick = {
	.name		= "mmchs2_ick",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* CPSW Clock(final) */
static struct clk cpsw_ick = {
	.name		= "cpsw_ick",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_ETHERNET_0_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon1_ethernet_clkdm",
	.recalc		= &followparent_recalc,
};

/* DSP Trace Fun Clock(final) */
static struct clk dsp_trc_fck = {
	.name		= "dsp_trc_fck",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "dsp_clkdm",
	.recalc		= &followparent_recalc,
};

/* McASP3 Clock(final) */
static struct clk mcasp3_ick = {
	.name		= "mcasp3_ick",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* McASP4 Clock(final) */
static struct clk mcasp4_ick = {
	.name		= "mcasp4_ick",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* McASP5 Clock(final) */
static struct clk mcasp5_ick = {
	.name		= "mcasp5_ick",
	.parent		= &sysclk5_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

static const struct clksel sysclk6_div[] = {
	{ .parent = &sysclk4_ck, .rates = div2_sysclk6_rates},
	{ .parent = NULL },
};

/* SYSCLK6(out) */
static struct clk sysclk6_ck = {
	.name		= "sysclk6_ck",
	.parent		= &sysclk4_ck,
	.clksel		= sysclk6_div,
	.init		= &omap2_init_clksel_parent,
	.ops		= &clkops_null,
	.clksel_reg	= TI81XX_CM_DPLL_SYSCLK6_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_0_MASK,
	.recalc		= &omap2_clksel_recalc,
};

/* L3 Slow Clock(final) */
static struct clk l3_slow_ick = {
	.name		= "l3_slow_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_L3_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* L4 Slow Clock(final) */
static struct clk l4_slow_ick = {
	.name		= "l4_slow_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_L4LS_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* UART1 Clock(final) */
static struct clk uart1_ick = {
	.name		= "uart1_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* UART2 Clock(final) */
static struct clk uart2_ick = {
	.name		= "uart2_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* UART3 Clock(final) */
static struct clk uart3_ick = {
	.name		= "uart3_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* UART4 Clock(final) */
static struct clk uart4_ick = {
	.name		= "uart4_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* UART5 Clock(final) */
static struct clk uart5_ick = {
	.name		= "uart5_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* UART6 Clock(final) */
static struct clk uart6_ick = {
	.name		= "uart6_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* I2C1 Clock(final) */
static struct clk i2c1_ick = {
	.name		= "i2c1_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* I2C2 Clock(final) */
static struct clk i2c2_ick = {
	.name		= "i2c2_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* I2C3 Clock(final) */
static struct clk i2c3_ick = {
	.name		= "i2c3_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* I2C4 Clock(final) */
static struct clk i2c4_ick = {
	.name		= "i2c4_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* McSPI0 Clock(final) */
static struct clk mcspi0_ick = {
	.name		= "mcspi0_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* McSPI1 Clock(final) */
static struct clk mcspi1_ick = {
	.name		= "mcspi1_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* McSPI2 Clock(final) */
static struct clk mcspi2_ick = {
	.name		= "mcspi2_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* McSPI3 Clock(final) */
static struct clk mcspi3_ick = {
	.name		= "mcspi3_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* SDIO Clock(final) */
static struct clk sdio_ick = {
	.name		= "sdio_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/*
 * Points to note about timer conventions:
 * 1) We are referring dmtimers as gptimers to match omap convention
 * 2) Skipping gptimer1 as it is secure mode timer.
 */

/* General TIMER1 Clock(final) */
static struct clk gpt1_ick = {
	.name		= "gpt1_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* General TIMER2 Clock(final) */
static struct clk gpt2_ick = {
	.name		= "gpt2_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* General TIMER3 Clock(final) */
static struct clk gpt3_ick = {
	.name		= "gpt3_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* General TIMER4 Clock(final) */
static struct clk gpt4_ick = {
	.name		= "gpt4_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* General TIMER5 Clock(final) */
static struct clk gpt5_ick = {
	.name		= "gpt5_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* General TIMER6 Clock(final) */
static struct clk gpt6_ick = {
	.name		= "gpt6_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* General TIMER7 Clock(final) */
static struct clk gpt7_ick = {
	.name		= "gpt7_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* General TIMER8 Clock(final) */
static struct clk gpt8_ick = {
	.name		= "gpt8_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* GPIO0 Clock(final) */
static struct clk gpio0_ick = {
	.name		= "gpio0_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_GPIO_0_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* GPIO1 Clock(final) */
static struct clk gpio1_ick = {
	.name		= "gpio1_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_GPIO_1_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* PRCM Clock(final) */
static struct clk prcm_ick = {
	.name		= "prcm_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* Smart card 0 Clock(final) */
static struct clk smartcard0_ick = {
	.name		= "smartcard0_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_SMARTCARD_0_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* Smart card 1 Clock(final) */
static struct clk smartcard1_ick = {
	.name		= "smartcard1_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_SMARTCARD_1_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* McASP0 Clock(final) */
static struct clk mcasp0_ick = {
	.name		= "mcasp0_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* McASP1 Clock(final) */
static struct clk mcasp1_ick = {
	.name		= "mcasp1_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* McASP2 Clock(final) */
static struct clk mcasp2_ick = {
	.name		= " mcasp2_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* McBSP Clock(final) */
static struct clk mcbsp_ick = {
	.name		= "mcbsp_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* GPMC Clock(final) / GPMC Functional Clock(final) */
static struct clk gpmc_fck = {
	.name		= "gpmc_fck",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_GPMC_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* HDMI Clock(final) */
static struct clk hdmi_ick = {
	.name		= "hdmi_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* MLB+ Clock(final) */
static struct clk mlbp_ick = {
	.name		= "mlbp_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI814X_CM_ALWON_MLB_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* WDT0 Clock(final) */
static struct clk wdt0_ick = {
	.name		= "wdt0_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_WDTIMER_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* WDT Clock(final) */
static struct clk wdt1_ick = {
	.name		= "wdt1_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* sync_timer_ick  Clock(final) */
static struct clk sync_timer_ick = {
	.name		= "sync_timer_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* PATA Clock(final) */
static struct clk pata_ick = {
	.name		= "pata_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI814X_CM_ALWON_PATA_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* Mail Box Clock(final) */
static struct clk mailbox_ick = {
	.name		= "mailbox_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_MAILBOX_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* SpinBox Clock(final) */
static struct clk spinbox_ick = {
	.name		= "spinbox_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_SPINBOX_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* SmartReflex 0 mclk Clock(final) */
static struct clk sr0_ick = {
	.name		= "sr0_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_SR_0_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* SmartReflex 1 Clock(final) */
static struct clk sr1_ick = {
	.name		= "sr1_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_SR_1_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* SmartReflex 2 Clock(final) */
static struct clk sr2_ick = {
	.name		= "sr2_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_SR_2_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* SmartReflex 3 Clock(final) */
static struct clk sr3_ick = {
	.name		= "sr3_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_SR_3_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* USB0 Clock(final) */
static struct clk usb_ick = {
	.name		= "usb_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_usb,
	.enable_reg	= TI814X_CM_ALWON2_USB_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon2_usb_clkdm",
	.recalc		= &followparent_recalc,
};

/* MMU CFG Clock(final) */
static struct clk mmu_cfg_ick = {
	.name		= "mmu_cfg_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_MMUCFG_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "mmu_cfg_clkdm",
	.recalc		= &followparent_recalc,
};

/* P1500 Clock(final) */
static struct clk p1500_ick = {
	.name		= "p1500_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* ELM Clock(final) */
static struct clk elm_ick = {
	.name		= "elm_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* MMCHS0 Clock(final) */
static struct clk mmchs0_ick = {
	.name		= "mmchs1_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* MMCHS1 Clock(final) */
static struct clk mmchs1_ick = {
	.name		= "mmchs1_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* ATL OCP Clock(final) */
static struct clk atl_ick = {
	.name		= "atl_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* FDIF Config Clock(final) */
static struct clk fdif_cfg_ick = {
	.name		= "fdif_cfg_ick",
	.parent		= &sysclk6_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI814X_CM_ISP_FDIF_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* ISP_DPLL for clock1(in)*/
static struct clk isp_dpll_ck = {
	.name		= "isp_dpll_ck",
	.ops		= &clkops_null,
	.rate		= 400000000,
	.flags		= RATE_IN_TI814X,
};

/* ISP_ISS Clock (final) */
static struct clk isp_iss_ck = {
	.name		= "isp_iss_ck",
	.parent		= &isp_dpll_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "isp_clkdm",
	.recalc		= &followparent_recalc,
};

/* TPPSS TSO Clock (final) */
static struct clk tppss_tso_ick = {
	.name		= "tppss_tso_ick",
	.parent		= &isp_dpll_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "isp_clkdm",
	.recalc		= &followparent_recalc,
};

static const struct clksel isp_iss_div[] = {
	{ .parent = &isp_dpll_ck, .rates = div_2_1_rates },
	{ .parent = NULL },
};

/* securss_mux Clock (mux in) */
static struct clk securss_mux0_ck = {
	.name		= "securss_mux0_ck",
	.parent		= &isp_dpll_ck,
	.clksel		= isp_iss_div,
	.ops		= &clkops_null,
	.clkdm_name	= "isp_clkdm",
	.recalc		= &followparent_recalc,
};

/* HDVPSS_DPLL for clock1(in)*/
static struct clk hdvpss_dpll_ck = {
	.name		= "hdvpss_dpll_ck",
	.ops		= &clkops_null,
	.rate		= 200000000,
	.flags		= RATE_IN_TI814X,
};

/* HDVPSS_proc Clock (final) */
static struct clk hdvpss_proc_fck = {
	.name		= "hdvpss_proc_fck",
	.parent		= &hdvpss_dpll_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "hdvpss_clkdm",
	.recalc		= &followparent_recalc,
};

/* HDVPSS_l3 Clock (final) */
static struct clk hdvpss_l3_ck = {
	.name		= "hdvpss_l3_ck",
	.parent		= &hdvpss_dpll_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "hdvpss_clkdm",
	.recalc		= &followparent_recalc,
};

static const struct clksel hdvpss_div[] = {
	{ .parent = &hdvpss_dpll_ck, .rates = div_2_1_rates },
	{ .parent = NULL },
};

/* HDVPSS_proc_d2 Func Clock (final) */
static struct clk hdvpss_proc_d2_fck = {
	.name		= "hdvpss_proc_d2_fck",
	.parent		= &hdvpss_dpll_ck,
	.clksel		= hdvpss_div,
	.ops		= &clkops_null,
	.clkdm_name	= "hdvpss_clkdm",
	.recalc		= &followparent_recalc,
};

/* HDVPSS_l4 Clock (final) */
static struct clk hdvpss_l4_ck = {
	.name		= "hdvpss_l4_ck",
	.parent		= &hdvpss_dpll_ck,
	.clksel		= isp_iss_div,
	.ops		= &clkops_null,
	.clkdm_name	= "hdvpss_clkdm",
	.recalc		= &followparent_recalc,
};

/******************************USB Module**************************************/
/* Clock input select for USB_DPLL */
static struct clk usb_dpll_clkin_ck = {
	.name		= "usb_dpll_clkin_ck",
	.parent		= &osc0_clkin_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* USB DPLL Clock */
static struct clk usb_dpll_ck = {
	.name		= "usb_dpll_ck",
	.ops		= &clkops_null,
	.rate		= 960000000,
	.flags		= RATE_IN_TI814X,
};

/* USB PHY0 Ref Clock */
static struct clk usb_phy0_rclk_ick = {
	.name		= "usb_phy0_rclk_ick",
	.parent		= &usb_dpll_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon2_usb_clkdm",
	.recalc		= &followparent_recalc,
};

/* USB PHY1 Ref Clock */
static struct clk usb_phy1_rclk_ick = {
	.name		= "usb_phy1_rclk_ick",
	.parent		= &usb_dpll_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon2_usb_clkdm",
	.recalc		= &followparent_recalc,
};

static const struct clksel usb_dpll_div[] = {
	{ .parent = &usb_dpll_ck, .rates = div_5_1_rates },
	{ .parent = NULL },
};

/* USB out clock after division */
static struct clk usb_dpll_div5_ck = {
	.name		= "usb_dpll_clk2_ck",
	.parent		= &usb_dpll_ck,
	.clksel		= usb_dpll_div,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel sysclk10_div[] = {
	{ .parent = &usb_dpll_div5_ck, .rates = div8_default1_rates },
	{ .parent = NULL },
};

/* SYSCLK10(out) */
static struct clk sysclk10_ck = {
	.name		= "sysclk10_ck",
	.parent		= &usb_dpll_div5_ck,
	.clksel		= sysclk10_div,
	.init		= &omap2_init_clksel_parent,
	.ops		= &clkops_null,
	.clksel_reg	= TI81XX_CM_DPLL_SYSCLK10_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_2_MASK,
	.recalc		= &omap2_clksel_recalc,
};

/* SPI0 Functional Clock(final) */
static struct clk mcspi0_fck = {
	.name		= "mcspi0_fck",
	.parent		= &sysclk10_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_SPI_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* SPI1 Functional Clock(final) */
static struct clk mcspi1_fck = {
	.name		= "mcspi1_fck",
	.parent		= &sysclk10_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_SPI_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* SPI2 Functional Clock(final) */
static struct clk mcspi2_fck = {
	.name		= "mcspi2_fck",
	.parent		= &sysclk10_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_SPI_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* SPI3 Functional Clock(final) */
static struct clk mcspi3_fck = {
	.name		= "mcspi3_fck",
	.parent		= &sysclk10_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_SPI_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* I2C02 Functional Clock(final) */
static struct clk i2c02_ck = {
	.name		= "i2c02_ck",
	.parent		= &sysclk10_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI814X_CM_ALWON_I2C_02_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* I2C13 Functional Clock(final) */
static struct clk i2c13_ck = {
	.name		= "i2c13_ck",
	.parent		= &sysclk10_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI814X_CM_ALWON_I2C_13_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* I2C1 Functional Clock(final) */
static struct clk i2c1_fck = {
	.name		= "i2c1_fck",
	.parent		= &i2c02_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* I2C2 Functional Clock(final) */
static struct clk i2c2_fck = {
	.name		= "i2c2_fck",
	.parent		= &i2c13_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* I2C3 Functional Clock(final) */
static struct clk i2c3_fck = {
	.name		= "i2c3_fck",
	.parent		= &i2c02_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* I2C4 Functional Clock(final) */
static struct clk i2c4_fck = {
	.name		= "i2c4_fck",
	.parent		= &i2c13_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* UART0 Functional Clock(final) */
static struct clk uart1_fck = {
	.name		= "uart1_fck",
	.parent		= &sysclk10_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_UART_0_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* UART1 Functional Clock(final) */
static struct clk uart2_fck = {
	.name		= "uart2_fck",
	.parent		= &sysclk10_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_UART_1_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* UART2 Functional Clock(final) */
static struct clk uart3_fck = {
	.name		= "uart3_fck",
	.parent		= &sysclk10_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI81XX_CM_ALWON_UART_2_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* HDMI CEC Clock */
static struct clk hdmi_cec_fck = {
	.name		= "hdmi_cec_fck",
	.parent		= &sysclk10_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

static const struct clksel sysclk8_div[] = {
	{ .parent = &usb_dpll_div5_ck, .rates = div_1_0_rates },
	{ .parent = NULL },
};

/* SYSCLK8(out) */
static struct clk sysclk8_ck = {
	.name		= "sysclk8_ck",
	.parent		= &usb_dpll_div5_ck,
	.clksel		= sysclk8_div,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

/* MMC0 Functional Clock(final) */
static struct clk mmchs0_fck = {
	.name		= "mmchs0_fck",
	.parent		= &sysclk8_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI814X_CM_ALWON_MMCHS_0_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* MMC1 Functional Clock(final) */
static struct clk mmchs1_fck = {
	.name		= "mmchs1_fck",
	.parent		= &sysclk8_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI814X_CM_ALWON_MMCHS_1_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* MMC2 Functional Clock(final) */
static struct clk mmchs2_fck = {
	.name		= "mmchs2_fck",
	.parent		= &sysclk8_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI814X_CM_ALWON_MMCHS_2_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

static const struct clksel uart456_mux_sel[] = {
	{ .parent = &sysclk8_ck, .rates = div_1_0_rates },
	{ .parent = &sysclk10_ck, .rates = div_1_1_rates },
	{ .parent = &sysclk6_ck, .rates = div_1_2_rates },
	{ .parent = NULL}
};

/* uart456_ck Clock */
static struct clk uart456_ck = {
	.name		= "uart456_ck",
	.parent		= &sysclk8_ck,
	.clksel		= uart456_mux_sel,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* UART3 Functional Clock(final) */
static struct clk uart4_fck = {
	.name		= "uart4_fck",
	.parent		= &uart456_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI814X_CM_ALWON_UART_3_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* UART4 Functional Clock(final) */
static struct clk uart5_fck = {
	.name		= "uart5_fck",
	.parent		= &uart456_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI814X_CM_ALWON_UART_4_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* UART5 Functional Clock(final) */
static struct clk uart6_fck = {
	.name		= "uart6_fck",
	.parent		= &uart456_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI814X_CM_ALWON_UART_5_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

static const struct clksel secureSS_mux_sel[] = {
	{ .parent = &securss_mux0_ck, .rates = div_1_0_rates },
	{ .parent = &usb_dpll_div5_ck, .rates = div_1_1_rates },
	{ .parent = NULL}
};

/* SecureSS Func Clock */
static struct clk securess_fck = {
	.name		= "securess_fck",
	.parent		= &securss_mux0_ck,
	.clksel		= secureSS_mux_sel,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI81XX_CM_ALWON_SECSS_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* TPPSS Func Clock */
static struct clk tppss_fck = {
	.name		= "tppss_fck",
	.parent		= &securess_fck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

static const struct clksel csi2_phy_div[] = {
	{ .parent = &usb_dpll_div5_ck, .rates = div_2_1_rates },
	{ .parent = NULL },
};

/* CSI2 PHY Clock */
static struct clk csi2_phy_fck = {
	.name		= "csi2_phy_fck",
	.parent		= &usb_dpll_div5_ck,
	.clksel		= csi2_phy_div,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/**********************************DDR Module**********************************/

/* Clock input select for DDR_DPLL */
static struct clk ddr_dpll_clkin_ck = {
	.name		= "ddr_dpll_clkin_ck",
	.parent		= &osc0_clkin_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* DDR DPLL Clock */
static struct clk ddr_dpll_ck = {
	.name		= "ddr_dpll_ck",
	.ops		= &clkops_null,
	.rate		= 333000000,
	.flags		= RATE_IN_TI814X,
};

/* DDR0 PHY Clock */
static struct clk ddr0_phy_ck = {
	.name		= "ddr0_phy_ck",
	.parent		= &ddr_dpll_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* DDR1 PHY Clock */
static struct clk ddr1_phy_ck = {
	.name		= "ddr1_phy_ck",
	.parent		= &ddr_dpll_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

static const struct clksel ddr_dpll_div[] = {
	{ .parent = &ddr_dpll_ck, .rates = div_2_1_rates },
	{ .parent = NULL },
};

/* DDR0 PHY Func Clock */
static struct clk ddr0_phy_fck = {
	.name		= "ddr0_phy_fck",
	.parent		= &ddr_dpll_ck,
	.clksel		= ddr_dpll_div,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* DDR0 EMIF m Clock */
static struct clk ddr0_emif_mclk_ck = {
	.name		= "ddr0_emif_mclk_ck",
	.parent		= &ddr0_phy_fck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* DMM PHY Func Clock */
static struct clk dmm_phy_fck = {
	.name		= "dmm_phy_fck",
	.parent		= &ddr0_phy_fck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* DDR1 PHY Func Clock */
static struct clk ddr1_phy_fck = {
	.name		= "ddr1_phy_fck",
	.parent		= &ddr0_phy_fck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* DDR1 EMIF m Clock */
static struct clk ddr1_emif_mclk_ck = {
	.name		= "ddr1_emif_mclk_ck",
	.parent		= &ddr0_phy_fck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/******************************Video Module***********************************/
static const struct clksel osc_video0_dpll_mux_sel[] = {
	{ .parent = &osc0_clkin_ck, .rates = div_1_0_rates },
	{ .parent = &osc1_clkin_ck, .rates = div_1_1_rates },
	{ .parent = NULL}
};

/* Clock input select for video0 DPLL */
static struct clk video0_dpll_clkin_ck = {
	.name		= "video0_dpll_clkin_ck",
	.parent		= &osc0_clkin_ck,
	.clksel		= osc_video0_dpll_mux_sel,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* Video0 DPLL clock(DPLL out) */
static struct clk video0_dpll_out_ck = {
	.name		= "video0_dpll_out_ck",
	.ops		= &clkops_null,
	.rate		= 54000000,
	.flags		= RATE_IN_TI814X,
};

/* Video0 DPLL for clock1(PRCM in) */
static struct clk video_dpll_clk1_ck = {
	.name		= "video_dpll_clk1_ck",
	.parent		= &video0_dpll_out_ck,
	.ops		= &clkops_null,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI814X_CM_DPLL_PV0D1_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_2_MASK,
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel osc_video1_dpll_mux_sel[] = {
	{ .parent = &osc0_clkin_ck, .rates = div_1_0_rates },
	{ .parent = &osc1_clkin_ck, .rates = div_1_1_rates },
	{ .parent = NULL}
};

/* Clock input select for video1 DPLL */
static struct clk video1_dpll_clkin_ck = {
	.name		= "video1_dpll_clkin_ck",
	.parent		= &osc0_clkin_ck,
	.clksel		= osc_video1_dpll_mux_sel,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* Video1 DPLL clock (DPLL out) */
static struct clk video1_dpll_out_ck = {
	.name		= "video1_dpll_out_ck",
	.ops		= &clkops_null,
	.rate		= 148500000,
	.flags		= RATE_IN_TI814X,
};

/* Video1 DPLL for clock3(PRCM in) */
static struct clk video_dpll_clk3_ck = {
	.name		= "video_dpll_clk3_ck",
	.parent		= &video1_dpll_out_ck,
	.ops		= &clkops_null,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI814X_CM_DPLL_PV1C1_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_1_MASK,
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel osc_hdmi_dpll_mux_sel[] = {
	{ .parent = &osc0_clkin_ck, .rates = div_1_0_rates },
	{ .parent = &osc1_clkin_ck, .rates = div_1_1_rates },
	{ .parent = NULL}
};

/* Clock input select for HDMI DPLL */
static struct clk hdmi_dpll_clkin_ck = {
	.name		= "hdmi_dpll_clkin_ck",
	.parent		= &osc0_clkin_ck,
	.clksel		= osc_hdmi_dpll_mux_sel,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* HDMI DPLL for clock2 (mux in) */
static struct clk hdmi_dpll_clk2_ck = {
	.name		= "hdmi_dpll_clk2_ck",
	.ops		= &clkops_null,
	.rate		= 27000000,
	.flags		= RATE_IN_TI814X,
};

/* video_m_pclk (mux in) */
static struct clk video_m_pclk_ck = {
	.name		= "video_m_pclk_ck",
	.ops		= &clkops_null,
	.rate		= 165000000,
	.flags		= RATE_IN_TI814X,
};

static const struct clksel video_dpll_clk_mux_sel[] = {
	{ .parent = &hdmi_dpll_clk2_ck, .rates = div_1_0_rates },
	{ .parent = &video_m_pclk_ck, .rates = div_1_1_rates },
	{ .parent = NULL}
};

/* Clock input select for PRCM */
static struct clk hdmi_dpll_muxout_ck = {
	.name		= "hdmi_dpll_muxout_ck",
	.parent		= &hdmi_dpll_clk2_ck,
	.clksel		= video_dpll_clk_mux_sel,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* Video1 DPLL for clock3(PRCM in) */
static struct clk video_dpll_clk2_ck = {
	.name		= "video_dpll_clk2_ck",
	.parent		= &hdmi_dpll_muxout_ck,
	.ops		= &clkops_null,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI814X_CM_DPLL_PV2B3_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_1_MASK,
	.recalc		= &omap2_clksel_recalc,
};

/* PRCM Audio Pll clk4 input */
static const struct clksel audio_dpll_prcm_mux_sel[] = {
	{ .parent = &video0_dpll_out_ck, .rates = div_1_0_rates },
	{ .parent = &video1_dpll_out_ck, .rates = div_1_1_rates },
	{ .parent = &hdmi_dpll_muxout_ck, .rates = div_1_2_rates },
	{ .parent = &logic0_ck, .rates = div_1_3_rates },
	{ .parent = NULL}
};

/* Audio pll prcm clk4 */
static struct clk audio_dpll_clk4_ck = {
	.name		= "audio_dpll_clk4_ck",
	.parent		= &video0_dpll_out_ck,
	.clksel		= audio_dpll_prcm_mux_sel,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel sysclk16_d1_div[] = {
	{ .parent = &video_dpll_clk1_ck, .rates = div8_default1_rates },
	{ .parent = NULL },
};

static struct clk sysclk16_d1mux_ck = {
	.name		= "sysclk16_d1mux_ck",
	.parent		= &video_dpll_clk1_ck,
	.clksel		= sysclk16_d1_div,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel sysclk16_b3_div[] = {
	{ .parent = &video_dpll_clk2_ck, .rates = div8_default1_rates },
	{ .parent = NULL },
};

static struct clk sysclk16_b3mux_ck = {
	.name		= "sysclk16_b3mux_ck",
	.parent		= &video_dpll_clk2_ck,
	.clksel		= sysclk16_b3_div,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel sysclk14_c1_div[] = {
	{ .parent = &video_dpll_clk3_ck, .rates = div8_default1_rates },
	{ .parent = NULL },
};

static struct clk sysclk14_c1mux_ck = {
	.name		= "sysclk14_c1mux_ck",
	.parent		= &video_dpll_clk3_ck,
	.clksel		= sysclk14_c1_div,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel sysclk16_clk_mux_sel[] = {
	{ .parent = &sysclk16_d1mux_ck, .rates = div_1_0_rates },
	{ .parent = &sysclk16_b3mux_ck, .rates = div_1_1_rates },
	{ .parent = NULL}
};

/* SYSCLK16 clock (PRCM out) */
static struct clk sysclk16_ck = {
	.name		= "sysclk16_ck",
	.parent		= &sysclk16_d1mux_ck,
	.clksel		= sysclk16_clk_mux_sel,
	.ops		= &clkops_null,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI81XX_CM_DPLL_SYSCLK16_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_0_MASK,
	.recalc		= &omap2_clksel_recalc,
};

/* TPPSS STC1 Clock (final) */
static struct clk tppss_stc1_fck = {
	.name		= "tppss_stc1_fck",
	.parent		= &sysclk16_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

static const struct clksel hdvpss_clk_mux_sel[] = {
	{ .parent = &video1_dpll_out_ck, .rates = div_1_0_rates },
	{ .parent = &hdmi_dpll_muxout_ck, .rates = div_1_1_rates },
	{ .parent = NULL}
};

/* hd_venc_g clock (PRCM out) */
static struct clk hd_venc_g_ck = {
	.name		= "hd_venc_g_ck",
	.parent		= &video1_dpll_out_ck,
	.clksel		= hdvpss_clk_mux_sel,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* sd_dac clock */
static struct clk sd_dac_ck = {
	.name		= "sd_dac_ck",
	.ops		= &clkops_null,
	.rate		= 54000000,
	.flags		= RATE_IN_TI814X,
};

/* AVDAC Clock (final) */
static struct clk avdac_fck = {
	.name		= "avdac_fclk",
	.parent		= &sd_dac_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

static const struct clksel sysclk14_ck_mux_sel[] = {
	{ .parent = &sysclk16_b3mux_ck, .rates = div_1_0_rates },
	{ .parent = &sysclk14_c1mux_ck, .rates = div_1_1_rates },
	{ .parent = &sys_32k_clkin_ck, .rates = div_1_2_rates },
	{ .parent = NULL}
};

/* SYSCLK14 (PRCM out) */
static struct clk sysclk14_ck = {
	.name		= "sysclk14_ck",
	.parent		= &video_dpll_clk3_ck,
	.clksel		= sysclk14_ck_mux_sel,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel video_dpll_xref_ck_mux_sel[] = {
	{ .parent = &sysclk14_ck, .rates = div_1_0_rates },
	{ .parent = &xref0_ck, .rates = div_1_1_rates },
	{ .parent = &xref1_ck, .rates = div_1_2_rates },
	{ .parent = &xref2_ck, .rates = div_1_3_rates },
	{ .parent = NULL}
};

/* TPPSS STC0 Clock (final) */
static struct clk tppss_stc0_fck = {
	.name		= "tppss_stc0_fck",
	.parent		= &sysclk14_ck,
	.clksel		= video_dpll_xref_ck_mux_sel,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/********************************* Audio Module *******************************/

static const struct clksel tppss_tso_dclk_mux_sel[] = {
	{ .parent = &securss_mux0_ck, .rates = div_1_0_rates },
	{ .parent = &isp_iss_ck, .rates = div_1_1_rates },
	{ .parent = &tsi0_dck_ck, .rates = div_1_2_rates },
	{ .parent = &tsi1_dck_ck, .rates = div_1_3_rates },
	{ .parent = NULL}
};

/* Audio pll Clock2 (PRCM in) */
static struct clk audio_dpll_clk2_ck = {
	.name		= "audio_dpll_clk2_ck",
	.parent		= &securss_mux0_ck,
	.clksel		= tppss_tso_dclk_mux_sel,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel osc_audio_dpll_mux_sel[] = {
	{ .parent = &osc0_clkin_ck, .rates = div_1_0_rates },
	{ .parent = &osc1_clkin_ck, .rates = div_1_1_rates },
	{ .parent = NULL}
};

/* Clock input select for audio PRCM */
static struct clk audio_dpll_clkin_ck = {
	.name		= "audio_dpll_clkin_ck",
	.parent		= &osc0_clkin_ck,
	.clksel		= osc_audio_dpll_mux_sel,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* Audio DPLL for clock3(PRCM in) */
static struct clk audio_dpll_clk3_ck = {
	.name		= "audio_dpll_clk3_ck",
	.ops		= &clkops_null,
	.rate		= 196000000,
	.flags		= RATE_IN_TI814X,
};

/* Audio PLL Clock5 or rtc_div_clkin*/
static struct clk audio_dpll_clk5_ck = {
	.name		= "audio_dpll_clk5_ck",
	.parent		= &osc0_clkin_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel sysclk18_a_div[] = {
	{ .parent = &rtc_divider_out_ck, .rates = div8_default1_rates },
	{ .parent = NULL },
};

/* Audio DPLL out Clock1 */
static struct clk audio_dpll_clk1_ck = {
	.name		= "audio_dpll_clk1_ck",
	.parent		= &rtc_divider_out_ck,
	.clksel		= sysclk18_a_div,
	.ops		= &clkops_null,
	.init		= &omap2_init_clksel_parent,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.clksel_reg	= TI814X_CM_DPLL_RTCDIVA_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_2_MASK,
	.recalc		= &omap2_clksel_recalc,

};

static const struct clksel audio_prcm_mux_sel[] = {
	{ .parent = &rtc_divider_out_ck, .rates = div_1_0_rates },
	{ .parent = &sys_32k_clkin_ck, .rates = div_1_1_rates },
	{ .parent = NULL}
};

/* Clock input select for audio PRCM signal */
static struct clk audio_prcm_clkin_ck = {
	.name		= "audio_prcm_clkin_ck",
	.parent		= &rtc_divider_out_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel sysclk18_mux_sel[] = {
	{ .parent = &audio_prcm_clkin_ck, .rates = div_1_0_rates },
	{ .parent = &audio_dpll_clk1_ck, .rates = div_1_1_rates },
	{ .parent = NULL}
};

/* SYSCLK18 */
static struct clk sysclk18_ck = {
	.name		= "sysclk18_ck",
	.parent		= &audio_prcm_clkin_ck,
	.clksel		= sysclk18_mux_sel,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI81XX_CM_DPLL_SYSCLK18_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_0_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel sysclk19_b_div[] = {
	{ .parent = &audio_dpll_clk2_ck, .rates = div8_default1_rates },
	{ .parent = NULL },
};

/* SYSCLK19 */
static struct clk sysclk19_ck = {
	.name		= "sysclk19_ck",
	.parent		= &audio_dpll_clk2_ck,
	.clksel		= sysclk19_b_div,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI81XX_CM_DPLL_SYSCLK19_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_0_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel sysclk20_c_div[] = {
	{ .parent = &audio_dpll_clk3_ck, .rates = div8_default1_rates },
	{ .parent = NULL },
};

/* SYSCLK20 */
static struct clk sysclk20_ck = {
	.name		= "sysclk20_ck",
	.parent		= &audio_dpll_clk3_ck,
	.clksel		= sysclk20_c_div,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI81XX_CM_DPLL_SYSCLK20_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_0_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel sysclk21_d_div[] = {
	{ .parent = &audio_dpll_clk4_ck, .rates = div8_default1_rates },
	{ .parent = NULL },
};

/* SYSCLK21 */
static struct clk sysclk21_ck = {
	.name		= "sysclk21_ck",
	.parent		= &audio_dpll_clk4_ck,
	.clksel		= sysclk21_d_div,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI81XX_CM_DPLL_SYSCLK21_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_0_MASK,
	.ops		= &clkops_null,
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel sysclk22_e_div[] = {
	{ .parent = &audio_dpll_clk5_ck, .rates = div8_default1_rates },
	{ .parent = NULL },
};

/* SYSCLK22 */
static struct clk sysclk22_ck = {
	.name		= "sysclk22_ck",
	.parent		= &audio_dpll_clk5_ck,
	.clksel		= sysclk22_e_div,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel mcbsp_prcm1_mux_sel[] = {
	{ .parent = &sysclk20_ck, .rates = div_1_0_rates },
	{ .parent = &sysclk21_ck, .rates = div_1_1_rates },
	{ .parent = &sysclk22_ck, .rates = div_1_2_rates },
	{ .parent = &gndclks_ck, .rates = div_1_3_rates },
	{ .parent = NULL}
};

/* Audio pll Clock1 out (PRCM out) */
static struct clk audio_prcm1_out_ck = {
	.name		= "audio_prcm1_out_ck",
	.parent		= &sysclk20_ck,
	.clksel		= mcbsp_prcm1_mux_sel,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel mcbsp_clks_mux_sel[] = {
	{ .parent = &audio_prcm1_out_ck, .rates = div_1_0_rates },
	{ .parent = &osc1_x1_ck, .rates = div_1_1_rates },
	{ .parent = &xref0_ck, .rates = div_1_2_rates },
	{ .parent = &xref1_ck, .rates = div_1_3_rates },
	{ .parent = &xref2_ck, .rates = div_1_4_rates },
	{ .parent = NULL}
};

/* Mcbsp Clocks (PRCM out) */
static struct clk mcbsp_fck = {
	.name		= "mcbsp_fck",
	.parent		= &audio_prcm1_out_ck,
	.clksel		= mcbsp_clks_mux_sel,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI81XX_CM_ALWON_MCBSP_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI81XX_CM_DPLL_AUDIOCLK_MCBSP_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_0_MASK,
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel mcbsp0to2_prcm2345_mux_sel[] = {
	{ .parent = &sysclk20_ck, .rates = div_1_0_rates },
	{ .parent = &sysclk21_ck, .rates = div_1_1_rates },
	{ .parent = &sysclk22_ck, .rates = div_1_2_rates },
	{ .parent = NULL}
};

/* Mcasp0 aux_clk out (PRCM out) */
static struct clk mcasp0_fck = {
	.name		= "mcasp0_fck",
	.parent		= &sysclk20_ck,
	.clksel		= mcbsp0to2_prcm2345_mux_sel,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI81XX_CM_ALWON_MCASP0_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI81XX_CM_DPLL_AUDIOCLK_MCASP0_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_1_MASK,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &omap2_clksel_recalc,
};

/* Mcasp1 aux_clk out (PRCM out) */
static struct clk mcasp1_fck = {
	.name		= "mcasp1_fck",
	.parent		= &sysclk20_ck,
	.clksel		= mcbsp0to2_prcm2345_mux_sel,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI81XX_CM_ALWON_MCASP1_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI81XX_CM_DPLL_AUDIOCLK_MCASP1_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_1_MASK,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &omap2_clksel_recalc,
};

/* Mcasp2 aux_clk out (PRCM out) */
static struct clk mcasp2_fck = {
	.name		= "mcasp2_fck",
	.parent		= &sysclk20_ck,
	.clksel		= mcbsp0to2_prcm2345_mux_sel,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI81XX_CM_ALWON_MCASP2_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI81XX_CM_DPLL_AUDIOCLK_MCASP2_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_1_MASK,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &omap2_clksel_recalc,
};

/* hdmi i2smclk mux in (PRCM out) */
static struct clk hdmi_i2sm_clkin_ck = {
	.name		= "hdmi_i2sm_clkin_ck",
	.parent		= &sysclk20_ck,
	.clksel		= mcbsp0to2_prcm2345_mux_sel,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI814X_CM_HDVPSS_HDMI_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clksel_reg	= TI81XX_CM_DPLL_HDMI_CLKSEL,
	.clksel_mask	= TI81XX_CLKSEL_0_1_MASK,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel hdmi_i2s_mclk_mux_sel[] = {
	{ .parent = &hdmi_i2sm_clkin_ck, .rates = div_1_0_rates },
	{ .parent = &xref0_ck, .rates = div_1_1_rates },
	{ .parent = &xref1_ck, .rates = div_1_2_rates },
	{ .parent = &xref2_ck, .rates = div_1_3_rates },
	{ .parent = &osc1_x1_ck, .rates = div_1_4_rates },
	{ .parent = NULL}
};

/* hdmi i2sm Func clk (MUX out) */
static struct clk hdmi_i2sm_fck = {
	.name		= "hdmi_i2sm_clkin_fck",
	.parent		= &hdmi_i2sm_clkin_ck,
	.clksel		= hdmi_i2s_mclk_mux_sel,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* TPPSS tso clk out (PRCM out) */
static struct clk tppss_tso_fck = {
	.name		= "tppss_tso_fck",
	.parent		= &sysclk19_ck,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI814X_CM_ALWON2_TPPSS_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

static const struct clksel atlp_clk_mux_sel[] = {
	{ .parent = &sysclk19_ck, .rates = div_1_0_rates },
	{ .parent = &audio_dpll_clk3_ck, .rates = div_1_1_rates },
	{ .parent = &audio_dpll_clk4_ck, .rates = div_1_2_rates },
	{ .parent = NULL}
};

/* atl pclk (MUX out) */
static struct clk atl_fck = {
	.name		= "atl_fck",
	.parent		= &sysclk19_ck,
	.clksel		= atlp_clk_mux_sel,
	.ops		= &clkops_ti81xx_dflt_wait,
	.enable_reg	= TI814X_CM_ALWON_ATL_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* gpio0 debounce clk (MUX out) */
static struct clk gpio0_debounce_clk_ck = {
	.name		= "gpio0_debounce_clk_ck",
	.parent		= &sysclk18_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* gpio1 debounce clk (MUX out) */
static struct clk gpio1_debounce_clk_ck = {
	.name		= "gpio1_debounce_clk_ck",
	.parent		= &sysclk18_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* RTC Func clk (MUX out) */
static struct clk rtc_fck = {
	.name		= "rtc_fck",
	.parent		= &sysclk18_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* clk32 for MMC0 debounce clk (MUX out) */
static struct clk mmchs0_debounce_clk_ck = {
	.name		= "mmchs0_debounce_clk_ck",
	.parent		= &sysclk18_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* clk32 for MMC1 debounce clk (MUX out) */
static struct clk mmchs1_debounce_clk_ck = {
	.name		= "mmchs1_debounce_clk_ck",
	.parent		= &sysclk18_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* clk32 for MMC0 debounce clk (MUX out) */
static struct clk mmchs2_debounce_clk_ck = {
	.name		= "mmchs2_debounce_clk_ck",
	.parent		= &sysclk18_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* Sync Timer clk (MUX out) */
static struct clk sync_timer_fck = {
	.name		= "sync_timer_fck",
	.parent		= &sysclk18_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* bandgaps Func clk (MUX out) */
static struct clk bandgaps_fck = {
	.name		= "bandgaps_fck",
	.parent		= &sysclk18_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* arm operational Func clk (MUX out) */
static struct clk arm_oper_fck = {
	.name		= "arm_oper_fck",
	.parent		= &sysclk18_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

static const struct clksel gpt1to8_fclk_mux_sel[] = {
	{ .parent = &sysclk18_ck, .rates = div_1_0_rates },
	{ .parent = &xref0_ck, .rates = div_1_1_rates },
	{ .parent = &xref1_ck, .rates = div_1_2_rates },
	{ .parent = &xref2_ck, .rates = div_1_3_rates },
	{ .parent = &osc0_clkin_ck, .rates = div_1_4_rates },
	{ .parent = &osc1_clkin_ck, .rates = div_1_5_rates },
	{ .parent = &tclkin_ck, .rates = div_1_6_rates },
	{ .parent = NULL, .rates = div_1_7_rates },
	{ .parent = NULL}
};

/* GM Timer Func Clocks */
static struct clk gpt1_fck = {
	.name		= "gpt1_fck",
	.parent		= &sysclk18_ck,
	.clksel		= gpt1to8_fclk_mux_sel,
	.ops		= &clkops_null,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI814X_PLL_DMTIMER_CLKSRC,
	.clksel_mask	= TI814X_DMTIMER1_CLKS_MASK,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &omap2_clksel_recalc,
};

static struct clk gpt2_fck = {
	.name		= "gpt2_fck",
	.parent		= &sysclk18_ck,
	.clksel		= gpt1to8_fclk_mux_sel,
	.ops		= &clkops_null,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI814X_PLL_DMTIMER_CLKSRC,
	.clksel_mask	= TI814X_DMTIMER2_CLKS_MASK,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &omap2_clksel_recalc,
};

static struct clk gpt3_fck = {
	.name		= "gpt3_fck",
	.parent		= &sysclk18_ck,
	.clksel		= gpt1to8_fclk_mux_sel,
	.ops		= &clkops_null,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI814X_PLL_DMTIMER_CLKSRC,
	.clksel_mask	= TI814X_DMTIMER3_CLKS_MASK,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &omap2_clksel_recalc,
};

static struct clk gpt4_fck = {
	.name		= "gpt4_fck",
	.parent		= &sysclk18_ck,
	.clksel		= gpt1to8_fclk_mux_sel,
	.ops		= &clkops_null,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI814X_PLL_DMTIMER_CLKSRC,
	.clksel_mask	= TI814X_DMTIMER4_CLKS_MASK,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &omap2_clksel_recalc,
};

static struct clk gpt5_fck = {
	.name		= "gpt5_fck",
	.parent		= &sysclk18_ck,
	.clksel		= gpt1to8_fclk_mux_sel,
	.ops		= &clkops_null,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI814X_PLL_DMTIMER_CLKSRC,
	.clksel_mask	= TI814X_DMTIMER5_CLKS_MASK,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &omap2_clksel_recalc,
};

static struct clk gpt6_fck = {
	.name		= "gpt6_fck",
	.parent		= &sysclk18_ck,
	.clksel		= gpt1to8_fclk_mux_sel,
	.ops		= &clkops_null,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI814X_PLL_DMTIMER_CLKSRC,
	.clksel_mask	= TI814X_DMTIMER6_CLKS_MASK,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &omap2_clksel_recalc,
};

static struct clk gpt7_fck = {
	.name		= "gpt7_fck",
	.parent		= &sysclk18_ck,
	.clksel		= gpt1to8_fclk_mux_sel,
	.ops		= &clkops_null,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI814X_PLL_DMTIMER_CLKSRC,
	.clksel_mask	= TI814X_DMTIMER7_CLKS_MASK,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &omap2_clksel_recalc,
};

static struct clk gpt8_fck = {
	.name		= "gpt8_fck",
	.parent		= &sysclk18_ck,
	.clksel		= gpt1to8_fclk_mux_sel,
	.ops		= &clkops_null,
	.init		= &omap2_init_clksel_parent,
	.clksel_reg	= TI814X_PLL_DMTIMER_CLKSRC,
	.clksel_mask	= TI814X_DMTIMER8_CLKS_MASK,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &omap2_clksel_recalc,
};

static const struct clksel mlb0to1_clk_mux_sel[] = {
	{ .parent = &mlb_ick, .rates = div_1_0_rates },
	{ .parent = &mlbp_ick, .rates = div_1_1_rates },
	{ .parent = NULL}
};

/* McASP aux clock Mux */
static struct clk mcasp_auxclk_mux0_ck = {
	.name		= "mcasp_auxclk_mux0_ck",
	.parent		= &mlb_ick,
	.clksel		= mlb0to1_clk_mux_sel,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

static const struct clksel mcasp0to6_auxclk_mux_sel[] = {
	{ .parent = &mcasp_auxclk_mux0_ck, .rates = div_1_0_rates },
	{ .parent = &audio_dpll_clk3_ck, .rates = div_1_1_rates },
	{ .parent = &audio_dpll_clk4_ck, .rates = div_1_2_rates },
	{ .parent = &xref0_ck, .rates = div_1_3_rates },
	{ .parent = &xref1_ck, .rates = div_1_4_rates },
	{ .parent = &xref2_ck, .rates = div_1_5_rates },
	{ .parent = &osc1_x1_ck, .rates = div_1_6_rates },
	{ .parent = NULL}
};

/* Mcasp3 aux_clk (mux out) */
static struct clk mcasp3_fck = {
	.name		= "mcasp3_fck",
	.parent		= &mcasp_auxclk_mux0_ck,
	.init		= &omap2_init_clksel_parent,
	.clksel		= mcasp0to6_auxclk_mux_sel,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI814X_CM_ALWON_MCASP_3_4_5_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* Mcasp4 aux_clk (mux out) */
static struct clk mcasp4_fck = {
	.name		= "mcasp4_fck",
	.parent		= &mcasp_auxclk_mux0_ck,
	.init		= &omap2_init_clksel_parent,
	.clksel		= mcasp0to6_auxclk_mux_sel,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI814X_CM_ALWON_MCASP_3_4_5_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* Mcasp5 aux_clk (mux out) */
static struct clk mcasp5_fck = {
	.name		= "mcasp5_fck",
	.parent		= &mcasp_auxclk_mux0_ck,
	.clksel		= mcasp0to6_auxclk_mux_sel,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI814X_CM_ALWON_MCASP_3_4_5_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

static const struct clksel mcasp0to7_ahclk_mux_sel[] = {
	{ .parent = &xref0_ck, .rates = div_1_0_rates },
	{ .parent = &xref1_ck, .rates = div_1_1_rates },
	{ .parent = &xref2_ck, .rates = div_1_2_rates },
	{ .parent = &osc1_x1_ck, .rates = div_1_3_rates },
	{ .parent = &atl0_clk_ck, .rates = div_1_4_rates },
	{ .parent = &atl1_clk_ck, .rates = div_1_5_rates },
	{ .parent = &atl2_clk_ck, .rates = div_1_6_rates },
	{ .parent = &atl3_clk_ck, .rates = div_1_7_rates },
	{ .parent = NULL}
};

/* mcasp0 ahclkx */
static struct clk mcasp0_ahclkx_pi_ck = {
	.name		= "mcasp0_ahclkx_pi_ck",
	.parent		= &xref0_ck,
	.clksel		= mcasp0to7_ahclk_mux_sel,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* mcasp0 ahclkr */
static struct clk mcasp0_ahclkr_pi_ck = {
	.name		= "mcasp0_ahclkr_pi_ck",
	.parent		= &xref0_ck,
	.clksel		= mcasp0to7_ahclk_mux_sel,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* mcasp1 ahclkx */
static struct clk mcasp1_ahclkx_pi_ck = {
	.name		= "mcasp1_ahclkx_pi_ck",
	.parent		= &xref0_ck,
	.clksel		= mcasp0to7_ahclk_mux_sel,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* mcasp1 ahclkr */
static struct clk mcasp1_ahclkr_pi_ck = {
	.name		= "mcasp1_ahclkr_pi_ck",
	.parent		= &xref0_ck,
	.clksel		= mcasp0to7_ahclk_mux_sel,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* mcasp2 ahclkx */
static struct clk mcasp2_ahclkx_pi_ck = {
	.name		= "mcasp2_ahclkx_pi_ck",
	.parent		= &xref0_ck,
	.clksel		= mcasp0to7_ahclk_mux_sel,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* mcasp3 ahclkx */
static struct clk mcasp3_ahclkx_pi_ck = {
	.name		= "mcasp3_ahclkx_pi_ck",
	.parent		= &xref0_ck,
	.clksel		= mcasp0to7_ahclk_mux_sel,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* mcasp4 ahclkx */
static struct clk mcasp4_ahclkx_pi_ck = {
	.name		= "mcasp4_ahclkx_pi_ck",
	.parent		= &xref0_ck,
	.clksel		= mcasp0to7_ahclk_mux_sel,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* mcasp5 ahclkx */
static struct clk mcasp5_ahclkx_pi_ck = {
	.name		= "mcasp5_ahclkx_pi_ck",
	.parent		= &xref0_ck,
	.clksel		= mcasp0to7_ahclk_mux_sel,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/************************** SERDES & Ethernet Clocking ************************/
static const struct clksel cpts0to4_rft_clk_mux_sel[] = {
	{ .parent = &video0_dpll_out_ck, .rates = div_1_0_rates },
	{ .parent = &video1_dpll_out_ck, .rates = div_1_1_rates },
	{ .parent = &audio_dpll_clk3_ck, .rates = div_1_2_rates },
	{ .parent = &hdmi_dpll_muxout_ck, .rates = div_1_3_rates },
	{ .parent = &l3_dpll_clk4_ck, .rates = div_1_4_rates },
	{ .parent = NULL}
};

/* cpts rft clk (mux out to GMAC switch) */
static struct clk cpts_rft_clk_ck = {
	.name		= "cpts_rft_clk_ck",
	.parent		= &video0_dpll_out_ck,
	.clksel		= cpts0to4_rft_clk_mux_sel,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* LJCB input1 clk serdes_xrefclkip_ck */
static struct clk ljcb_clkin_serdes_xrefp_ck = {
	.name		= "ljcb_clkin_serdes_xrefp_ck",
	.ops		= &clkops_null,
	.rate		= 100000000,
	.flags		= RATE_IN_TI814X,
};

/* LJCB input1 clk serdes_xrefclkin_ck */
static struct clk ljcb_clkin_serdes_xrefn_ck = {
	.name		= "ljcb_clkin_serdes_xrefn_ck",
	.ops		= &clkops_null,
	.rate		= 100000000,
	.flags		= RATE_IN_TI814X,
};

/* pciess_20_refin_ck */
static struct clk pciess_20_refin_ck = {
	.name		= "pciess_20_refin_ck",
	.parent		= &osc0_clkin_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* 100MHz diff ref clk (LJCB out and PCIe SS in) */
static struct clk pciess_refp_clk_ck = {
	.name		= "pciess_refp_clk_ck",
	.parent		= &ljcb_clkin_serdes_xrefp_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* 100MHz diff ref clk (LJCB out and PCIe SS in) */
static struct clk pciess_refn_clk_ck = {
	.name		= "pciess_refn_clk_ck",
	.parent		= &ljcb_clkin_serdes_xrefn_ck,
	.ops		= &clkops_null,
	.recalc		= &followparent_recalc,
};

/* 50MHz PCIe SS out */
static struct clk pciess_50_clk_ck = {
	.name		= "pciess_50_clk_ck",
	.ops		= &clkops_null,
	.rate		= 50000000,
	.flags		= RATE_IN_TI814X,
};

/* 125MHz PCIe SS out */
static struct clk pciess_125_clk_ck = {
	.name		= "pciess_125_clk_ck",
	.ops		= &clkops_null,
	.rate		= 125000000,
	.flags		= RATE_IN_TI814X,
};

/* 100MHz diff ref clk out from PCIe  */
static struct clk satass_refp_clk_ck = {
	.name		= "satass_refp_clk_ck",
	.ops		= &clkops_null,
	.rate		= 100000000,
	.flags		= RATE_IN_TI814X,
};

/* 100MHz diff ref clk out from PCIe */
static struct clk satass_refn_clk_ck = {
	.name		= "satass_refn_clk_ck",
	.ops		= &clkops_null,
	.rate		= 100000000,
	.flags		= RATE_IN_TI814X,
};

/* satass_20_refin_ck */
static struct clk satass_20_refin_ck = {
	.name		= "satass_20_refin_ck",
	.ops		= &clkops_null,
	.rate		= 20000000,
	.flags		= RATE_IN_TI814X,
};

/* 50MHz SATA SS out */
static struct clk satass_50_clk_ck = {
	.name		= "satass_50_clk_ck",
	.ops		= &clkops_null,
	.rate		= 50000000,
	.flags		= RATE_IN_TI814X,
};

/* 125MHz SATA SS out */
static struct clk satass_125_clk_ck = {
	.name		= "satass_125_clk_ck",
	.ops		= &clkops_null,
	.rate		= 125000000,
	.flags		= RATE_IN_TI814X,
};

static const struct clksel gmac0to1_mux_sel[] = {
	{ .parent = &satass_50_clk_ck, .rates = div_1_0_rates },
	{ .parent = &external_ck, .rates = div_1_1_rates },
	{ .parent = NULL}
};

/* RMII ref clk */
static struct clk gmac_rmii_ref_ck = {
	.name		= "gmac_rmii_ref_ck",
	.parent		= &satass_50_clk_ck,
	.clksel		= gmac0to1_mux_sel,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* GMII ref clk */
static struct clk gmac_gmii_ref_ck = {
	.name		= "gmac_gmii_ref_ck",
	.parent		= &satass_125_clk_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/************ Watchdog, Secure timer and Sync timer Functional clocking *******/

static const struct clksel wdt0to1_fclk_mux_sel[] = {
	{ .parent = &audio_dpll_clk5_ck, .rates = div_1_0_rates },
	{ .parent = &rcosc_32k_ck, .rates = div_1_1_rates },
	{ .parent = NULL}
};

/* wdt0 Non secure A8 WD clk (mux out) */
static struct clk wdt0_fck = {
	.name		= "wdt0_fck",
	.parent		= &audio_dpll_clk5_ck,
	.clksel		= wdt0to1_fclk_mux_sel,
	.ops		= &clkops_omap2_dflt,
	.enable_reg	= TI81XX_CM_ALWON_WDTIMER_CLKCTRL,
	.enable_bit	= TI81XX_MODULEMODE_SWCTRL,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/* wdt1 Secure A8 WD clk (mux out) */
static struct clk wdt1_fck = {
	.name		= "wdt1_fck",
	.parent		= &rcosc_32k_ck,
	.ops		= &clkops_null,
	.clkdm_name	= "alwon_l3_slow_clkdm",
	.recalc		= &followparent_recalc,
};

/*
 * clkdev
 *
 * FIXME: Some of the external clocks (e.g., tclk) are kept here for
 * completeness.
 */
static struct omap_clk ti814x_clks[] = {
	CLK(NULL,		"rcosc_32k_ck",			&rcosc_32k_ck,			CK_TI814X),
	CLK(NULL,		"sys_32k_clkin_ck",		&sys_32k_clkin_ck,		CK_TI814X),
	CLK(NULL,		"tclkin_ck",			&tclkin_ck,			CK_TI814X),
	CLK(NULL,		"osc0_clkin_ck",		&osc0_clkin_ck,			CK_TI814X),
	CLK(NULL,		"osc1_clkin_ck",		&osc1_clkin_ck,			CK_TI814X),
	CLK(NULL,		"rtc_divider_out_ck",		&rtc_divider_out_ck,		CK_TI814X),
	CLK(NULL,		"osc1_x1_ck",			&osc1_x1_ck,			CK_TI814X),
	CLK(NULL,		"xref0_ck",			&xref0_ck,			CK_TI814X),
	CLK(NULL,		"xref1_ck",			&xref1_ck,			CK_TI814X),
	CLK(NULL,		"xref2_ck",			&xref2_ck,			CK_TI814X),
	CLK(NULL,		"tsi0_dck_ck",			&tsi0_dck_ck,			CK_TI814X),
	CLK(NULL,		"tsi1_dck_ck",			&tsi1_dck_ck,			CK_TI814X),
	CLK(NULL,		"logic0_ck",			&logic0_ck,			CK_TI814X),
	CLK(NULL,		"gndclks_ck",			&gndclks_ck,			CK_TI814X),
	CLK(NULL,		"external_ck",			&external_ck,			CK_TI814X),
	CLK(NULL,		"atl0_clk_ck",			&atl0_clk_ck,			CK_TI814X),
	CLK(NULL,		"atl1_clk_ck",			&atl1_clk_ck,			CK_TI814X),
	CLK(NULL,		"atl2_clk_ck",			&atl2_clk_ck,			CK_TI814X),
	CLK(NULL,		"atl3_clk_ck",			&atl3_clk_ck,			CK_TI814X),
	CLK(NULL,		"refsync0_ck",			&refsync0_ck,			CK_TI814X),
	CLK(NULL,		"refsync1_ck",			&refsync1_ck,			CK_TI814X),
	CLK(NULL,		"refsync2_ck",			&refsync2_ck,			CK_TI814X),
	CLK(NULL,		"refsync3_ck",			&refsync3_ck,			CK_TI814X),
	CLK(NULL,		"refsync4_ck",			&refsync4_ck,			CK_TI814X),
	CLK(NULL,		"refsync5_ck",			&refsync5_ck,			CK_TI814X),
	CLK(NULL,		"refsync6_ck",			&refsync6_ck,			CK_TI814X),
	CLK(NULL,		"refsync7_ck",			&refsync7_ck,			CK_TI814X),
	CLK(NULL,		"refsync8_ck",			&refsync8_ck,			CK_TI814X),
	CLK(NULL,		"refsync9_ck",			&refsync9_ck,			CK_TI814X),
	CLK(NULL,		"refsync10_ck",			&refsync10_ck,			CK_TI814X),
	CLK(NULL,		"dcan0_fck",			&dcan0_fck,			CK_TI814X),
	CLK(NULL,		"dcan1_fck",			&dcan1_fck,			CK_TI814X),
	CLK(NULL,		"sr0_fck",			&sr0_fck,			CK_TI814X),
	CLK(NULL,		"sr1_fck",			&sr1_fck,			CK_TI814X),
	CLK(NULL,		"sr2_fck",			&sr2_fck,			CK_TI814X),
	CLK(NULL,		"sr3_fck",			&sr3_fck,			CK_TI814X),
	CLK(NULL,		"arm_dpll_clkin_ck",		&arm_dpll_clkin_ck,		CK_TI814X),
	CLK(NULL,		"arm_dpll_ck",			&arm_dpll_ck,			CK_TI814X),
	CLK(NULL,		"mpu_ck",			&mpu_ck,			CK_TI814X),
	CLK(NULL,		"dsp_dpll_clk1_ck",		&dsp_dpll_clk1_ck,		CK_TI814X),
	CLK(NULL,		"dsp_fck",			&dsp_fck,			CK_TI814X),
	CLK(NULL,		"gfx_dpll_clk2_ck",		&gfx_dpll_clk2_ck,		CK_TI814X),
	CLK(NULL,		"sysclk23_ck",			&sysclk23_ck,			CK_TI814X),
	CLK(NULL,		"gfx_fck",			&gfx_fck,			CK_TI814X),
	CLK(NULL,		"gfx_sys_fck",			&gfx_sys_fck,			CK_TI814X),
	CLK(NULL,		"gfx_mem_fck",			&gfx_mem_fck,			CK_TI814X),
	CLK(NULL,		"hdvicp_dpll_clk3_ck",		&hdvicp_dpll_clk3_ck,		CK_TI814X),
	CLK(NULL,		"sysclk3_ck",			&sysclk3_ck,			CK_TI814X),
	CLK(NULL,		"hdvicp_fck",			&hdvicp_fck,			CK_TI814X),
	CLK(NULL,		"l3_dpll_clkin_ck",		&l3_dpll_clkin_ck,		CK_TI814X),
	CLK(NULL,		"l3_dpll_clk4_ck",		&l3_dpll_clk4_ck,		CK_TI814X),
	CLK(NULL,		"sysclk4_ck",			&sysclk4_ck,			CK_TI814X),
	CLK(NULL,		"l3_fast_ick",			&l3_fast_ick,			CK_TI814X),
	CLK(NULL,		"hdvicp_ick",			&hdvicp_ick,			CK_TI814X),
	CLK(NULL,		"exp_slot_ick",			&exp_slot_ick,			CK_TI814X),
	CLK(NULL,		"mmu_ick",			&mmu_ick,			CK_TI814X),
	CLK(NULL,		"dsp_ick",			&dsp_ick,			CK_TI814X),
	CLK(NULL,		"iss_ick",			&iss_ick,			CK_TI814X),
	CLK(NULL,		"tptc0_ick",			&tptc0_ick,			CK_TI814X),
	CLK(NULL,		"tptc1_ick",			&tptc1_ick,			CK_TI814X),
	CLK(NULL,		"tptc2_ick",			&tptc2_ick,			CK_TI814X),
	CLK(NULL,		"tptc3_ick",			&tptc3_ick,			CK_TI814X),
	CLK(NULL,		"hdvicp_sl2_ick",		&hdvicp_sl2_ick,		CK_TI814X),
	CLK(NULL,		"tpcc_ick",			&tpcc_ick,			CK_TI814X),
	CLK(NULL,		"sysclk5_ck",			&sysclk5_ck,			CK_TI814X),
	CLK(NULL,		"l3_med_ick",			&l3_med_ick,			CK_TI814X),
	CLK(NULL,		"l4_fast_ick",			&l4_fast_ick,			CK_TI814X),
	CLK(NULL,		"gfx_sys_ick",			&gfx_sys_ick,			CK_TI814X),
	CLK(NULL,		"gfx_mem_ick",			&gfx_mem_ick,			CK_TI814X),
	CLK(NULL,		"ducati_ick",			&ducati_ick,			CK_TI814X),
	CLK(NULL,		"securityss_bdg_ick",		&securityss_bdg_ick,		CK_TI814X),
	CLK(NULL,		"tpgsw_ick",			&tpgsw_ick,			CK_TI814X),
	CLK(NULL,		"sata_ick",			&sata_ick,			CK_TI814X),
	CLK(NULL,		"pcie_ck",			&pcie_ck,			CK_TI814X),
	CLK(NULL,		"vcp2_fck",			&vcp2_fck,			CK_TI814X),
	CLK(NULL,		"mlb_ick",			&mlb_ick,			CK_TI814X),
	CLK(NULL,		"fdif_ick",			&fdif_ick,			CK_TI814X),
	CLK(NULL,		"dap_ick",			&dap_ick,			CK_TI814X),
	CLK(NULL,		"ocmc_ram_ick",			&ocmc_ram_ick,			CK_TI814X),
	CLK("mmci-omap-hs.2",	"ick",				&mmchs2_ick,			CK_TI814X),
	CLK("cpsw.0",		NULL,				&cpsw_ick,			CK_TI814X),
	CLK(NULL,		"dsp_trc_fck",			&dsp_trc_fck,			CK_TI814X),
	CLK(NULL,		"mcasp3_ick",			&mcasp3_ick,			CK_TI814X),
	CLK(NULL,		"mcasp4_ick",			&mcasp4_ick,			CK_TI814X),
	CLK(NULL,		"mcasp5_ick",			&mcasp5_ick,			CK_TI814X),
	CLK(NULL,		"sysclk6_ck",			&sysclk6_ck,			CK_TI814X),
	CLK(NULL,		"l3_slow_ick",			&l3_slow_ick,			CK_TI814X),
	CLK(NULL,		"l4_slow_ick",			&l4_slow_ick,			CK_TI814X),
	CLK(NULL,		"uart1_ick",			&uart1_ick,			CK_TI814X),
	CLK(NULL,		"uart2_ick",			&uart2_ick,			CK_TI814X),
	CLK(NULL,		"uart3_ick",			&uart3_ick,			CK_TI814X),
	CLK(NULL,		"uart4_ick",			&uart4_ick,			CK_TI814X),
	CLK(NULL,		"uart5_ick",			&uart5_ick,			CK_TI814X),
	CLK(NULL,		"uart6_ick",			&uart6_ick,			CK_TI814X),
	CLK("i2c_omap.1",	"ick",				&i2c1_ick,			CK_TI814X),
	CLK("i2c_omap.2",	"ick",				&i2c2_ick,			CK_TI814X),
	CLK("i2c_omap.3",	"ick",				&i2c3_ick,			CK_TI814X),
	CLK("i2c_omap.4",	"ick",				&i2c4_ick,			CK_TI814X),
	CLK("omap2_mcspi.0",	"ick",				&mcspi0_ick,			CK_TI814X),
	CLK("omap2_mcspi.1",	"ick",				&mcspi1_ick,			CK_TI814X),
	CLK("omap2_mcspi.2",	"ick",				&mcspi2_ick,			CK_TI814X),
	CLK("omap2_mcspi.3",	"ick",				&mcspi3_ick,			CK_TI814X),
	CLK(NULL,		"sdio_ick",			&sdio_ick,			CK_TI814X),
	CLK(NULL,		"gpt1_ick",			&gpt1_ick,			CK_TI814X),
	CLK(NULL,		"gpt2_ick",			&gpt2_ick,			CK_TI814X),
	CLK(NULL,		"gpt3_ick",			&gpt3_ick,			CK_TI814X),
	CLK(NULL,		"gpt4_ick",			&gpt4_ick,			CK_TI814X),
	CLK(NULL,		"gpt5_ick",			&gpt5_ick,			CK_TI814X),
	CLK(NULL,		"gpt6_ick",			&gpt6_ick,			CK_TI814X),
	CLK(NULL,		"gpt7_ick",			&gpt7_ick,			CK_TI814X),
	CLK(NULL,		"gpt8_ick",			&gpt8_ick,			CK_TI814X),
	CLK(NULL,		"gpio0_ick",			&gpio0_ick,			CK_TI814X),
	CLK(NULL,		"gpio1_ick",			&gpio1_ick,			CK_TI814X),
	CLK(NULL,		"prcm_ick",			&prcm_ick,			CK_TI814X),
	CLK(NULL,		"smartcard0_ick",		&smartcard0_ick,		CK_TI814X),
	CLK(NULL,		"smartcard1_ick",		&smartcard1_ick,		CK_TI814X),
	CLK(NULL,		"mcasp0_ick",			&mcasp0_ick,			CK_TI814X),
	CLK(NULL,		"mcasp1_ick",			&mcasp1_ick,			CK_TI814X),
	CLK(NULL,		"mcasp2_ick",			&mcasp2_ick,			CK_TI814X),
	CLK(NULL,		"mcbsp_ick",			&mcbsp_ick,			CK_TI814X),
	CLK(NULL,		"gpmc_fck",			&gpmc_fck,			CK_TI814X),
	CLK(NULL,		"hdmi_ick",			&hdmi_ick,			CK_TI814X),
	CLK(NULL,		"mlbp_ick",			&mlbp_ick,			CK_TI814X),
	CLK(NULL,		"wdt0_ick",			&wdt0_ick,			CK_TI814X),
	CLK(NULL,		"wdt1_ick",			&wdt1_ick,			CK_TI814X),
	CLK(NULL,		"sync_timer_ick",		&sync_timer_ick,		CK_TI814X),
	CLK(NULL,		"pata_ick",			&pata_ick,			CK_TI814X),
	CLK(NULL,		"mailbox_ick",			&mailbox_ick,			CK_TI814X),
	CLK(NULL,		"spinbox_ick",			&spinbox_ick,			CK_TI814X),
	CLK(NULL,		"sr0_ick",			&sr0_ick,			CK_TI814X),
	CLK(NULL,		"sr1_ick",			&sr1_ick,			CK_TI814X),
	CLK(NULL,		"sr2_ick",			&sr2_ick,			CK_TI814X),
	CLK(NULL,		"sr3_ick",			&sr3_ick,			CK_TI814X),
	CLK(NULL,		"usb_ick",			&usb_ick,			CK_TI814X),
	CLK(NULL,		"mmu_cfg_ick",			&mmu_cfg_ick,			CK_TI814X),
	CLK(NULL,		"p1500_ick",			&p1500_ick,			CK_TI814X),
	CLK(NULL,		"elm_ick",			&elm_ick,			CK_TI814X),
	CLK("mmci-omap-hs.0",	"ick",				&mmchs0_ick,			CK_TI814X),
	CLK("mmci-omap-hs.1",	"ick",				&mmchs1_ick,			CK_TI814X),
	CLK(NULL,		"atl_ick",			&atl_ick,			CK_TI814X),
	CLK(NULL,		"fdif_cfg_ick",			&fdif_cfg_ick,			CK_TI814X),
	CLK(NULL,		"isp_dpll_ck",			&isp_dpll_ck,			CK_TI814X),
	CLK(NULL,		"isp_iss_ck",			&isp_iss_ck,			CK_TI814X),
	CLK(NULL,		"tppss_tso_ick",		&tppss_tso_ick,			CK_TI814X),
	CLK(NULL,		"securss_mux0_ck",		&securss_mux0_ck,		CK_TI814X),
	CLK(NULL,		"hdvpss_dpll_ck",		&hdvpss_dpll_ck,		CK_TI814X),
	CLK(NULL,		"hdvpss_proc_fck",		&hdvpss_proc_fck,		CK_TI814X),
	CLK(NULL,		"hdvpss_l3_ck",			&hdvpss_l3_ck,			CK_TI814X),
	CLK(NULL,		"hdvpss_proc_d2_fck",		&hdvpss_proc_d2_fck,		CK_TI814X),
	CLK(NULL,		"hdvpss_l4_ck",			&hdvpss_l4_ck,			CK_TI814X),
	CLK(NULL,		"usb_dpll_clkin_ck",		&usb_dpll_clkin_ck,		CK_TI814X),
	CLK(NULL,		"usb_dpll_ck",			&usb_dpll_ck,			CK_TI814X),
	CLK(NULL,		"usb_phy0_rclk_ick",		&usb_phy0_rclk_ick,		CK_TI814X),
	CLK(NULL,		"usb_phy1_rclk_ick",		&usb_phy1_rclk_ick,		CK_TI814X),
	CLK(NULL,		"usb_dpll_div5_ck",		&usb_dpll_div5_ck,		CK_TI814X),
	CLK(NULL,		"sysclk10_ck",			&sysclk10_ck,			CK_TI814X),
	CLK("omap2_mcspi.0",	"fck",				&mcspi0_fck,			CK_TI814X),
	CLK("omap2_mcspi.1",	"fck",				&mcspi1_fck,			CK_TI814X),
	CLK("omap2_mcspi.2",	"fck",				&mcspi2_fck,			CK_TI814X),
	CLK("omap2_mcspi.3",	"fck",				&mcspi3_fck,			CK_TI814X),
	CLK(NULL,		"i2c02_ck",			&i2c02_ck,			CK_TI814X),
	CLK(NULL,		"i2c13_ck",			&i2c13_ck,			CK_TI814X),
	CLK("i2c_omap.1",	"fck",				&i2c1_fck,			CK_TI814X),
	CLK("i2c_omap.2",	"fck",				&i2c2_fck,			CK_TI814X),
	CLK("i2c_omap.3",	"fck",				&i2c3_fck,			CK_TI814X),
	CLK("i2c_omap.4",	"fck",				&i2c4_fck,			CK_TI814X),
	CLK(NULL,		"uart1_fck",			&uart1_fck,			CK_TI814X),
	CLK(NULL,		"uart2_fck",			&uart2_fck,			CK_TI814X),
	CLK(NULL,		"uart3_fck",			&uart3_fck,			CK_TI814X),
	CLK(NULL,		"hdmi_cec_fck",			&hdmi_cec_fck,			CK_TI814X),
	CLK(NULL,		"sysclk8_ck",			&sysclk8_ck,			CK_TI814X),
	CLK("mmci-omap-hs.0",	"fck",				&mmchs0_fck,			CK_TI814X),
	CLK("mmci-omap-hs.1",	"fck",				&mmchs1_fck,			CK_TI814X),
	CLK("mmci-omap-hs.2",	"fck",				&mmchs2_fck,			CK_TI814X),
	CLK(NULL,		"uart456_ck",			&uart456_ck,			CK_TI814X),
	CLK(NULL,		"uart4_fck",			&uart4_fck,			CK_TI814X),
	CLK(NULL,		"uart5_fck",			&uart5_fck,			CK_TI814X),
	CLK(NULL,		"uart6_fck",			&uart6_fck,			CK_TI814X),
	CLK(NULL,		"securess_fck",			&securess_fck,			CK_TI814X),
	CLK(NULL,		"tppss_fck",			&tppss_fck,			CK_TI814X),
	CLK(NULL,		"csi2_phy_fck",			&csi2_phy_fck,			CK_TI814X),
	CLK(NULL,		"ddr_dpll_clkin_ck",		&ddr_dpll_clkin_ck,		CK_TI814X),
	CLK(NULL,		"ddr_dpll_ck",			&ddr_dpll_ck,			CK_TI814X),
	CLK(NULL,		"ddr0_phy_ck",			&ddr0_phy_ck,			CK_TI814X),
	CLK(NULL,		"ddr1_phy_ck",			&ddr1_phy_ck,			CK_TI814X),
	CLK(NULL,		"ddr0_phy_fck",			&ddr0_phy_fck,			CK_TI814X),
	CLK(NULL,		"ddr0_emif_mclk_ck",		&ddr0_emif_mclk_ck,		CK_TI814X),
	CLK(NULL,		"dmm_phy_fck",			&dmm_phy_fck,			CK_TI814X),
	CLK(NULL,		"ddr1_phy_fck",			&ddr1_phy_fck,			CK_TI814X),
	CLK(NULL,		"ddr1_emif_mclk_ck",		&ddr1_emif_mclk_ck,		CK_TI814X),
	CLK(NULL,		"video0_dpll_clkin_ck",		&video0_dpll_clkin_ck,		CK_TI814X),
	CLK(NULL,		"video0_dpll_out_ck",		&video0_dpll_out_ck,		CK_TI814X),
	CLK(NULL,		"video_dpll_clk1_ck",		&video_dpll_clk1_ck,		CK_TI814X),
	CLK(NULL,		"video1_dpll_clkin_ck",		&video1_dpll_clkin_ck,		CK_TI814X),
	CLK(NULL,		"video1_dpll_out_ck",		&video1_dpll_out_ck,		CK_TI814X),
	CLK(NULL,		"video_dpll_clk3_ck",		&video_dpll_clk3_ck,		CK_TI814X),
	CLK(NULL,		"hdmi_dpll_clkin_ck",		&hdmi_dpll_clkin_ck,		CK_TI814X),
	CLK(NULL,		"hdmi_dpll_clk2_ck",		&hdmi_dpll_clk2_ck,		CK_TI814X),
	CLK(NULL,		"video_m_pclk_ck",		&video_m_pclk_ck,		CK_TI814X),
	CLK(NULL,		"hdmi_dpll_muxout_ck",		&hdmi_dpll_muxout_ck,		CK_TI814X),
	CLK(NULL,		"video_dpll_clk2_ck",		&video_dpll_clk2_ck,		CK_TI814X),
	CLK(NULL,		"audio_dpll_clk4_ck",		&audio_dpll_clk4_ck,		CK_TI814X),
	CLK(NULL,		"sysclk16_d1mux_ck",		&sysclk16_d1mux_ck,		CK_TI814X),
	CLK(NULL,		"sysclk16_b3mux_ck",		&sysclk16_b3mux_ck,		CK_TI814X),
	CLK(NULL,		"sysclk14_c1mux_ck",		&sysclk14_c1mux_ck,		CK_TI814X),
	CLK(NULL,		"sysclk16_ck",			&sysclk16_ck,			CK_TI814X),
	CLK(NULL,		"tppss_stc1_fck",		&tppss_stc1_fck,		CK_TI814X),
	CLK(NULL,		"hd_venc_g_ck",			&hd_venc_g_ck,			CK_TI814X),
	CLK(NULL,		"sd_dac_ck",			&sd_dac_ck,			CK_TI814X),
	CLK(NULL,		"avdac_fck",			&avdac_fck,			CK_TI814X),
	CLK(NULL,		"sysclk14_ck",			&sysclk14_ck,			CK_TI814X),
	CLK(NULL,		"tppss_stc0_fck",		&tppss_stc0_fck,		CK_TI814X),
	CLK(NULL,		"audio_dpll_clk2_ck",		&audio_dpll_clk2_ck,		CK_TI814X),
	CLK(NULL,		"audio_dpll_clkin_ck",		&audio_dpll_clkin_ck,		CK_TI814X),
	CLK(NULL,		"audio_dpll_clk3_ck",		&audio_dpll_clk3_ck,		CK_TI814X),
	CLK(NULL,		"audio_dpll_clk5_ck",		&audio_dpll_clk5_ck,		CK_TI814X),
	CLK(NULL,		"audio_dpll_clk1_ck",		&audio_dpll_clk1_ck,		CK_TI814X),
	CLK(NULL,		"audio_prcm_clkin_ck",		&audio_prcm_clkin_ck,		CK_TI814X),
	CLK(NULL,		"sysclk18_ck",			&sysclk18_ck,			CK_TI814X),
	CLK(NULL,		"sysclk19_ck",			&sysclk19_ck,			CK_TI814X),
	CLK(NULL,		"sysclk20_ck",			&sysclk20_ck,			CK_TI814X),
	CLK(NULL,		"sysclk21_ck",			&sysclk21_ck,			CK_TI814X),
	CLK(NULL,		"sysclk22_ck",			&sysclk22_ck,			CK_TI814X),
	CLK(NULL,		"audio_prcm1_out_ck",		&audio_prcm1_out_ck,		CK_TI814X),
	CLK(NULL,		"mcbsp_fck",			&mcbsp_fck,			CK_TI814X),
	CLK("davinci-mcasp.0",	NULL,				&mcasp0_fck,			CK_TI814X),
	CLK("davinci-mcasp.1",	NULL,				&mcasp1_fck,			CK_TI814X),
	CLK("davinci-mcasp.2",	NULL,				&mcasp2_fck,			CK_TI814X),
	CLK(NULL,		"hdmi_i2sm_clkin_ck",		&hdmi_i2sm_clkin_ck,		CK_TI814X),
	CLK(NULL,		"hdmi_i2sm_fck",		&hdmi_i2sm_fck,			CK_TI814X),
	CLK(NULL,		"tppss_tso_fck",		&tppss_tso_fck,			CK_TI814X),
	CLK(NULL,		"atl_fck",			&atl_fck,			CK_TI814X),
	CLK(NULL,		"gpio0_debounce_clk_ck",	&gpio0_debounce_clk_ck,		CK_TI814X),
	CLK(NULL,		"gpio1_debounce_clk_ck",	&gpio1_debounce_clk_ck,		CK_TI814X),
	CLK(NULL,		"rtc_fck",			&rtc_fck,			CK_TI814X),
	CLK(NULL,		"mmchs0_debounce_clk_ck",	&mmchs0_debounce_clk_ck,	CK_TI814X),
	CLK(NULL,		"mmchs1_debounce_clk_ck",	&mmchs1_debounce_clk_ck,	CK_TI814X),
	CLK(NULL,		"mmchs2_debounce_clk_ck",	&mmchs2_debounce_clk_ck,	CK_TI814X),
	CLK(NULL,		"sync_timer_fck",		&sync_timer_fck,		CK_TI814X),
	CLK(NULL,		"bandgaps_fck",			&bandgaps_fck,			CK_TI814X),
	CLK(NULL,		"arm_oper_fck",			&arm_oper_fck,			CK_TI814X),
	CLK(NULL,		"gpt1_fck",			&gpt1_fck,			CK_TI814X),
	CLK(NULL,		"gpt2_fck",			&gpt2_fck,			CK_TI814X),
	CLK(NULL,		"gpt3_fck",			&gpt3_fck,			CK_TI814X),
	CLK(NULL,		"gpt4_fck",			&gpt4_fck,			CK_TI814X),
	CLK(NULL,		"gpt5_fck",			&gpt5_fck,			CK_TI814X),
	CLK(NULL,		"gpt6_fck",			&gpt6_fck,			CK_TI814X),
	CLK(NULL,		"gpt7_fck",			&gpt7_fck,			CK_TI814X),
	CLK(NULL,		"gpt8_fck",			&gpt8_fck,			CK_TI814X),
	CLK(NULL,		"mcasp_auxclk_mux0_ck",		&mcasp_auxclk_mux0_ck,		CK_TI814X),
	CLK("davinci-mcasp.3",	NULL,				&mcasp3_fck,			CK_TI814X),
	CLK("davinci-mcasp.4",	NULL,				&mcasp4_fck,			CK_TI814X),
	CLK("davinci-mcasp.5",	NULL,				&mcasp5_fck,			CK_TI814X),
	CLK(NULL,		"mcasp0_ahclkx_pi_ck",		&mcasp0_ahclkx_pi_ck,		CK_TI814X),
	CLK(NULL,		"mcasp0_ahclkr_pi_ck",		&mcasp0_ahclkr_pi_ck,		CK_TI814X),
	CLK(NULL,		"mcasp1_ahclkx_pi_ck",		&mcasp1_ahclkx_pi_ck,		CK_TI814X),
	CLK(NULL,		"mcasp1_ahclkr_pi_ck",		&mcasp1_ahclkr_pi_ck,		CK_TI814X),
	CLK(NULL,		"mcasp2_ahclkx_pi_ck",		&mcasp2_ahclkx_pi_ck,		CK_TI814X),
	CLK(NULL,		"mcasp3_ahclkx_pi_ck",		&mcasp3_ahclkx_pi_ck,		CK_TI814X),
	CLK(NULL,		"mcasp4_ahclkx_pi_ck",		&mcasp4_ahclkx_pi_ck,		CK_TI814X),
	CLK(NULL,		"mcasp5_ahclkx_pi_ck",		&mcasp5_ahclkx_pi_ck,		CK_TI814X),
	CLK(NULL,		"cpts_rft_clk_ck",		&cpts_rft_clk_ck,		CK_TI814X),
	CLK(NULL,		"ljcb_clkin_serdes_xrefp_ck",	&ljcb_clkin_serdes_xrefp_ck,	CK_TI814X),
	CLK(NULL,		"ljcb_clkin_serdes_xrefn_ck",	&ljcb_clkin_serdes_xrefn_ck,	CK_TI814X),
	CLK(NULL,		"pciess_refp_clk_ck",		&pciess_refp_clk_ck,		CK_TI814X),
	CLK(NULL,		"pciess_refn_clk_ck",		&pciess_refn_clk_ck,		CK_TI814X),
	CLK(NULL,		"pciess_20_refin_ck",		&pciess_20_refin_ck,		CK_TI814X),
	CLK(NULL,		"pciess_50_clk_ck",		&pciess_50_clk_ck,		CK_TI814X),
	CLK(NULL,		"pciess_125_clk_ck",		&pciess_125_clk_ck,		CK_TI814X),
	CLK(NULL,		"satass_refp_clk_ck",		&satass_refp_clk_ck,		CK_TI814X),
	CLK(NULL,		"satass_refn_clk_ck",		&satass_refn_clk_ck,		CK_TI814X),
	CLK(NULL,		"satass_20_refin_ck",		&satass_20_refin_ck,		CK_TI814X),
	CLK(NULL,		"satass_50_clk_ck",		&satass_50_clk_ck,		CK_TI814X),
	CLK(NULL,		"satass_125_clk_ck",		&satass_125_clk_ck,		CK_TI814X),
	CLK(NULL,		"gmac_rmii_ref_ck",		&gmac_rmii_ref_ck,		CK_TI814X),
	CLK(NULL,		"gmac_gmii_ref_ck",		&gmac_gmii_ref_ck,		CK_TI814X),
	CLK(NULL,		"wdt0_fck",			&wdt0_fck,			CK_TI814X),
	CLK(NULL,		"wdt1_fck",			&wdt1_fck,			CK_TI814X),
};

int __init ti814x_clk_init(void)
{
	struct omap_clk *c;
	u32 cpu_clkflg;

	if (cpu_is_ti81xx()) {
		cpu_mask = RATE_IN_TI814X;
		cpu_clkflg = CK_TI814X;
	}

	clk_init(&omap2_clk_functions);

	for (c = ti814x_clks; c < ti814x_clks + ARRAY_SIZE(ti814x_clks); c++)
		clk_preinit(c->lk.clk);

	for (c = ti814x_clks; c < ti814x_clks + ARRAY_SIZE(ti814x_clks); c++) {
		if (c->cpu & cpu_clkflg) {
			clkdev_add(&c->lk);
			clk_register(c->lk.clk);
			omap2_init_clk_clkdm(c->lk.clk);
		}
	}

	recalculate_root_clocks();

	/*
	* Only enable those clocks we will need, let the drivers
	* enable other clocks as necessary
	*/

	clk_enable_init_clocks();

	return 0;
}
