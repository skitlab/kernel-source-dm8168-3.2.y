/*
 *
 * HDMI driver header file for TI 816x
 *
 * Copyright (C) 2010 TI
 * Author: Sujith Shivalingappa <sujith.s@ti.com>
 *
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA	02111-1307, USA.
 */

#ifndef __TI816X_HDMI_H__
#define __TI816X_HDMI_H__


#include <linux/ioctl.h>

/* IOCTL definitions */
#define TI816XHDMI_MAGIC            'N'
#define TI816XHDMI_IOW(num, dtype)  _IOW(TI816XHDMI_MAGIC, num, dtype)
#define TI816XHDMI_IOR(num, dtype)  _IOR(TI816XHDMI_MAGIC, num, dtype)
#define TI816XHDMI_IOWR(num, dtype) _IOWR(TI816XHDMI_MAGIC, num, dtype)
#define TI816XHDMI_IO(num)          _IO(TI816XHDMI_MAGIC, num)

/* IOCLT Supported by this driver */
#define TI816XHDMI_START      		TI816XHDMI_IO(0)
#define TI816XHDMI_STOP       		TI816XHDMI_IO(1)
#define TI816XHDMI_GET_STATUS 		TI816XHDMI_IOR(2, int *)
#define TI816XHDMI_READ_EDID  		TI816XHDMI_IOR(3, struct hdmi_edid_read_params)
/* Use this command only when hdmi is streaming video to a sink */
/* TODO Not supported for now */
#if 0
#define TI816XHDMI_GET_CONFIG 		TI816XHDMI_IOR(4, struct hdmi_cfg_params)
#define TI816XHDMI_SET_CONFIG 		TI816XHDMI_IOW(5, struct hdmi_cfg_params)
#endif
#define TI816XHDMI_SET_MODE         TI816XHDMI_IOW(6, enum hdmi_resolution)
#define TI816XHDMI_TEST_HDMI        TI816XHDMI_IOW(7, enum hdmi_resolution)
/* Data Types */

/* Supported resolutions */
enum hdmi_resolution {
	hdmi_ntsc_mode = 0,
	hdmi_pal_mode,
	hdmi_1080P_60_mode,
	hdmi_720P_60_mode,
	hdmi_1080I_60_mode,
	hdmi_1080P_30_mode,
	hdmi_max_mode
};
struct hdmi_edid_read_params {
	unsigned int slave_address;
	unsigned int segment_ptr;
	unsigned int offset;
	unsigned int no_of_bytes;
	void *buffer_ptr;
	unsigned int no_of_bytes_read;
	unsigned int timeout;
	unsigned int use_eddc_read;
};
#endif /* End of #ifndef __TI816X_HDMI_H__ */


