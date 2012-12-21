/*
 * Code for TI8168 EVM.
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

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <plat/irqs.h>
#include <plat/board.h>
#include <plat/common.h>

#include "mux.h"

static struct omap_board_config_kernel ti8168_evm_config[] __initdata = {
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

static void __init ti8168_evm_init(void)
{
	ti81xx_mux_init(board_mux);
	omap_serial_init();
	omap_sdrc_init(NULL, NULL);
	omap_board_config = ti8168_evm_config;
	omap_board_config_size = ARRAY_SIZE(ti8168_evm_config);
}

static void __init ti8168_evm_map_io(void)
{
	omapti816x_map_common_io();
}

MACHINE_START(TI8168EVM, "ti8168evm")
	/* Maintainer: Texas Instruments */
	.atag_offset	= 0x100,
	.map_io		= ti8168_evm_map_io,
	.init_early	= ti816x_init_early,
	.init_irq	= ti816x_init_irq,
	.timer		= &omap3_timer,
	.init_machine	= ti8168_evm_init,
MACHINE_END
