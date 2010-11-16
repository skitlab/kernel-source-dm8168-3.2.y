/*
 *  OMAP SoC specific OPP wrapper function
 *
 * Copyright (C) 2009 - 2010 Texas Instruments Incorporated.
 *	Nishanth Menon
 * Copyright (C) 2009 - 2010 Deep Root Systems, LLC.
 *	Kevin Hilman
 * Copyright (C) 2010 Nokia Corporation.
 *      Eduardo Valentin
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/err.h>
#include <linux/opp.h>

#include <plat/cpu.h>
#include <plat/omap_device.h>

#include "pm.h"

/**
 * struct omap_opp_def - OMAP OPP Definition
 * @hwmod_name:	Name of the hwmod for this domain
 * @freq:	Frequency in hertz corresponding to this OPP
 * @u_volt:	Nominal voltage in microvolts corresponding to this OPP
 * @enabled:	True/false - is this OPP enabled/disabled by default
 *
 * OMAP SOCs have a standard set of tuples consisting of frequency and voltage
 * pairs that the device will support per voltage domain. This is called
 * Operating Points or OPP. The actual definitions of OMAP Operating Points
 * varies over silicon within the same family of devices. For a specific
 * domain, you can have a set of {frequency, voltage} pairs and this is denoted
 * by an array of omap_opp_def. As the kernel boots and more information is
 * available, a set of these are activated based on the precise nature of
 * device the kernel boots up on. It is interesting to remember that each IP
 * which belongs to a voltage domain may define their own set of OPPs on top
 * of this - but this is handled by the appropriate driver.
 */
struct omap_opp_def {
	char *hwmod_name;

	unsigned long freq;
	unsigned long u_volt;

	bool default_available;
};

/*
 * Initialization wrapper used to define an OPP for OMAP variants.
 */
#define OPP_INITIALIZER(_hwmod_name, _enabled, _freq, _uv)	\
{								\
	.hwmod_name	= _hwmod_name,				\
	.default_available	= _enabled,			\
	.freq		= _freq,				\
	.u_volt		= _uv,					\
}

/* Temp variable to allow multiple calls */
static u8 __initdata omap_table_init;

/**
 * omap_init_opp_table() - Initialize opp table as per the CPU type
 * @opp_def:		opp default list for this silicon
 * @opp_def_size:	number of opp entries for this silicon
 *
 * Register the initial OPP table with the OPP library based on the CPU
 * type.
 */
static int __init omap_init_opp_table(struct omap_opp_def *opp_def,
		u32 opp_def_size)
{
	int i, r;

	if (!opp_def || !opp_def_size) {
		pr_err("%s: invalid params!\n", __func__);
		return -EINVAL;
	}

	/*
	 * Initialize only if not already initialized even if the previous
	 * call failed, because, no reason we'd succeed again.
	 */
	if (omap_table_init)
		return -EEXIST;
	omap_table_init = 1;

	/* Lets now register with OPP library */
	for (i = 0; i < opp_def_size; i++) {
		struct omap_hwmod *oh;
		struct device *dev;

		if (!opp_def->hwmod_name) {
			pr_err("%s: NULL name of omap_hwmod, failing [%d].\n",
				__func__, i);
			return -EINVAL;
		}
		oh = omap_hwmod_lookup(opp_def->hwmod_name);
		if (!oh || !oh->od) {
			pr_warn("%s: no hwmod or odev for %s, [%d] "
				"cannot add OPPs.\n", __func__,
				opp_def->hwmod_name, i);
			return -EINVAL;
		}
		dev = &oh->od->pdev.dev;

		r = opp_add(dev, opp_def->freq, opp_def->u_volt);
		if (r) {
			dev_err(dev, "%s: add OPP %ld failed for %s [%d] "
				"result=%d\n",
			       __func__, opp_def->freq,
			       opp_def->hwmod_name, i, r);
		} else {
			if (!opp_def->default_available)
				r = opp_disable(dev, opp_def->freq);
			if (r)
				dev_err(dev, "%s: disable %ld failed for %s "
					"[%d] result=%d\n",
					__func__, opp_def->freq,
					opp_def->hwmod_name, i, r);
		}
		opp_def++;
	}

	return 0;
}

/* omap3 opps */
#include "opp3xxx_data.c"
/* omap4 opps */
#include "opp4xxx_data.c"
