/*
 * TI81XX Suspend to Memory
 *
 * This module implements TI814X specific suspend to memory routines
 *
 * Copyright (C) {2011} Texas Instruments Incorporated - http://www.ti.com/
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

#include <linux/linkage.h>
#include <asm/assembler.h>

/*
 * ======================
 * == Idle entry point ==
 * ======================
 */

ENTRY(ti814x_cpu_suspend)
	stmfd	sp!, {r0-r12, lr}	@ save registers on stack


/* Execute wfi on A8 */
ti814x_do_wfi:

/* Data memory barrier and Data sync barrier */
	dmb
	dsb
/*
 * ===================================
 * == WFI instruction => Enter idle ==
 * ===================================
 */

	wfi				@ wait for interrupt

/*
 * ===================================
 * == Resume path ==
 * ===================================
 */
/* Restore registers from stack and return to ddr */
restore_regs_pc:
	ldmfd	sp!, {r0-r12, pc}	@ restore regs and return

/* Suspend function size */
ENTRY(ti814x_cpu_suspend_sz)
	.word	. - ti814x_cpu_suspend
