/*
 * This file contains the address data for various ti81xx modules.
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

#ifndef __ASM_ARCH_TI81XX_H
#define __ASM_ARCH_TI81XX_H

#define L3_TI81XX_BASE		0x44000000
#define L4_FAST_TI81XX_BASE	0x4a000000
#define L4_SLOW_TI81XX_BASE	0x48000000

#define TI81XX_SCM_BASE		0x48140000
#define TI81XX_CTRL_BASE	TI81XX_SCM_BASE
#define TI81XX_PRCM_BASE	0x48180000
#define TI814X_PLL_BASE		0x481C5000

#define TI81XX_ARM_INTC_BASE	0x48200000

#define TI81XX_GPMC_BASE	0x50000000

#define TI81XX_USBSS_BASE	0x47400000
#define TI81XX_USBSS_LEN	0xFFF
#define TI81XX_USB0_BASE	0x47401000
#define TI81XX_USB1_BASE	0x47401800
#define TI81XX_USB_CPPIDMA_BASE	0x47402000
#define TI81XX_USB_CPPIDMA_LEN	0x5FFF

#define TI81XX_SATA_BASE	0x4A140000

#endif /* __ASM_ARCH_TI81XX_H */
