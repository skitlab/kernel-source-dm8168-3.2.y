/*
 * omap_hwmod_ti81xx_data.c - hardware modules data for TI81XX chips
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
 *
 */

#include <linux/gpio.h>

#include <plat/omap_hwmod.h>
#include <mach/irqs.h>
#include <plat/cpu.h>
#include <plat/dma.h>
#include <plat/serial.h>
#include <plat/l4_3xxx.h>
#include <plat/dmtimer.h>
#include <plat/mmc.h>
#include <plat/ti81xx.h>

#include "omap_hwmod_common_data.h"

#include "control.h"
#include "cm81xx.h"
#include "cm-regbits-81xx.h"

/*
 * TI816X hardware modules integration data
 *
 * Note: This is incomplete and at present, not generated from h/w database.
 *
 * TODO: Add EDMA in the 'user' field wherever applicable.
 */

static struct omap_hwmod ti816x_mpu_hwmod;
static struct omap_hwmod ti816x_l3_slow_hwmod;
static struct omap_hwmod ti816x_l4_slow_hwmod;

/* L3 SLOW -> L4_SLOW Peripheral interface */
static struct omap_hwmod_ocp_if ti816x_l3_slow__l4_slow = {
	.master	= &ti816x_l3_slow_hwmod,
	.slave	= &ti816x_l4_slow_hwmod,
	.user	= OCP_USER_MPU,
};

/* MPU -> L3 SLOW interface */
static struct omap_hwmod_ocp_if ti816x_mpu__l3_slow = {
	.master = &ti816x_mpu_hwmod,
	.slave	= &ti816x_l3_slow_hwmod,
	.user	= OCP_USER_MPU,
};

/* Slave interfaces on the L3 SLOW interconnect */
static struct omap_hwmod_ocp_if *ti816x_l3_slow_slaves[] = {
	&ti816x_mpu__l3_slow,
};

/* Master interfaces on the L3 SLOW interconnect */
static struct omap_hwmod_ocp_if *ti816x_l3_slow_masters[] = {
	&ti816x_l3_slow__l4_slow,
};

/* L3 SLOW */
static struct omap_hwmod ti816x_l3_slow_hwmod = {
	.name		= "l3_main",
	.class		= &l3_hwmod_class,
	.masters	= ti816x_l3_slow_masters,
	.masters_cnt	= ARRAY_SIZE(ti816x_l3_slow_masters),
	.slaves		= ti816x_l3_slow_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_l3_slow_slaves),
	.flags		= HWMOD_NO_IDLEST,
};

static struct omap_hwmod ti816x_uart1_hwmod;
static struct omap_hwmod ti816x_uart2_hwmod;
static struct omap_hwmod ti816x_uart3_hwmod;
static struct omap_hwmod ti816x_wd_timer2_hwmod;
static struct omap_hwmod ti81xx_i2c1_hwmod;
static struct omap_hwmod ti816x_i2c2_hwmod;
static struct omap_hwmod ti81xx_gpio1_hwmod;
static struct omap_hwmod ti81xx_gpio2_hwmod;
static struct omap_hwmod ti81xx_usbss_hwmod;
static struct omap_hwmod ti81xx_elm_hwmod;
static struct omap_hwmod ti816x_mmc1_hwmod;
static struct omap_hwmod ti816x_iva_hwmod;

static struct omap_hwmod_ocp_if ti816x_l4_slow__uart1;
static struct omap_hwmod_ocp_if ti816x_l4_slow__uart2;
static struct omap_hwmod_ocp_if ti816x_l4_slow__uart3;
static struct omap_hwmod_ocp_if ti816x_l4_slow__wd_timer2;
static struct omap_hwmod_ocp_if ti816x_l4_slow__i2c1;
static struct omap_hwmod_ocp_if ti816x_l4_slow__i2c2;
static struct omap_hwmod_ocp_if ti81xx_l4_slow__gpio1;
static struct omap_hwmod_ocp_if ti81xx_l4_slow__gpio2;
static struct omap_hwmod_ocp_if ti81xx_l4_slow__elm;
static struct omap_hwmod_ocp_if ti816x_l4_slow__mmc1;
static struct omap_hwmod_ocp_if ti816x_l3__iva;

/* Slave interfaces on the L4_SLOW interconnect */
static struct omap_hwmod_ocp_if *ti816x_l4_slow_slaves[] = {
	&ti816x_l3_slow__l4_slow,
};

/* Master interfaces on the L4_SLOW interconnect */
static struct omap_hwmod_ocp_if *ti816x_l4_slow_masters[] = {
	&ti816x_l4_slow__uart1,
	&ti816x_l4_slow__uart2,
	&ti816x_l4_slow__uart3,
	&ti816x_l4_slow__wd_timer2,
	&ti816x_l4_slow__i2c1,
	&ti816x_l4_slow__i2c2,
	&ti81xx_l4_slow__gpio1,
	&ti81xx_l4_slow__gpio2,
	&ti81xx_l4_slow__elm,
	&ti816x_l4_slow__mmc1,
};

/* L4 SLOW */
static struct omap_hwmod ti816x_l4_slow_hwmod = {
	.name		= "l4_slow",
	.class		= &l4_hwmod_class,
	.masters	= ti816x_l4_slow_masters,
	.masters_cnt	= ARRAY_SIZE(ti816x_l4_slow_masters),
	.slaves		= ti816x_l4_slow_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_l4_slow_slaves),
	.flags		= HWMOD_NO_IDLEST,
};

/* Master interfaces on the MPU device */
static struct omap_hwmod_ocp_if *ti816x_mpu_masters[] = {
	&ti816x_mpu__l3_slow,
};

/* MPU */
static struct omap_hwmod ti816x_mpu_hwmod = {
	.name		= "mpu",
	.class		= &mpu_hwmod_class,
	/* .main_clk	= "mpu_ck", */
	.main_clk	= "arm_fck",
	.vdd_name	= "mpu",
	.masters	= ti816x_mpu_masters,
	.masters_cnt	= ARRAY_SIZE(ti816x_mpu_masters),
};

/* UART common */

static struct omap_hwmod_class_sysconfig uart_sysc = {
	.rev_offs	= 0x50,
	.sysc_offs	= 0x54,
	.syss_offs	= 0x58,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE |
				   SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
				   SYSC_HAS_AUTOIDLE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class uart_class = {
	.name = "uart",
	.sysc = &uart_sysc,
};

/* UART1 */

/* L4 SLOW -> UART1 interface */
static struct omap_hwmod_addr_space ti816x_uart1_addr_space[] = {
	{
		.pa_start	= TI81XX_UART1_BASE,
		.pa_end		= TI81XX_UART1_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__uart1 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_uart1_hwmod,
	.clk		= "uart1_ick",
	.addr		= ti816x_uart1_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_irq_info uart1_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_UART0, },
	{ .irq = -1 }
};

/*
 * There is no SDMA on TI81XX, instead we have EDMA. Presently using dummy
 * channel numbers as the omap UART driver (drivers/serial/omap-serial.c)
 * requires these values to be filled in even if we don't have DMA enabled. Same
 * applies for other UARTs below.
 */
static struct omap_hwmod_dma_info uart1_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_ocp_if *ti816x_uart1_slaves[] = {
	&ti816x_l4_slow__uart1,
};

static struct omap_hwmod ti816x_uart1_hwmod = {
	.name		= "uart1",
	.mpu_irqs	= uart1_mpu_irqs,
	.sdma_reqs	= uart1_edma_reqs,
	.main_clk	= "uart1_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI81XX_CM_ALWON_UART_0_CLKCTRL_OFF,
		},
	},
	.slaves		= ti816x_uart1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_uart1_slaves),
	.class		= &uart_class,
};

/* UART2 */

/* L4 SLOW -> UART2 interface */
static struct omap_hwmod_addr_space ti816x_uart2_addr_space[] = {
	{
		.pa_start	= TI81XX_UART2_BASE,
		.pa_end		= TI81XX_UART2_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__uart2 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_uart2_hwmod,
	.clk		= "uart2_ick",
	.addr		= ti816x_uart2_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_irq_info uart2_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_UART1, },
	{ .irq = -1 }
};

static struct omap_hwmod_dma_info uart2_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_ocp_if *ti816x_uart2_slaves[] = {
	&ti816x_l4_slow__uart2,
};

static struct omap_hwmod ti816x_uart2_hwmod = {
	.name		= "uart2",
	.mpu_irqs	= uart2_mpu_irqs,
	.sdma_reqs	= uart2_edma_reqs,
	.main_clk	= "uart2_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI81XX_CM_ALWON_UART_1_CLKCTRL_OFF,
		},
	},
	.slaves		= ti816x_uart2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_uart2_slaves),
	.class		= &uart_class,
};

/* UART3 */

/* L4 SLOW -> UART3 interface */
static struct omap_hwmod_addr_space ti816x_uart3_addr_space[] = {
	{
		.pa_start	= TI81XX_UART3_BASE,
		.pa_end		= TI81XX_UART3_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__uart3 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_uart3_hwmod,
	.clk		= "uart3_ick",
	.addr		= ti816x_uart3_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_irq_info uart3_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_UART2, },
	{ .irq = -1 }
};

static struct omap_hwmod_dma_info uart3_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_ocp_if *ti816x_uart3_slaves[] = {
	&ti816x_l4_slow__uart3,
};

static struct omap_hwmod ti816x_uart3_hwmod = {
	.name		= "uart3",
	.mpu_irqs	= uart3_mpu_irqs,
	.sdma_reqs	= uart3_edma_reqs,
	.main_clk	= "uart3_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI81XX_CM_ALWON_UART_2_CLKCTRL_OFF,
		},
	},
	.slaves		= ti816x_uart3_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_uart3_slaves),
	.class		= &uart_class,
};

/* Watchdog */

/*
 * 'wd_timer' class
 * 32-bit watchdog upward counter that generates a pulse on the reset pin on
 * overflow condition
 */

static struct omap_hwmod_class_sysconfig wd_timer_sysc = {
	.rev_offs       = 0x0000,
	.sysc_offs      = 0x0010,
	.syss_offs      = 0x0014,
	.sysc_flags     = (SYSC_HAS_SIDLEMODE | SYSC_HAS_EMUFREE |
					   SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
					   SYSC_HAS_AUTOIDLE | SYSC_HAS_CLOCKACTIVITY),
	.idlemodes      = (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class wd_timer_class = {
	.name = "wd_timer",
	.sysc = &wd_timer_sysc,
};

/* L4 SLOW -> Watchdog */
static struct omap_hwmod_addr_space ti816x_wd_timer2_addrs[] = {
	{
		.pa_start	= TI816X_WDTIMER1_BASE,
		.pa_end		= TI816X_WDTIMER1_BASE + SZ_4K - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__wd_timer2 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_wd_timer2_hwmod,
	.clk		= "wdt2_ick",
	.addr		= ti816x_wd_timer2_addrs,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti816x_wd_timer2_slaves[] = {
	&ti816x_l4_slow__wd_timer2,
};

static struct omap_hwmod ti816x_wd_timer2_hwmod = {
	.name		= "wd_timer2",
	.main_clk	= "wdt2_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI81XX_CM_ALWON_WDTIMER_CLKCTRL_OFF,
		},
	},
	.slaves		= ti816x_wd_timer2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_wd_timer2_slaves),
	.class		= &wd_timer_class,
	.flags	  = HWMOD_INIT_NO_RESET,
};

/* I2C common */
static struct omap_hwmod_class_sysconfig i2c_sysc = {
	.rev_offs	= 0x0,
	.sysc_offs	= 0x10,
	.syss_offs	= 0x90,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE |
				   SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
				   SYSC_HAS_AUTOIDLE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class i2c_class = {
	.name = "i2c",
	.sysc = &i2c_sysc,
};

/* I2C1 */

/* L4 SLOW -> I2C1 */
static struct omap_hwmod_addr_space ti816x_i2c1_addr_space[] = {
	{
		.pa_start	= TI816X_I2C0_BASE,
		.pa_end		= TI816X_I2C0_BASE + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__i2c1 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti81xx_i2c1_hwmod,
	.clk		= "i2c1_ick",
	.addr		= ti816x_i2c1_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_irq_info i2c1_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_I2C0, },
	{ .irq = -1 }
};

static struct omap_hwmod_dma_info i2c1_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_ocp_if *ti816x_i2c1_slaves[] = {
	&ti816x_l4_slow__i2c1,
};

static struct omap_hwmod ti81xx_i2c1_hwmod = {
	.name		= "i2c1",
	.mpu_irqs	= i2c1_mpu_irqs,
	.sdma_reqs	= i2c1_edma_reqs,
	.main_clk	= "i2c1_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_I2C_0_CLKCTRL_OFF,
		},
	},
	.slaves		= ti816x_i2c1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_i2c1_slaves),
	.class		= &i2c_class,
};

/* I2C2 */

/* L4 SLOW -> I2C2 */
static struct omap_hwmod_addr_space ti816x_i2c2_addr_space[] = {
	{
		.pa_start	= TI816X_I2C1_BASE,
		.pa_end		= TI816X_I2C1_BASE + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__i2c2 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_i2c2_hwmod,
	.clk		= "i2c2_ick",
	.addr		= ti816x_i2c2_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_irq_info i2c2_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_I2C1, },
	{ .irq = -1 }
};

static struct omap_hwmod_dma_info i2c2_edma_reqs[] = {
	{ .name = "tx", .dma_req = 0, },
	{ .name = "rx", .dma_req = 0, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_ocp_if *ti816x_i2c2_slaves[] = {
	&ti816x_l4_slow__i2c2,
};

static struct omap_hwmod ti816x_i2c2_hwmod = {
	.name           = "i2c2",
	.mpu_irqs       = i2c2_mpu_irqs,
	.sdma_reqs      = i2c2_edma_reqs,
	.main_clk       = "i2c2_fck",
	.prcm           = {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_I2C_1_CLKCTRL_OFF,
		},
	},
	.slaves         = ti816x_i2c2_slaves,
	.slaves_cnt     = ARRAY_SIZE(ti816x_i2c2_slaves),
	.class          = &i2c_class,
};

/* ELM */
static struct omap_hwmod_class_sysconfig ti81xx_elm_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0014,
	.sysc_flags	= (SYSC_HAS_CLOCKACTIVITY | SYSC_HAS_SIDLEMODE |
				SYSC_HAS_SOFTRESET |
				SYSS_HAS_RESET_STATUS),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};
/* 'elm' class */
static struct omap_hwmod_class ti81xx_elm_hwmod_class = {
	.name = "elm",
	.sysc = &ti81xx_elm_sysc,
};

static struct omap_hwmod_irq_info ti81xx_elm_irqs[] = {
	{ .irq = TI81XX_IRQ_ELM },
	{ .irq = -1 }
};

struct omap_hwmod_addr_space ti81xx_elm_addr_space[] = {
	{
		.pa_start	= TI81XX_ELM_BASE,
		.pa_end		= TI81XX_ELM_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__elm = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti81xx_elm_hwmod,
	.addr		= ti81xx_elm_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_elm_slaves[] = {
	&ti81xx_l4_slow__elm,
};

/* elm */
static struct omap_hwmod ti81xx_elm_hwmod = {
	.name           = "elm",
	.class          = &ti81xx_elm_hwmod_class,
	.main_clk       = "elm_fck",
	.mpu_irqs		= ti81xx_elm_irqs,
	.slaves			= ti81xx_elm_slaves,
	.slaves_cnt		= ARRAY_SIZE(ti81xx_elm_slaves),
};

/*
 * 'gpio' class
 * general purpose io module
 */
static struct omap_hwmod_class_sysconfig ti81xx_gpio_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0114,
	.sysc_flags	= (SYSC_HAS_AUTOIDLE | SYSC_HAS_ENAWAKEUP |
				   SYSC_HAS_SIDLEMODE | SYSC_HAS_SOFTRESET |
				   SYSS_HAS_RESET_STATUS),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART |
				   SIDLE_SMART_WKUP),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class ti81xx_gpio_hwmod_class = {
	.name	= "gpio",
	.sysc	= &ti81xx_gpio_sysc,
	.rev	= 2,
};

/* gpio dev_attr */
static struct omap_gpio_dev_attr gpio_dev_attr = {
	.bank_width	= 32,
	.dbck_flag	= true,
};

/* GPIO1 TI81XX */

/* L4 SLOW -> GPIO1 */
static struct omap_hwmod_addr_space ti81xx_gpio1_addrs[] = {
	{
		.pa_start	= TI81XX_GPIO0_BASE,
		.pa_end		= TI81XX_GPIO0_BASE + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__gpio1 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti81xx_gpio1_hwmod,
	.addr		= ti81xx_gpio1_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

static struct omap_hwmod_irq_info ti81xx_gpio1_irqs[] = {
	{ .irq = TI81XX_IRQ_GPIO_0A },
	{ .irq = TI81XX_IRQ_GPIO_0B },
	{ .irq = -1 }
};

/* gpio1 slave ports */
static struct omap_hwmod_ocp_if *ti81xx_gpio1_slaves[] = {
	&ti81xx_l4_slow__gpio1,
};

static struct omap_hwmod_opt_clk gpio1_opt_clks[] = {
	{ .role = "dbclk", .clk = "gpio1_dbck" },
};

static struct omap_hwmod ti81xx_gpio1_hwmod = {
	.name		= "gpio1",
	.class		= &ti81xx_gpio_hwmod_class,
	.mpu_irqs	= ti81xx_gpio1_irqs,
	.main_clk	= "gpio1_ick",
	.prcm = {
		.omap4 = {
			.clkctrl_offs = TI81XX_CM_ALWON_GPIO_0_CLKCTRL_OFF,
		},
	},
	.opt_clks	= gpio1_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(gpio1_opt_clks),
	.dev_attr	= &gpio_dev_attr,
	.slaves		= ti81xx_gpio1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_gpio1_slaves),
};

/* GPIO2 TI81XX*/

/* L4 SLOW -> GPIO2 */
static struct omap_hwmod_addr_space ti81xx_gpio2_addrs[] = {
	{
		.pa_start	= TI81XX_GPIO1_BASE,
		.pa_end		= TI81XX_GPIO1_BASE + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__gpio2 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti81xx_gpio2_hwmod,
	.addr		= ti81xx_gpio2_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

static struct omap_hwmod_irq_info ti81xx_gpio2_irqs[] = {
	{ .irq = TI81XX_IRQ_GPIO_1A },
	{ .irq = TI81XX_IRQ_GPIO_1B },
	{ .irq = -1 }
};

/* gpio2 slave ports */
static struct omap_hwmod_ocp_if *ti81xx_gpio2_slaves[] = {
	&ti81xx_l4_slow__gpio2,
};

static struct omap_hwmod_opt_clk gpio2_opt_clks[] = {
	{ .role = "dbclk", .clk = "gpio2_dbck" },
};

static struct omap_hwmod ti81xx_gpio2_hwmod = {
	.name		= "gpio2",
	.class		= &ti81xx_gpio_hwmod_class,
	.mpu_irqs	= ti81xx_gpio2_irqs,
	.main_clk	= "gpio2_ick",
	.prcm = {
		.omap4 = {
			.clkctrl_offs = TI81XX_CM_ALWON_GPIO_1_CLKCTRL_OFF,
		},
	},
	.opt_clks	= gpio2_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(gpio2_opt_clks),
	.dev_attr	= &gpio_dev_attr,
	.slaves		= ti81xx_gpio2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_gpio2_slaves),
};

/* L3 SLOW -> USBSS interface */
static struct omap_hwmod_addr_space ti81xx_usbss_addr_space[] = {
	{
		.name		= "usbss",
		.pa_start	= TI81XX_USBSS_BASE,
		.pa_end		= TI81XX_USBSS_BASE + SZ_4K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{
		.name		= "musb0",
		.pa_start	= TI81XX_USB0_BASE,
		.pa_end		= TI81XX_USB0_BASE + SZ_2K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{
		.name		= "musb1",
		.pa_start	= TI81XX_USB1_BASE,
		.pa_end		= TI81XX_USB1_BASE + SZ_2K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

static struct omap_hwmod_class_sysconfig ti81xx_usbhsotg_sysc = {
	.rev_offs	= 0x0,
	.sysc_offs	= 0x10,
};

static struct omap_hwmod_class ti81xx_usbotg_class = {
	.name = "usbotg",
	.sysc = &ti81xx_usbhsotg_sysc,
};

static struct omap_hwmod_irq_info ti81xx_usbss_mpu_irqs[] = {
	{ .name = "usbss-irq", .irq = TI81XX_IRQ_USBSS, },
	{ .name = "musb0-irq", .irq = TI81XX_IRQ_USB0, },
	{ .name = "musb1-irq", .irq = TI81XX_IRQ_USB1, },
	{ .irq = -1 }
};

static struct omap_hwmod_ocp_if ti81xx_l3_slow__usbss = {
	.master		= &ti816x_l3_slow_hwmod,
	.slave		= &ti81xx_usbss_hwmod,
	.clk		= "usb_ick",
	.addr		= ti81xx_usbss_addr_space,
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_usbss_slaves[] = {
	&ti81xx_l3_slow__usbss,
};

static struct omap_hwmod ti81xx_usbss_hwmod = {
	.name		= "usb_otg_hs",
	.mpu_irqs	= ti81xx_usbss_mpu_irqs,
	.main_clk	= "usb_ick",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_DEFAULT_USB_CLKCTRL_OFF,
		},
	},
	.slaves		= ti81xx_usbss_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_usbss_slaves),
	.class		= &ti81xx_usbotg_class,
};

/* timer class */
static struct omap_hwmod_class_sysconfig ti816x_timer_1ms_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0014,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE | SYSC_HAS_CLOCKACTIVITY |
				   SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
				   SYSC_HAS_EMUFREE | SYSC_HAS_AUTOIDLE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class ti816x_timer_1ms_hwmod_class = {
	.name = "timer",
	.sysc = &ti816x_timer_1ms_sysc,
	.rev = OMAP_TIMER_IP_VERSION_1,
};

static struct omap_hwmod_class_sysconfig ti816x_timer_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0014,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE | SYSC_HAS_ENAWAKEUP |
				   SYSC_HAS_SOFTRESET | SYSC_HAS_AUTOIDLE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class ti816x_timer_hwmod_class = {
	.name = "timer",
	.sysc = &ti816x_timer_sysc,
	.rev =  OMAP_TIMER_IP_VERSION_1,
};

/* always-on timers dev attribute */
static struct omap_timer_capability_dev_attr capability_alwon_dev_attr = {
	.timer_capability       = OMAP_TIMER_ALWON,
};

/* timer1 */
static struct omap_hwmod ti816x_timer1_hwmod;

static struct omap_hwmod_addr_space ti816x_timer1_addrs[] = {
	{
		.pa_start	= TI816X_TIMER0_BASE,
		.pa_end		= TI816X_TIMER0_BASE + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

struct omap_hwmod_irq_info ti816x_timer1_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_GPT1, },
	{ .irq = -1 }
};

/* l4_wkup -> timer1 */
static struct omap_hwmod_ocp_if ti816x_l4_wkup__timer1 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_timer1_hwmod,
	.clk		= "gpt1_ick",
	.addr		= ti816x_timer1_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer1 slave port */
static struct omap_hwmod_ocp_if *ti816x_timer1_slaves[] = {
	&ti816x_l4_wkup__timer1,
};

/* timer1 hwmod */
static struct omap_hwmod ti816x_timer1_hwmod = {
	.name		= "timer1",
	.mpu_irqs	= ti816x_timer1_mpu_irqs,
	.main_clk	= "gpt1_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_TIMER_0_CLKCTRL_OFF,
		},
	},
	.dev_attr	= &capability_alwon_dev_attr,
	.slaves		= ti816x_timer1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer1_slaves),
	.class		= &ti816x_timer_1ms_hwmod_class,
};

/* timer2 */
static struct omap_hwmod ti816x_timer2_hwmod;

static struct omap_hwmod_addr_space ti816x_timer2_addrs[] = {
	{
		.pa_start	= TI816X_TIMER1_BASE,
		.pa_end		= TI816X_TIMER1_BASE + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

struct omap_hwmod_irq_info ti816x_timer2_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_GPT2, },
	{ .irq = -1 }
};

/* l4_per -> timer2 */
static struct omap_hwmod_ocp_if ti816x_l4_per__timer2 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_timer2_hwmod,
	.clk		= "gpt2_ick",
	.addr		= ti816x_timer2_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer2 slave port */
static struct omap_hwmod_ocp_if *ti816x_timer2_slaves[] = {
	&ti816x_l4_per__timer2,
};

/* timer2 hwmod */
static struct omap_hwmod ti816x_timer2_hwmod = {
	.name		= "timer2",
	.mpu_irqs	= ti816x_timer2_mpu_irqs,
	.main_clk	= "gpt2_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_TIMER_1_CLKCTRL_OFF,
		},
	},
	.dev_attr	= &capability_alwon_dev_attr,
	.slaves		= ti816x_timer2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer2_slaves),
	.class		= &ti816x_timer_1ms_hwmod_class,
};

/* timer3 */
static struct omap_hwmod ti816x_timer3_hwmod;

static struct omap_hwmod_addr_space ti816x_timer3_addrs[] = {
	{
		.pa_start	= TI816X_TIMER2_BASE,
		.pa_end		= TI816X_TIMER2_BASE + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

struct omap_hwmod_irq_info ti816x_timer3_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_GPT3, },
	{ .irq = -1 }
};

/* l4_per -> timer3 */
static struct omap_hwmod_ocp_if ti816x_l4_per__timer3 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_timer3_hwmod,
	.clk		= "gpt3_ick",
	.addr		= ti816x_timer3_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer3 slave port */
static struct omap_hwmod_ocp_if *ti816x_timer3_slaves[] = {
	&ti816x_l4_per__timer3,
};

/* timer3 hwmod */
static struct omap_hwmod ti816x_timer3_hwmod = {
	.name		= "timer3",
	.mpu_irqs	= ti816x_timer3_mpu_irqs,
	.main_clk	= "gpt3_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_TIMER_2_CLKCTRL_OFF,
		},
	},
	.dev_attr	= &capability_alwon_dev_attr,
	.slaves		= ti816x_timer3_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer3_slaves),
	.class		= &ti816x_timer_hwmod_class,
};

/* timer4 */
static struct omap_hwmod ti816x_timer4_hwmod;

static struct omap_hwmod_addr_space ti816x_timer4_addrs[] = {
	{
		.pa_start	= TI816X_TIMER3_BASE,
		.pa_end		= TI816X_TIMER3_BASE + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

struct omap_hwmod_irq_info ti816x_timer4_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_GPT4, },
	{ .irq = -1 }
};

/* l4_per -> timer4 */
static struct omap_hwmod_ocp_if ti816x_l4_per__timer4 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_timer4_hwmod,
	.clk		= "gpt4_ick",
	.addr		= ti816x_timer4_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer4 slave port */
static struct omap_hwmod_ocp_if *ti816x_timer4_slaves[] = {
	&ti816x_l4_per__timer4,
};

/* timer4 hwmod */
static struct omap_hwmod ti816x_timer4_hwmod = {
	.name		= "timer4",
	.mpu_irqs	= ti816x_timer4_mpu_irqs,
	.main_clk	= "gpt4_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_TIMER_3_CLKCTRL_OFF,
		},
	},
	.dev_attr	= &capability_alwon_dev_attr,
	.slaves		= ti816x_timer4_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer4_slaves),
	.class		= &ti816x_timer_hwmod_class,
};

/* timer5 */
static struct omap_hwmod ti816x_timer5_hwmod;

static struct omap_hwmod_addr_space ti816x_timer5_addrs[] = {
	{
		.pa_start	= TI816X_TIMER4_BASE,
		.pa_end		= TI816X_TIMER4_BASE + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

struct omap_hwmod_irq_info ti816x_timer5_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_GPT5, },
	{ .irq = -1 }
};

/* l4_per -> timer5 */
static struct omap_hwmod_ocp_if ti816x_l4_per__timer5 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_timer5_hwmod,
	.clk		= "gpt5_ick",
	.addr		= ti816x_timer5_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer5 slave port */
static struct omap_hwmod_ocp_if *ti816x_timer5_slaves[] = {
	&ti816x_l4_per__timer5,
};

/* timer5 hwmod */
static struct omap_hwmod ti816x_timer5_hwmod = {
	.name		= "timer5",
	.mpu_irqs	= ti816x_timer5_mpu_irqs,
	.main_clk	= "gpt5_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_TIMER_4_CLKCTRL_OFF,
		},
	},
	.dev_attr	= &capability_alwon_dev_attr,
	.slaves		= ti816x_timer5_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer5_slaves),
	.class		= &ti816x_timer_hwmod_class,
};

/* timer6 */
static struct omap_hwmod ti816x_timer6_hwmod;

static struct omap_hwmod_addr_space ti816x_timer6_addrs[] = {
	{
		.pa_start	= TI816X_TIMER5_BASE,
		.pa_end		= TI816X_TIMER5_BASE + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

struct omap_hwmod_irq_info ti816x_timer6_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_GPT6, },
	{ .irq = -1 }
};

/* l4_per -> timer6 */
static struct omap_hwmod_ocp_if ti816x_l4_per__timer6 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_timer6_hwmod,
	.clk		= "gpt6_ick",
	.addr		= ti816x_timer6_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer6 slave port */
static struct omap_hwmod_ocp_if *ti816x_timer6_slaves[] = {
	&ti816x_l4_per__timer6,
};

/* timer6 hwmod */
static struct omap_hwmod ti816x_timer6_hwmod = {
	.name		= "timer6",
	.mpu_irqs	= ti816x_timer6_mpu_irqs,
	.main_clk	= "gpt6_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_TIMER_5_CLKCTRL_OFF,
		},
	},
	.dev_attr	= &capability_alwon_dev_attr,
	.slaves		= ti816x_timer6_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer6_slaves),
	.class		= &ti816x_timer_hwmod_class,
};

/* timer7 */
static struct omap_hwmod ti816x_timer7_hwmod;

static struct omap_hwmod_addr_space ti816x_timer7_addrs[] = {
	{
		.pa_start	= TI816X_TIMER6_BASE,
		.pa_end		= TI816X_TIMER6_BASE + SZ_1K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{ }
};

struct omap_hwmod_irq_info ti816x_timer7_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_GPT7, },
	{ .irq = -1 }
};

/* l4_per -> timer7 */
static struct omap_hwmod_ocp_if ti816x_l4_per__timer7 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_timer7_hwmod,
	.clk		= "gpt7_ick",
	.addr		= ti816x_timer7_addrs,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* timer7 slave port */
static struct omap_hwmod_ocp_if *ti816x_timer7_slaves[] = {
	&ti816x_l4_per__timer7,
};

/* timer7 hwmod */
static struct omap_hwmod ti816x_timer7_hwmod = {
	.name		= "timer7",
	.mpu_irqs	= ti816x_timer7_mpu_irqs,
	.main_clk	= "gpt7_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_TIMER_6_CLKCTRL_OFF,
		},
	},
	.dev_attr	= &capability_alwon_dev_attr,
	.slaves		= ti816x_timer7_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_timer7_slaves),
	.class		= &ti816x_timer_hwmod_class,
};

/* MMC/SD/SDIO common */

static struct omap_hwmod_class_sysconfig ti816x_mmc_sysc = {
	.rev_offs	= 0x0,
	.sysc_offs	= 0x110,
	.syss_offs	= 0x114,
	.sysc_flags	= (SYSC_HAS_CLOCKACTIVITY | SYSC_HAS_SIDLEMODE |
				   SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
				   SYSC_HAS_AUTOIDLE | SYSS_HAS_RESET_STATUS),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class ti816x_mmc_class = {
	.name = "mmc",
	.sysc = &ti816x_mmc_sysc,
};

/* MMC/SD/SDIO1 */

static struct omap_hwmod_irq_info ti816x_mmc1_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_SD, },
	{ .irq = -1 }
};

static struct omap_hwmod_dma_info ti816x_mmc1_sdma_reqs[] = {
	{ .name = "tx",	.dma_req = OMAP24XX_DMA_MMC1_TX, },
	{ .name = "rx",	.dma_req = OMAP24XX_DMA_MMC1_RX, },
	{ .dma_req = -1 }
};

static struct omap_hwmod_opt_clk ti816x_mmc1_opt_clks[] = {
	{ .role = "dbck", .clk = "sys_32k_ck", },
};

struct omap_hwmod_addr_space ti816x_mmc1_addr_space[] = {
	{
		.pa_start	= TI816X_MMC0_BASE,
		.pa_end		= TI816X_MMC0_BASE + SZ_64K - 1,
		.flags		= ADDR_TYPE_RT,
	},
	{ }
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__mmc1 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_mmc1_hwmod,
	.clk		= "mmchs1_ick",
	.addr		= ti816x_mmc1_addr_space,
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
	.flags		= OMAP_FIREWALL_L4
};

static struct omap_hwmod_ocp_if *ti816x_mmc1_slaves[] = {
	&ti816x_l4_slow__mmc1,
};

static struct omap_mmc_dev_attr mmc1_dev_attr = {
	.flags = OMAP_HSMMC_SUPPORTS_DUAL_VOLT,
};

static struct omap_hwmod ti816x_mmc1_hwmod = {
	.name		= "mmc1",
	.mpu_irqs	= ti816x_mmc1_mpu_irqs,
	.sdma_reqs	= ti816x_mmc1_sdma_reqs,
	.opt_clks	= ti816x_mmc1_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(ti816x_mmc1_opt_clks),
	.main_clk	= "mmchs1_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_offs = TI816X_CM_ALWON_SDIO_CLKCTRL_OFF,
		},
	},
	.dev_attr	= &mmc1_dev_attr,
	.slaves		= ti816x_mmc1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_mmc1_slaves),
	.class		= &ti816x_mmc_class,
};

/*
 * IVA2_2 interface data
 */

/* IVA2 <- L3 interface */
static struct omap_hwmod_ocp_if ti816x_l3__iva = {
	.master		= &ti816x_l3_slow_hwmod,
	.slave		= &ti816x_iva_hwmod,
	.clk		= "iva2_ck",
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

static struct omap_hwmod_ocp_if *ti816x_iva_masters[] = {
	&ti816x_l3__iva,
};

/*
 * IVA2 (IVA2)
 */

static struct omap_hwmod ti816x_iva_hwmod = {
	.name		= "iva",
	.class		= &iva_hwmod_class,
	.masters	= ti816x_iva_masters,
	.masters_cnt	= ARRAY_SIZE(ti816x_iva_masters),
};

static __initdata struct omap_hwmod *ti816x_hwmods[] = {
	&ti816x_l3_slow_hwmod,
	&ti816x_l4_slow_hwmod,
	&ti816x_mpu_hwmod,

	&ti816x_timer1_hwmod,
	&ti816x_timer2_hwmod,
	&ti816x_timer3_hwmod,
	&ti816x_timer4_hwmod,
	&ti816x_timer5_hwmod,
	&ti816x_timer6_hwmod,
	&ti816x_timer7_hwmod,

	&ti816x_uart1_hwmod,
	&ti816x_uart2_hwmod,
	&ti816x_uart3_hwmod,

	&ti816x_wd_timer2_hwmod,

	&ti81xx_i2c1_hwmod,
	&ti816x_i2c2_hwmod,

	&ti81xx_gpio1_hwmod,
	&ti81xx_gpio2_hwmod,

	&ti81xx_usbss_hwmod,

	/* &ti81xx_elm_hwmod, */

	&ti816x_mmc1_hwmod,

	&ti816x_iva_hwmod,
	NULL,
};

int __init ti81xx_hwmod_init(void)
{
	if (cpu_is_ti816x())
		return omap_hwmod_register(ti816x_hwmods);
	return 0;
}
