/*
 * TI81XX CM register bits.
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

#ifndef __ARCH_ARM_MACH_OMAP2_CM_REGBITS_81XX_H
#define __ARCH_ARM_MACH_OMAP2_CM_REGBITS_81XX_H

#include "cm.h"

#define TI81XX_CLKSEL_0_0_SHIFT					0
#define TI81XX_CLKSEL_0_0_MASK					(1 << 0)

#define TI81XX_CLKSEL_0_1_SHIFT					0
#define TI81XX_CLKSEL_0_1_MASK					(3 << 0)

#define TI81XX_CLKSEL_0_2_SHIFT					0
#define TI81XX_CLKSEL_0_2_MASK					(7 << 0)

/* Modulemode bit */
#define TI81XX_MODULEMODE_SWCTRL				1

/* IDLEST bit */
#define TI81XX_IDLEST_SHIFT					16
#define TI81XX_IDLEST_MASK					(3 << 16)
#define TI81XX_IDLEST_VAL					3

/* Used for clockdomain control */
#define TI81XX_CLKTRCTRL_SHIFT					0
#define TI81XX_CLKTRCTRL_MASK					(3 << 0)

/* Select DMTIMER source */
#define TI814X_DMTIMER1_CLKS_SHIFT				3
#define TI814X_DMTIMER1_CLKS_MASK				(7 << 3)

#define TI814X_DMTIMER2_CLKS_SHIFT				6
#define TI814X_DMTIMER2_CLKS_MASK				(7 << 6)

#define TI814X_DMTIMER3_CLKS_SHIFT				9
#define TI814X_DMTIMER3_CLKS_MASK				(7 << 9)

#define TI814X_DMTIMER4_CLKS_SHIFT				16
#define TI814X_DMTIMER4_CLKS_MASK				(7 << 16)

#define TI814X_DMTIMER5_CLKS_SHIFT				19
#define TI814X_DMTIMER5_CLKS_MASK				(7 << 19)

#define TI814X_DMTIMER6_CLKS_SHIFT				22
#define TI814X_DMTIMER6_CLKS_MASK				(7 << 22)

#define TI814X_DMTIMER7_CLKS_SHIFT				25
#define TI814X_DMTIMER7_CLKS_MASK				(7 << 25)

#define TI814X_DMTIMER8_CLKS_SHIFT				0
#define TI814X_DMTIMER8_CLKS_MASK				(7 << 0)


/* TODO: Add other specific CM register bits */

#endif
