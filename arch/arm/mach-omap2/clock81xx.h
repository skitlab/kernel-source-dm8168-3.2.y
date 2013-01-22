/*
 * TI816X clock function prototypes and macros.
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

#ifndef __ARCH_ARM_MACH_OMAP2_CLOCK81XX_H
#define __ARCH_ARM_MACH_OMAP2_CLOCK81XX_H

int ti81xx_clk_init(void);
#if defined(CONFIG_MACH_TI8148EVM)
int ti814x_clk_init(void);
#else
#define ti814x_clk_init() do {} while (0);
#endif

#if defined(CONFIG_MACH_TI8168EVM)
int ti816x_clk_init(void);
int ti816x_fapll_init(void);
#else
#define ti816x_clk_init() do {} while (0);
#define ti816x_fapll_init() do {} while (0);
#endif

extern const struct clkops clkops_ti81xx_dflt_wait;
extern const struct clkops clkops_ti81xx_pcie;
extern const struct clkops clkops_ti81xx_usb;

#endif
