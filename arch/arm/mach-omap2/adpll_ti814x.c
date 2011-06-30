/*
 * TI816X FAPLL clock functions
 *
 * Copyright (C) 2011 Texas Instruments, Inc. - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/string.h>

#include <asm/div64.h>
#include <asm/io.h>
#include <plat/clock.h>

#include "clock.h"
#include "cm-regbits-81xx.h"

/* Macros */
/* Internal ref clock frequency range in (Hz) */
/* Modena PLL */
#define ADPLLS_FINT_BAND_MIN		32000
#define ADPLLS_FINT_BAND_MAX		52000000

/* ADPLLLJx */
#define ADPLLJ_FINT_BAND_MIN		500000
#define ADPLLJ_FINT_BAND_MAX		2500000

/* _dpll_test_fint() return codes */
#define ADPLL_FINT_UNDERFLOW		-1
#define ADPLL_FINT_INVALID		-2

/* Maximum multiplier & divider values for ADPLL */
/* Modena PLL */
#define TI814X_ADPLLS_MAX_MULT		2047
#define TI814X_ADPLLS_MAX_DIV		127

/* ADPLLLJx */
#define TI814X_ADPLLJ_MAX_MULT		4095
#define TI814X_ADPLLJ_MAX_DIV		255

#define TI814X_ADPLL_MIN_DIV		0
#define TI814X_ADPLL_MIN_MULT		2

/* Number of times to check the status register for a state change */
#define MAX_DPLL_WAIT_TRIES		100000

/* To prevent overflow during calculation */
#define ADPLL_ROUNDING_DIVIDER		10000

/* Private functions */

/* _ti814x_wait_dpll_status: wait for a DPLL to enter a specific state
 * @clk - Pointer to ADPLL clk structure
 * @state - State to enter
 */
static int _ti814x_wait_dpll_status(struct clk *clk, u32 state)
{
	const struct dpll_data *dd;
	int i = 0;
	int ret = -EINVAL;

	dd = clk->dpll_data;

	state <<= __ffs(dd->idlest_mask);

	while (((__raw_readl(dd->idlest_reg) & dd->idlest_mask) != state) &&
		i < MAX_DPLL_WAIT_TRIES) {
		i++;
		udelay(1);
	}
	if (i == MAX_DPLL_WAIT_TRIES) {
		pr_debug("clock: %s failed transition to '%s'\n",
			clk->name,
			(state == ST_ADPLL_BYPASSED) ? "bypassed" : "locked");
	} else {
		pr_debug("clock: %s transitioned to '%s'\n",
			clk->name,
			(state == ST_ADPLL_BYPASSED) ? "bypassed" : "locked");
		ret = 0;
	}
	return ret;
}

/*
 * _ti814x_dpll_bypass - instruct a DPLL to bypass and wait for readiness
 * @clk: pointer to an ADPLL struct clk
 *
 * Instructs a ADPLL to enter low-power bypass mode.
 * return -EINVAL.
 */
static int _ti814x_dpll_bypass(struct clk *clk)
{
	const struct dpll_data *dd = clk->dpll_data;
	u32 v;
	int r;

	if (!(dd->modes & (1 << ADPLL_LOW_POWER_BYPASS)))
		return -EINVAL;

	pr_debug("clock: configuring DPLL %s for low-power bypass\n",
			clk->name);

	/* Instruct ADPLL to enter Bypass mode 1 - bypass 0 - Active&Locked */
	v = __raw_readl(dd->control_reg);
	v |= (1 << dd->bypass_bit);
	__raw_writel(v, dd->control_reg);

	v = __raw_readl(dd->control_reg);
	v |= (1 << dd->soft_reset_bit);
	__raw_writel(v, dd->control_reg);

	r = _ti814x_wait_dpll_status(clk, ST_ADPLL_BYPASSED);

	return r;
}

/*
 * _ti814x_rel_dpll_bypass - instruct an ADPLL to exit from bypass
 * @clk: pointer to an ADPLL struct clk
 *
 * Instructs a ADPLL to enter low-power bypass mode.
 * return -EINVAL.
 */
static int _ti814x_rel_dpll_bypass(struct clk *clk)
{
	u32 v;
	const struct dpll_data *dd;
	dd = clk->dpll_data;

	/* Instruct ADPLL to exit Bypass mode 1 - bypass 0 - Active&Locked */
	v = __raw_readl(dd->control_reg);
	v &= ~(1 << dd->bypass_bit);
	__raw_writel(v, dd->control_reg);

	return 0;
}

/*
 * _ti814x_dpll_lock - instruct an ADPLL to lock and wait for readiness
 * @clk: pointer to an ADPLL struct clk
 *
 * Instructs an  ADPLL to lock.  Waits for the ADPLL to report
 * readiness before returning.
 */
static int _ti814x_dpll_lock(struct clk *clk)
{
	struct dpll_data *dd = clk->dpll_data;
	int r;

	pr_debug("clock: locking DPLL %s\n", clk->name);
	if (!(dd->modes & (1 << ADPLL_LOCKED)))
		return -EINVAL;

	_ti814x_rel_dpll_bypass(clk);

	r = _ti814x_wait_dpll_status(clk, ST_ADPLL_LOCKED);

	return r;
}

/* _ti814x_wait_dpll_ret_ack: wait for an ADPLL to acknowledge ret request
 * @clk - pointer to ADPLL struct clk
 *
 * Checks the SYSBYRETACK bit in ADPLL status register to be set
 * return -EINVAL
 */
static int _ti814x_wait_dpll_ret_ack(struct clk *clk)
{
	const struct dpll_data *dd = clk->dpll_data;
	int i = -1, v;
	int ret = -EINVAL;

	do {
		udelay(1);
		i++;
		v = (__raw_readl(dd->idlest_reg) &
			TI814X_ADPLL_STBYRET_ACK_MASK);
		v >>= __ffs(TI814X_ADPLL_STBYRET_ACK_MASK);
	} while ((v != 0x1) && i < MAX_DPLL_WAIT_TRIES);

	if (i == MAX_DPLL_WAIT_TRIES) {
		pr_debug("clock: %s failed to gate internal clocks\n",
			clk->name);
	} else {
		pr_debug("clock: %s internal clocks gated in %d loops\n",
			clk->name, i);
		ret = 0;
	}
	return ret;
}

/*
 * _ti814_dpll_stop - instruct a DPLL to stop
 * @clk: pointer to a DPLL struct clk
 *
 * Instructs an ADPLL to enter low-power stop mode.
 */
static int _ti814_dpll_stop(struct clk *clk)
{
	struct dpll_data *dd = clk->dpll_data;
	u32 v;
	int r = 0;

	if (!(dd->modes & (1 << ADPLL_LOW_POWER_STOP)))
		return -EINVAL;

	pr_debug("clock: stopping DPLL %s\n", clk->name);

	/* DPLL must be in retention before entering stop mode */
	v = __raw_readl(dd->control_reg);
	v |= (1 << dd->stby_ret_bit);
	__raw_writel(v, dd->control_reg);


	/* Read and wait for retention ack from PLL_STATUS register */
	r = _ti814x_wait_dpll_ret_ack(clk);
	if (r) {
		pr_debug("clock: %s DPLL could not be placed in retention\n",
			clk->name);

		/* Return from retention as Ack not received */
		v = __raw_readl(dd->control_reg);
		v &= ~(1 << dd->stby_ret_bit);
		__raw_writel(v, dd->control_reg);
		return -EINVAL;
	}
	if (dd->flags & TI814X_ADPLL_LS_TYPE) {

		/* Enter DPLL stop mode */
		v = __raw_readl(dd->control_reg);
		v |= (1 << dd->stop_mode_bit);
		__raw_writel(v, dd->control_reg);
	}
	return 0;
}

/* Public functions */

/*
 * ti814x_init_dpll_parent - Initialise an ADPLL's parent
 * @clk - pointer to an ADPLL clk structure
 *
 * Clock needs to be in bypass mode before changing the parent
 */
void ti814x_init_dpll_parent(struct clk *clk)
{
	u32 v;
	struct dpll_data *dd = clk->dpll_data;

	if (!dd)
		return;
	/* Check dpll status */
	v = __raw_readl(dd->control_reg);
	v &= (1 << dd->bypass_bit);
	v >>= dd->bypass_bit;

	/* Reparent in case the dpll is in bypass */
	if (v == 1)
		clk_reparent(clk, dd->clk_bypass);
	return;
}

/**
 * ti814x_get_dpll_rate - returns the current ADPLL CLKOUT rate
 * @clk: pointer to  an ADPLL clk structure
 *
 * ADPLLs can be locked or bypassed - basically, enabled or disabled.
 * Read the multiplier an ddivider values from registers to compute rate
 */
u32 ti814x_get_dpll_rate(struct clk *clk)
{
	unsigned long long dpll_rate;
	unsigned long long num, den;
	u32 m, frac_m, n, m2, v, ctrl;
	struct dpll_data *dd = clk->dpll_data;

	if (!dd)
		return 0;

	/* Return bypass rate if DPLL is bypassed or stbyret/stopmode */
	ctrl = __raw_readl(dd->control_reg);
	v = (ctrl & (1 << dd->bypass_bit)) | (ctrl & (1 << dd->stby_ret_bit));

	if (v)
		return dd->clk_bypass->rate;

	v = __raw_readl(dd->mult_div1_reg);
	m = v & dd->mult_mask;
	m >>= __ffs(dd->mult_mask);

	v = __raw_readl(dd->frac_mult_reg);
	frac_m = v & dd->frac_mult_mask;
	frac_m >>= __ffs(dd->frac_mult_mask);

	num = (unsigned long long)((m << 18) + frac_m);

	v = __raw_readl(dd->div_m2n_reg);
	n = v & dd->div_n_mask;
	n >>= __ffs(dd->div_n_mask);
	m2 = v & dd->div_m2_mask;
	m2 >>= __ffs(dd->div_m2_mask);

	den = (unsigned long long)(((n+1) << 18)*m2);
	dpll_rate = (unsigned long long)(dd->clk_ref->rate * num);
	do_div(dpll_rate, den);

	return dpll_rate;
}

/**
 * ti814x_dpll_recalc - recalculate ADPLL rate
 * @clk: ADPLL struct clk
 *
 * Recalculate and propagate the FAPLL rate.
 */
unsigned long ti814x_dpll_recalc(struct clk *clk)
{
	return ti814x_get_dpll_rate(clk);

}

/*
 * ti814x_dpll_enable - Enable an ADPLL
 * @clk - pointer to struct clk of ADPLL
 */
int ti814x_dpll_enable(struct clk *clk)
{
	int r;
	struct dpll_data *dd;
	dd = clk->dpll_data;
	if (!dd)
		return -EINVAL;

	if (clk->rate == dd->clk_bypass->rate) {

		WARN_ON(clk->parent != dd->clk_bypass);
		r = _ti814x_dpll_bypass(clk);
	} else {
		WARN_ON(clk->parent != dd->clk_ref);
		r = _ti814x_dpll_lock(clk);
	}
	if (!r)
		clk->rate = ti814x_get_dpll_rate(clk);
	return r;
}

/**
 * ti814x_dpll_disable - Disable an ADPLL( enter low-power stop)
 * @clk: pointer to a DPLL struct clk
 *
 * Instructs an ADPLL to enter low-power stop.  This function is
 * intended for use in struct clkops.  No return value.
 */
void ti814x_dpll_disable(struct clk *clk)
{

	_ti814_dpll_stop(clk);
}

