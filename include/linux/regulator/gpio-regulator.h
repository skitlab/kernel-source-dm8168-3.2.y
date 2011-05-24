/*
 * gpio-regulator.h  --  GPIO Voltage regulation
 *
 * Copyright (C) 2011 Texas Instruments, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#ifndef REGULATOR_GPIO
#define REGULATOR_GPIO

#include <linux/gpio.h>
#include <linux/regulator/machine.h>

/**
 * gpio_vr_data - gpio voltage regulator data
 * @gpio_value:	gpio value of the regulator
 * @uV:		Micro voltage corresponds to each gpio value
 */
struct gpio_vr_data {
	u8	gpio_value;
	s32	uV;
};

/**
 * gpio_reg_platform_data - platform data for gpio regulator
 * @name:		name of the pmic output
 * @pmic_init_data:	regulator platform inititialization data
 * @gpio_vsel_table:	gpio voltage regulator, gpio value and its
 *			coresponding voltage
 * @num_voltages:	number of voltage levels
 * @gpios:		array of GPIOs, needed for voltage control
 * @num_gpio_pins:	number of gpio pins
 * @pmic_vout:		pmic output voltage value
 */
struct gpio_reg_platform_data {
	const	char			*name;
	struct	regulator_init_data	*pmic_init_data;
	struct	gpio_vr_data		*gpio_vsel_table;
	int				num_voltages;
	struct	gpio			*gpios;
	int				num_gpio_pins;
	int				pmic_vout;
};

#endif
