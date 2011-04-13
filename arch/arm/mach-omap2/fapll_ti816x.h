/*
 * TI816X FAPLL clock header file
 *
 * Copyright (C) 2011 Texas Instruments, Inc. - http://www.ti.com/
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
#include <linux/errno.h>
#include <linux/io.h>

#include "cm-regbits-81xx.h"

struct freq_parameters {
	u32 synthesizer_id;
	u32 mult_n;
	u32 pre_div_p;
	u32 post_div_m;
	u32 syn_integer;
	u32 syn_frac;
	u32 out_freq;
};

#define FAPLL(FAP, SID, ofrq)						\
{									\
	.synthesizer_id = SID,						\
	.mult_n = TI816X_##FAP##_N,					\
	.pre_div_p = TI816X_##FAP##_P,					\
	.post_div_m = TI816X_##FAP##_MDIV##SID,				\
	.syn_integer = TI816X_##FAP##_INTFREQ##SID,			\
	.syn_frac = TI816X_##FAP##_FRACFREQ##SID,			\
	.out_freq = ofrq,						\
}

#define MAX_FAPLL_WAIT_TRIES		1000
#define TI816X_FAPLL_K			8

