/*
 * include/linux//ti81xx_hdmi.c
 *
 * Copyright (C) 2009 Texas Instruments
 *
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * Author: Sujith Shivalingappa <sujith.s@ti.com>
 *       : Varada Bellary <varadab@ti.com>, modified for new version (Mar 2011) of driver.
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
#define TI81XXHDMI_GET_STATUS 		TI81XXHDMI_IOR(2, unsigned char *)
#define TI81XXHDMI_READ_EDID  		TI81XXHDMI_IOR(3, unsigned char *)
/*#define TI81XXHDMI_READ_EDID  		TI81XXHDMI_IOR(3, struct ti81xxdhmi_edid_params) */

/* Use this command only when hdmi is streaming video to a sink */
/* TODO Not supported for now */
#if 0
#define TI81XX_HDMI_SET_MODE         	TI81XX_HDMI_IOW(6, enum ti81xxhdmi_mode)
#define TI81XX_HDMI_GET_MODE         	TI81XX_HDMI_IOR(7, enum ti81xxhdmi_mode)
#endif


struct ti81xxhdmi_status {
	unsigned int is_hpd_detected;
	unsigned int is_hdmi_streaming;
};

#endif /* End of #ifndef __TI81XX_HDMI_H__ */

