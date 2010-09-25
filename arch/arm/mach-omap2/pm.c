/*
 * pm.c - Common OMAP2+ power management-related code
 *
 * Copyright (C) 2010 Texas Instruments, Inc.
 * Copyright (C) 2010 Nokia Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/err.h>
#include <linux/clk.h>

#include <plat/omap-pm.h>
#include <plat/omap_device.h>
#include <plat/common.h>

#include <plat/powerdomain.h>
#include <plat/clockdomain.h>
#include <plat/voltage.h>

#include "pm.h"

static struct omap_device_pm_latency *pm_lats;

static struct device *mpu_dev;
static struct device *iva_dev;
static struct device *l3_dev;
static struct device *dsp_dev;

struct device *omap2_get_mpuss_device(void)
{
	WARN_ON_ONCE(!mpu_dev);
	return mpu_dev;
}

struct device *omap2_get_iva_device(void)
{
	WARN_ON_ONCE(!iva_dev);
	return iva_dev;
}

struct device *omap2_get_l3_device(void)
{
	WARN_ON_ONCE(!l3_dev);
	return l3_dev;
}

struct device *omap4_get_dsp_device(void)
{
	WARN_ON_ONCE(!dsp_dev);
	return dsp_dev;
}
EXPORT_SYMBOL(omap4_get_dsp_device);

/* static int _init_omap_device(struct omap_hwmod *oh, void *user) */
static int _init_omap_device(char *name, struct device **new_dev)
{
	struct omap_hwmod *oh;
	struct omap_device *od;

	oh = omap_hwmod_lookup(name);
	if (WARN(!oh, "%s: could not find omap_hwmod for %s\n",
		 __func__, name))
		return -ENODEV;

	od = omap_device_build(oh->name, 0, oh, NULL, 0, pm_lats, 0, false);
	if (WARN(IS_ERR(od), "%s: could not build omap_device for %s\n",
		 __func__, name))
		return -ENODEV;

	*new_dev = &od->pdev.dev;

	return 0;
}

/*
 * Build omap_devices for processors and bus.
 */
static void omap2_init_processor_devices(void)
{
	_init_omap_device("mpu", &mpu_dev);
	_init_omap_device("iva", &iva_dev);
	if (cpu_is_omap44xx()) {
		_init_omap_device("l3_main_1", &l3_dev);
		_init_omap_device("dsp", &dsp_dev);
	} else {
		_init_omap_device("l3_main", &l3_dev);
	}
}

/*
 * This sets pwrdm state (other than mpu & core. Currently only ON &
 * RET are supported. Function is assuming that clkdm doesn't have
 * hw_sup mode enabled.
 */
int omap_set_pwrdm_state(struct powerdomain *pwrdm, u32 state)
{
	u32 cur_state;
	int sleep_switch = 0;
	int ret = 0;

	if (pwrdm == NULL || IS_ERR(pwrdm))
		return -EINVAL;

	while (!(pwrdm->pwrsts & (1 << state))) {
		if (state == PWRDM_POWER_OFF)
			return ret;
		state--;
	}

	cur_state = pwrdm_read_next_pwrst(pwrdm);
	if (cur_state == state)
		return ret;

	if (pwrdm_read_pwrst(pwrdm) < PWRDM_POWER_ON) {
		omap2_clkdm_wakeup(pwrdm->pwrdm_clkdms[0]);
		sleep_switch = 1;
		pwrdm_wait_transition(pwrdm);
	}

	ret = pwrdm_set_next_pwrst(pwrdm, state);
	if (ret) {
		printk(KERN_ERR "Unable to set state of powerdomain: %s\n",
		       pwrdm->name);
		goto err;
	}

	if (sleep_switch) {
		omap2_clkdm_allow_idle(pwrdm->pwrdm_clkdms[0]);
		pwrdm_wait_transition(pwrdm);
		pwrdm_state_switch(pwrdm);
	}

err:
	return ret;
}

/*
 * This is to be called during init to put the various voltage
 * domains to the voltage as per the opp table. Typically we boot up
 * at the nominal voltage. So this function finds out the rate of
 * the clock associated with the voltage domain, finds out the correct
 * opp entry and puts the voltage domain to the voltage specifies
 * in the opp entry
 */
static int __init omap2_set_init_voltage(char *vdd_name, char *clk_name,
						struct device *dev)
{
	struct voltagedomain *voltdm;
	struct clk *_clk;
	struct omap_opp *opp;
	unsigned long freq, bootup_volt;

	if (!vdd_name || !clk_name || !dev) {
		printk(KERN_ERR "%s: Invalid parameters!\n", __func__);
		goto exit;
	}

	voltdm = omap_voltage_domain_get(vdd_name);
	if (IS_ERR(voltdm)) {
		printk(KERN_ERR "%s: Unable to get vdd pointer for vdd_%s\n",
			__func__, vdd_name);
		goto exit;
	}

	_clk =  clk_get(NULL, clk_name);
	if (IS_ERR(_clk)) {
		printk(KERN_ERR "%s: unable to get clk %s\n",
			__func__, clk_name);
		goto exit;
	}

	freq = _clk->rate;
	opp = opp_find_freq_ceil(dev, &freq);
	if (IS_ERR(opp)) {
		printk(KERN_ERR "%s: unable to find boot up OPP for vdd_%s\n",
			__func__, vdd_name);
		goto exit;
	}

	bootup_volt = opp_get_voltage(opp);
	if (!bootup_volt) {
		printk(KERN_ERR "%s: unable to find voltage corresponding"
			"to the bootup OPP for vdd_%s\n", __func__, vdd_name);
		goto exit;
	}

	omap_voltage_scale_vdd(voltdm, bootup_volt);

	return 0;

exit:
	printk(KERN_ERR "%s: Unable to put vdd_%s to its init voltage\n\n",
		__func__, vdd_name);
	return -EINVAL;
}

static int __init omap2_common_pm_init(void)
{
	omap2_init_processor_devices();

	if (cpu_is_omap34xx()) {
		omap3_pm_init_opp_table();
		omap2_set_init_voltage("mpu", "dpll1_ck", mpu_dev);
		omap2_set_init_voltage("core", "l3_ick", l3_dev);
	} else if (cpu_is_omap44xx()) {
		omap4_pm_init_opp_table();
		omap2_set_init_voltage("mpu", "dpll_mpu_ck", mpu_dev);
		omap2_set_init_voltage("core", "l3_div_ck", l3_dev);
		omap2_set_init_voltage("iva", "dpll_iva_m5x2_ck", iva_dev);
	}

	omap_pm_if_init();

	return 0;
}
device_initcall(omap2_common_pm_init);

