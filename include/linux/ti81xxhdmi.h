/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2009 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/

/**
 *  \file ti81xxhdmi.h
 *
 *  \brief User level interface file for using TI81xx hdmi character driver.
 *
 */

#ifndef __TI81XXHDMI_H__
#define __TI81XXHDMI_H__


#include <linux/ioctl.h>

/* IOCTL definitions */
#define TI81XXHDMI_MAGIC            'N'
#define TI81XXHDMI_IOW(num, dtype)  _IOW(TI81XXHDMI_MAGIC, num, dtype)
#define TI81XXHDMI_IOR(num, dtype)  _IOR(TI81XXHDMI_MAGIC, num, dtype)
#define TI81XXHDMI_IOWR(num, dtype) _IOWR(TI81XXHDMI_MAGIC, num, dtype)
#define TI81XXHDMI_IO(num)          _IO(TI81XXHDMI_MAGIC, num)

/* IOCLT Supported by this driver */
#define TI81XXHDMI_START      		TI81XXHDMI_IO(0)
#define TI81XXHDMI_STOP       		TI81XXHDMI_IO(1)
#define TI81XXHDMI_GET_STATUS 		TI81XXHDMI_IOR(2, int *)
#define TI81XXHDMI_READ_EDID  		TI81XXHDMI_IOR(3, struct ti81xxdhmi_edid_params)
/* Use this command only when hdmi is streaming video to a sink */
/* TODO Not supported for now */
#if 0
#define TI81XXHDMI_GET_CONFIG 		TI81XXHDMI_IOR(4, struct hdmi_cfg_params)
#define TI81XXHDMI_SET_CONFIG 		TI81XXHDMI_IOW(5, struct hdmi_cfg_params)
#endif
#define TI81XXHDMI_SET_MODE         	TI81XXHDMI_IOW(6, enum ti81xxhdmi_mode)
#define TI81XXHDMI_GET_MODE         	TI81XXHDMI_IOR(7, enum ti81xxhdmi_mode)
#define TI81XXHDMI_TEST_HDMI        	TI81XXHDMI_IOW(8, enum ti81xxhdmi_mode)
#define TI81XXHDMI_GET_PHY_STAT		TI81XXHDMI_IOR(9, enum ti81xxhdmi_mode)
/* Data Types */

/* Supported resolutions */
enum ti81xxhdmi_mode {
	hdmi_ntsc_mode = 0,
	hdmi_pal_mode,
	hdmi_1080P_60_mode,
	hdmi_720P_60_mode,
	hdmi_1080I_60_mode,
	hdmi_1080P_30_mode,
	hdmi_max_mode
};
struct ti81xxdhmi_edid_params {
	unsigned int slave_address;
	unsigned int segment_ptr;
	unsigned int offset;
	unsigned int no_of_bytes;
	void *buffer_ptr;
	unsigned int no_of_bytes_read;
	unsigned int timeout;
	unsigned int use_eddc_read;
};
struct ti81xxhdmi_phy_status {
	unsigned int rst_done_pclk;
	unsigned int rst_done_pwrclk;
	unsigned int rst_done_scpclk;
	unsigned int rst_done_refclk;
	unsigned int dct_5v_short_clk;
	unsigned int rx_detect;
	unsigned int dct_5v_short_data;
};
#endif /* End of #ifndef __TI81XXHDMI_H__ */


