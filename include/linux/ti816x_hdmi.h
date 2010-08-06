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

#if 0
#include <linux/ioctl.h>
#include <linux/types.h>


/* IOCTL definitions */
#define TI816X_HDMI_SET_PARAM	_IOW('N', num, dtype)
#define TI816X_HDMI_GET_PARAM	_IOR('N', num, dtype)
#endif
/* IOCLT Supported by this driver */
#define IOCTL_HDMI_START      		(0)
#define IOCTL_HDMI_STOP       		(1)
#define IOCTL_HDMI_GET_STATUS 		(2)
#define IOCTL_HDMI_READ_EDID  		(3)
/* Use this command only when hdmi is streaming video to a sink */
#define IOCTL_HDMI_GET_CONFIG 		(4)
#define IOCTL_HDMI_SET_CONFIG 		(5)
#define IOCTL_HDMI_SET_MODE         (6)
#define IOCTL_HDMI_TEST_HDMI        (7)
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

#endif /* End of #ifndef __TI816X_HDMI_H__ */


