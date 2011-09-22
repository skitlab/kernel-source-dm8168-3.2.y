/*
 * arch/arm/mach-omap2/board-ti8168evm.c
 *
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

#ifndef _BOARD_TI816X_H
#define _BOARD_TI816X_H
#include <linux/fvid2.h>

#define VPS_SEL_TVP7002_DECODER	0
#define VPS_SEL_SIL9135_DECODER	1


enum ti816x_ths_filter_ctrl {
	TI816X_THSFILTER_ENABLE_MODULE = 0,
	TI816X_THSFILTER_BYPASS_MODULE,
	TI816X_THSFILTER_DISABLE_MODULE
};

/* \brief Enum for selecting filter for component input/output in THS7360 */
enum ti816x_ths7360_sf_ctrl {
	TI816X_THS7360_DISABLE_SF = 0,
	TI816X_THS7360_BYPASS_SF,
	TI816X_THS7360_SF_SD_MODE,
	TI816X_THS7360_SF_ED_MODE,
	TI816X_THS7360_SF_HD_MODE,
	TI816X_THS7360_SF_TRUE_HD_MODE
};

#ifdef CONFIG_ARCH_TI816X
int pcf8575_ths7375_enable(enum ti816x_ths_filter_ctrl ctrl);
int pcf8575_ths7360_sd_enable(enum ti816x_ths_filter_ctrl ctrl);
int pcf8575_ths7360_hd_enable(enum ti816x_ths7360_sf_ctrl ctrl);
int ti816x_pcf8575_init(void);
int ti816x_pcf8575_exit(void);
int vps_ti816x_select_video_decoder(int vid_decoder_id);
int vps_ti816x_set_tvp7002_filter(enum fvid2_standard standard);
#else
int pcf8575_ths7375_enable(enum ti816x_ths_filter_ctrl ctrl)
{
	return 0;
}
int pcf8575_ths7360_sd_enable(enum ti816x_ths_filter_ctrl ctrl)
{
	return 0;
}
int pcf8575_ths7360_hd_enable(enum ti816x_ths7360_sf_ctrl ctrl)
{
	return 0;
}
int ti816x_pcf8575_init(void)
{
	return 0;
}
int ti816x_pcf8575_exit(void)
{
	return 0;
}
int vps_ti816x_select_video_decoder(int vid_decoder_id)
{
	return 0;
}
int vps_ti816x_set_tvp7002_filter(enum fvid2_standard standard)
{
	return 0;
}
#endif
#endif

