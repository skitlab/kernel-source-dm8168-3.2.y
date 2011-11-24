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
#include <plat/sram.h>
#include <plat/ti81xx.h>
#include <mach/io.h>

#include "cm2xxx_3xxx.h"
#include "prm2xxx_3xxx.h"
#include "sdrc.h"
#include "control.h"
#include "cm81xx.h"

/* DDR PMCR configuration for Self-refresh
 * LP_MOD - Self refresh
 * SR_TIM - 16 ddr cycles
*/
#define ENTER_SELF_REFRESH				0x210

/* Wait time for cke low (~1ms) */
#define CKE_WAIT_LOOP_COUNT				1000

/* DLL wait count - 1s (should be >10ms) */
#define DLL_LOCK_WAIT_COUNT				3000000

/* EMIF clk Gate reg val */
#define EMIF_DDR_PHY_CLK_GATE_VAL			0x3

/* DDR PLL control */
#define DDR_PLL_DIS_TINITZ				0xfffffffe
#define DDR_PLL_EN_TINITZ				0x1
#define DDR_PLL_CLEAR_IDLE_BIT				0xff7fffff
#define DDR_PLL_CONTROL	TI814X_PLL_REGADDR(DDR_PLL_BASE + ADPLLJ_CLKCTRL)
#define DDR_PLL_STATUS	TI814X_PLL_REGADDR(DDR_PLL_BASE + ADPLLJ_STATUS)

/*
 * ======================
 * == Idle entry point ==
 * ======================
 */

ENTRY(ti814x_cpu_suspend)
	stmfd	sp!, {r0-r12, lr}	@ save registers on stack

/* Accessing some register may result in tlb miss and lead to page table walk
 * which is in ddr. Accessing ddr while its in self-refresh may result
 * in content loss, to prevent this access all registers once before
 * entering self-refresh, so that page table entries are available in tlb
 */
load_regs_to_mem:
	ldr	r1, _emif0_base
	ldr	r1, _emif1_base
	ldr	r1, _dmm_base
	ldr	r1, emif_pmcr_offset
	ldr	r1, enter_self_refresh
	ldr	r1, dmm_lisa_0
	ldr	r1, emif_clk_gate
	ldr	r1, ddr_pll_control
	ldr	r1, ddr_pll_status
	ldr	r1, _ddr_pll_dis_tinitz
	ldr	r1, _ddr_pll_en_tinitz
	ldr	r1, _ddr_pll_clear_idle
	ldr	r1, _dll_wait_count
	ldr	r1, _wait_count
	ldr	r1, emif_phy_clk_gate_val

/* Enter self refresh by configuring PMCR register of EMIF 0/1 */
enter_ddr_self_refresh:
	ldr	r1, _emif0_base
	ldr	r4, emif_pmcr_offset
	add	r1, r1, r4
	ldr	r3, enter_self_refresh
	str	r3, [r1]
	ldr	r1, _emif1_base
	add	r1, r1, r4
	str	r3, [r1]
wait_for_cke_low:
	ldr	r1, _wait_count
cke_loop:
	subs	r1, r1, #0x1
	bne	cke_loop

/* Once in self refresh emif should not be accessed so configure dmm
 * for no emif, only mmr access is allowed
 * Save the configuration to r9,r10,r11 and r12 to restore later
 */
configure_dmm_for_no_emif:
	ldr	r4, _dmm_base
	ldr	r1, dmm_lisa_0
	add	r1, r1, r4
	ldr	r2, [r1]
	mov	r9, r2
	mov	r3, #0x0
	str	r3, [r1]
	ldr	r1, dmm_lisa_1
	add	r1, r1, r4
	ldr	r2, [r1]
	mov	r10, r2
	str	r3, [r1]
	ldr	r1, dmm_lisa_2
	add	r1, r1, r4
	ldr	r2, [r1]
	mov	r11, r2
	str	r3, [r1]
	ldr	r1, dmm_lisa_3
	add	r1, r1, r4
	ldr	r2, [r1]
	mov	r12, r2
	str	r3, [r1]

/* Gate DDR PHY 0/1 clocks */
gate_dfi_clocks:
	ldr	r1, emif_clk_gate
	ldr	r3, emif_phy_clk_gate_val
	str	r3, [r1]

/* Place DDR_PLL in Bypass, to save power
 * Clearing tinitz drives pll to bypass
 */
ddr_pll_enter_bypass:
	ldr	r1, ddr_pll_control
	ldr	r2, [r1]
	ldr	r3, _ddr_pll_dis_tinitz
	and	r2, r2, r3
	str	r2, [r1]

/* Wait for Frequency and Phase lock bits to clear */
wait_for_freq_unlock:
	ldr	r1, ddr_pll_status
unlock_loop:
	ldr	r2, [r1]
	mov	r3, #0x600
	and	r2, r2, r3
	cmp	r2, #0x0
	bne	unlock_loop

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

/* NOPs to clear pipe line */
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

/* Bring pll out of bypass
 * Setting tinitz starts locking sequence
 * DDR_PLL will be locked to the same frequncy as before entering self-refresh
 * Clear idle bypass bit to ensure it does not go into idle
 * Wait till Frequency is locked
 */
ddr_pll_exit_bypass:
	ldr	r1, ddr_pll_control
	ldr	r2, [r1]
	ldr	r3, _ddr_pll_en_tinitz
	orr	r2, r2, r3
	str	r2, [r1]
clear_idle_bit:
	ldr	r1, ddr_pll_control
	ldr	r2, [r1]
	ldr	r3, _ddr_pll_clear_idle
	and	r2, r2, r3
	str	r2, [r1]

/* Wait till Frequency lock and Phase lock bits are asserted */
wait_for_freq_lock:
	ldr	r1, ddr_pll_status
lock_loop:
	ldr	r2, [r1]
	mov	r3, #0x600
	and	r2, r2, r3
	cmp	r2, #0x600
	bne	lock_loop

/* Frequency lock bit is asserted as soon as frequency is
 * +/- 1% close to the target frequency, so when frequency is locked
 * we might have not achieved final frequency.
 * Wait for some time to allow frequency synthesis(this should take some ns)
 */
wait_for_pll_synthesis:
	ldr	r1, _wait_count
pll_loop:
	subs	r1, r1, #0x1
	bne	pll_loop

/* Ungate DDR pHY 0/1 Clocks */
ungate_dfi_clocks:
	ldr	r1, emif_clk_gate
	mov	r3, #0x0
	str	r3, [r1]

/* Wait for DLL to lock before configuring dmm */
wait_for_dll_lock:
	ldr	r1, _dll_wait_count
dll_lock_loop:
	subs	r1, r1, #0x1
	bne	dll_lock_loop
/* Restore DMM configuration back to what it was before self-refresh
 * We have saved configuration to r9,r10,r11 and r12
 */
reconfigure_dmm:
	ldr	r4, _dmm_base
	ldr	r1, dmm_lisa_0
	add	r1, r1, r4
	str	r9, [r1]
	ldr	r1, dmm_lisa_1
	add	r1, r1, r4
	str	r10, [r1]
	ldr	r1, dmm_lisa_2
	add	r1, r1, r4
	str	r11, [r1]
	ldr	r1, dmm_lisa_3
	add	r1, r1, r4
	str	r12, [r1]

/* prevent ddr going in to self refresh in idle time */
block_self_refresh:
	ldr	r1, _emif0_base
	ldr	r4, emif_pmcr_offset
	add	r1, r1, r4
	mov	r3, #0x0
	str	r3, [r1]		@ write value to register
	ldr	r1, _emif1_base
	add	r1, r1, r4
	str	r3, [r1]

/*
 *=====================================
 * Exit Point
 *=====================================
 */

/* Restore registers from stack and return to ddr */
restore_regs_pc:
	ldmfd	sp!, {r0-r12, pc}	@ restore regs and return

/* Macros - Register addresses, constants */
_emif0_base:
	.word emif0_base
_emif1_base:
	.word emif1_base
_dmm_base:
	.word dmm_base
emif_pmcr_offset:
	.word TI814X_PMCR
dmm_lisa_0:
	.word TI814X_DMM_LISA_MAP_0
dmm_lisa_1:
	.word TI814X_DMM_LISA_MAP_1
dmm_lisa_2:
	.word TI814X_DMM_LISA_MAP_2
dmm_lisa_3:
	.word TI814X_DMM_LISA_MAP_3
emif_clk_gate:
	.word TI814X_PLL_CMGC_EMIF_CLK_GATE
ddr_pll_control:
	.word DDR_PLL_CONTROL
ddr_pll_status:
	.word DDR_PLL_STATUS
_ddr_pll_dis_tinitz:
	.word DDR_PLL_DIS_TINITZ
_ddr_pll_en_tinitz:
	.word DDR_PLL_EN_TINITZ
_ddr_pll_clear_idle:
	.word DDR_PLL_CLEAR_IDLE_BIT
_wait_count:
	.word CKE_WAIT_LOOP_COUNT
_dll_wait_count:
	.word DLL_LOCK_WAIT_COUNT
enter_self_refresh:
	.word ENTER_SELF_REFRESH
emif_phy_clk_gate_val:
	.word EMIF_DDR_PHY_CLK_GATE_VAL
/* Suspend function size */
ENTRY(ti814x_cpu_suspend_sz)
	.word	. - ti814x_cpu_suspend
