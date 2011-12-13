/*
 * Code for TI8168/TI8148 EVM.
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
#include <linux/init.h>

#ifdef CONFIG_REGULATOR_GPIO
#include <linux/gpio.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/gpio-regulator.h>
#endif

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <plat/irqs.h>
#include <plat/board.h>
#include <plat/mmc.h>
#include <plat/usb.h>
#include "common.h"
#include "mux.h"
#include "hsmmc.h"

static struct omap_board_config_kernel ti81xx_evm_config[] __initdata = {
};

#ifdef CONFIG_OMAP_MUX
static struct omap_board_mux board_mux[] __initdata = {

	/* PIN mux for non-muxed NOR */
	TI816X_MUX(TIM7_OUT, OMAP_MUX_MODE1),	/* gpmc_a12 */
	TI816X_MUX(UART1_CTSN, OMAP_MUX_MODE1),	/* gpmc_a13 */
	TI816X_MUX(UART1_RTSN, OMAP_MUX_MODE1),	/* gpmc_a14 */
	TI816X_MUX(UART2_RTSN, OMAP_MUX_MODE1),	/* gpmc_a15 */
	/* REVISIT: why 2 lines configured as gpmc_a15 */
	TI816X_MUX(SC1_RST, OMAP_MUX_MODE1),	/* gpmc_a15 */
	TI816X_MUX(UART2_CTSN, OMAP_MUX_MODE1),	/* gpmc_a16 */
	TI816X_MUX(UART0_RIN, OMAP_MUX_MODE1),	/* gpmc_a17 */
	TI816X_MUX(UART0_DCDN, OMAP_MUX_MODE1),	/* gpmc_a18 */
	TI816X_MUX(UART0_DSRN, OMAP_MUX_MODE1),	/* gpmc_a19 */
	TI816X_MUX(UART0_DTRN, OMAP_MUX_MODE1),	/* gpmc_a20 */
	TI816X_MUX(SPI_SCS3, OMAP_MUX_MODE1),	/* gpmc_a21 */
	TI816X_MUX(SPI_SCS2, OMAP_MUX_MODE1),	/* gpmc_a22 */
	TI816X_MUX(GP0_IO6, OMAP_MUX_MODE2),	/* gpmc_a23 */
	TI816X_MUX(TIM6_OUT, OMAP_MUX_MODE1),	/* gpmc-a24 */
	TI816X_MUX(SC0_DATA, OMAP_MUX_MODE1),	/* gpmc_a25 */
	/* for controlling high address */
	TI816X_MUX(GPMC_A27, OMAP_MUX_MODE1),	/* gpio-20 */
	{ .reg_offset = OMAP_MUX_TERMINATOR },
};
#else
#define board_mux	NULL
#endif

static struct omap2_hsmmc_info mmc[] = {
	{
		.mmc		= 1,
		.caps		= MMC_CAP_4_BIT_DATA,
		.gpio_cd	= -EINVAL,/* Dedicated pins for CD and WP */
		.gpio_wp	= -EINVAL,
		.ocr_mask	= MMC_VDD_33_34,
	},
	{}	/* Terminator */
};

static struct omap_musb_board_data musb_board_data = {
	.interface_type	= MUSB_INTERFACE_ULPI,
	.mode           = MUSB_HOST,
	.power		= 500,
	.instances	= 1,
};

#ifdef CONFIG_REGULATOR_GPIO
static struct regulator_consumer_supply ti816x_gpio_dcdc_supply[] = {
	{
		.supply = "vdd_avs",
	},
};

static struct regulator_init_data ti816x_gpio_vr_init_data = {
	.constraints = {
		.min_uV		= 800000,
		.max_uV		= 1025000,
		.valid_ops_mask	= (REGULATOR_CHANGE_VOLTAGE |
						   REGULATOR_CHANGE_STATUS),
	},
	.consumer_supplies	= ti816x_gpio_dcdc_supply,
	.num_consumer_supplies	= ARRAY_SIZE(ti816x_gpio_dcdc_supply),
};

/* Supported voltage values for regulators */
static struct gpio_regulator_state ti816x_states_table[] = {
	{ .value = 800000, .gpios = 0x0 },
	{ .value = 815000, .gpios = 0x8 },
	{ .value = 830000, .gpios = 0x4 },
	{ .value = 845000, .gpios = 0xC },
	{ .value = 860000, .gpios = 0x2 },
	{ .value = 875000, .gpios = 0xA },
	{ .value = 890000, .gpios = 0x6 },
	{ .value = 905000, .gpios = 0xE },
	{ .value = 920000, .gpios = 0x1 },
	{ .value = 935000, .gpios = 0x9 },
	{ .value = 950000, .gpios = 0x5 },
	{ .value = 965000, .gpios = 0xD },
	{ .value = 980000, .gpios = 0x3 },
	{ .value = 995000, .gpios = 0xB },
	{ .value = 1010000, .gpios = 0x7},
	{ .value = 1025000, .gpios = 0xF},
};

/* Macro for GPIO voltage regulator */
#define VR_GPIO_INSTANCE	0

static struct gpio ti816x_vcore_gpios[] = {
	{ (VR_GPIO_INSTANCE * 32) + 0, GPIOF_OUT_INIT_HIGH /* GPIOF_IN */, "vgpio 0"},
	{ (VR_GPIO_INSTANCE * 32) + 1, GPIOF_OUT_INIT_HIGH /* GPIOF_IN */, "vgpio 1"},
	{ (VR_GPIO_INSTANCE * 32) + 2, GPIOF_OUT_INIT_HIGH /* GPIOF_IN */, "vgpio 2"},
	{ (VR_GPIO_INSTANCE * 32) + 3, GPIOF_OUT_INIT_HIGH /* GPIOF_IN */, "vgpio 3"},
};

/* GPIO regulator platform data */
static struct gpio_regulator_config ti816x_gpio_vr_config = {
	.supply_name	= "VFB",
	.enable_gpio	= -EINVAL,
	.enable_high	= 1,
	.enabled_at_boot = 1,
	.startup_delay	= 0,
	.gpios			= ti816x_vcore_gpios,
	.nr_gpios		= ARRAY_SIZE(ti816x_vcore_gpios),
	.states			= ti816x_states_table,
	.nr_states		= ARRAY_SIZE(ti816x_states_table),
	.type			= REGULATOR_VOLTAGE,
	.init_data		= &ti816x_gpio_vr_init_data,
};

/* VCORE for SR regulator init */
static struct platform_device ti816x_gpio_vr_device = {
	.name	= "gpio-regulator",
	.id		= -1,
	.dev = {
		.platform_data = &ti816x_gpio_vr_config,
	},
};

static void __init ti816x_gpio_vr_init(void)
{
	if (platform_device_register(&ti816x_gpio_vr_device))
		pr_err("failed to register ti816x_gpio_vr device\n");
	else
		pr_info("registered ti816x_gpio_vr device\n");
}
#else
static inline void ti816x_gpio_vr_init(void) {}
#endif

static void __init ti81xx_evm_init(void)
{
	ti81xx_mux_init(board_mux);
	omap_serial_init();
	omap_sdrc_init(NULL, NULL);
	omap_board_config = ti81xx_evm_config;
	omap_board_config_size = ARRAY_SIZE(ti81xx_evm_config);
	omap2_hsmmc_init(mmc);
	usb_musb_init(&musb_board_data);
#ifdef CONFIG_REGULATOR_GPIO
	if (cpu_is_ti816x()) {
		ti816x_gpio_vr_init();
		regulator_has_full_constraints();
		regulator_use_dummy_regulator();
	}
#endif
}

MACHINE_START(TI8168EVM, "ti8168evm")
	/* Maintainer: Texas Instruments */
	.atag_offset	= 0x100,
	.map_io		= ti81xx_map_io,
	.reserve	= ti81xx_reserve,
	.init_early	= ti81xx_init_early,
	.init_irq	= ti81xx_init_irq,
	.timer		= &omap3_timer,
	.init_machine	= ti81xx_evm_init,
MACHINE_END

MACHINE_START(TI8148EVM, "ti8148evm")
	/* Maintainer: Texas Instruments */
	.atag_offset	= 0x100,
	.map_io		= ti81xx_map_io,
	.init_early	= ti81xx_init_early,
	.init_irq	= ti81xx_init_irq,
	.timer		= &omap3_timer,
	.init_machine	= ti81xx_evm_init,
MACHINE_END
