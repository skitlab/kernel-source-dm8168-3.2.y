/*
 * Smart reflex Class 3 specific implementations
 *
 * Author: Thara Gopinath       <thara@ti.com>
 *
 * Copyright (C) 2010 Texas Instruments, Inc.
 * Thara Gopinath <thara@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <plat/smartreflex.h>

#include "smartreflex-class3.h"
#include "voltage.h"

static int sr_class3_enable(int id)
{
	unsigned long volt = 0;

	volt = get_curr_voltage(id);
	if (!volt) {
		pr_warning("%s: Current voltage unknown.Cannot enable SR%d\n",
				__func__, id);
		return -ENODATA;
	}

	omap_voltageprocessor_enable(id);
	return sr_enable(id, volt);
}

static int sr_class3_disable(int id, int is_volt_reset)
{
	omap_voltageprocessor_disable(id);
	sr_disable(id);
	if (is_volt_reset)
		omap_reset_voltage(id);

	return 0;
}

static int sr_class3_configure(int id)
{
	return sr_configure_errgen(id);
}

/* SR class3 structure */
struct omap_smartreflex_class_data class3_data = {
	.enable = sr_class3_enable,
	.disable = sr_class3_disable,
	.configure = sr_class3_configure,
	.class_type = SR_CLASS3,
};

/* Smartreflex CLASS3 init API to be called from board file */
int __init sr_class3_init(void)
{
	pr_info("SmartReflex CLASS3 initialized\n");
	return omap_sr_register_class(&class3_data);
}
