/*
 * OMAP4 OPP table definitions.
 *
 * Copyright (C) 2009 - 2010 Texas Instruments Incorporated.
 *	Nishanth Menon
 * Copyright (C) 2009 - 2010 Deep Root Systems, LLC.
 *	Kevin Hilman
 * Copyright (C) 2010 Nokia Corporation.
 *      Eduardo Valentin
 * Copyright (C) 2010 Texas Instruments Incorporated.
 *	Thara Gopinath
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

static struct omap_opp_def __initdata omap44xx_opp_def_list[] = {
	/* MPU OPP1 - OPP50 */
	OPP_INITIALIZER("mpu", true, 300000000, 1100000),
	/* MPU OPP2 - OPP100 */
	OPP_INITIALIZER("mpu", true, 600000000, 1200000),
	/* MPU OPP3 - OPP-Turbo */
	OPP_INITIALIZER("mpu", false, 800000000, 1260000),
	/* MPU OPP4 - OPP-SB */
	OPP_INITIALIZER("mpu", false, 1008000000, 1350000),
	/* L3 OPP1 - OPP50 */
	OPP_INITIALIZER("l3_main_1", true, 100000000, 930000),
	/* L3 OPP2 - OPP100, OPP-Turbo, OPP-SB */
	OPP_INITIALIZER("l3_main_1", true, 200000000, 1100000),
	/* TODO: add IVA, DSP, aess, fdif, gpu */
};

/**
 * omap4_opp_init() - initialize omap4 opp table
 */
int __init omap4_opp_init(void)
{
	int r = -ENODEV;

	if (!cpu_is_omap44xx())
		return r;

	r = omap_init_opp_table(omap44xx_opp_def_list,
			ARRAY_SIZE(omap44xx_opp_def_list));

	return r;
}
device_initcall(omap4_opp_init);
