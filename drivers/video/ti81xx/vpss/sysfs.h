/*
 * linux/drivers/video/ti81xx/vpss/sysfs.h
 *
 * Sysfs Header file for TI 81XX
 *
 * Copyright (C) 2011 TI
 * Author: Yihe Hu <yihehu@ti.com>
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

#ifndef TI81XX_VPSS_SYSFS_H
#define TI81XX_VPSS_SYSFS_H
/**********************************************************
  The name in the followig arrays are the value used in the sysfs.
**********************************************************/

/*used for mode sysfs*/
static const struct dc_vencmode_info vmode_info[] = {
	{"ntsc", FVID2_STD_NTSC,
	    {FVID2_STD_NTSC, 720, 480, FVID2_SF_INTERLACED,
		216000, 60, 12, 68, 64, 5, 41, 5}
	},
	{"pal", FVID2_STD_PAL,
	    {FVID2_STD_PAL, 720, 576, FVID2_SF_INTERLACED,
		216000, 50, 16, 58, 64, 6, 31, 6}
	},
	{"1080p-60", FVID2_STD_1080P_60,
	    {FVID2_STD_1080P_60, 1920, 1080, FVID2_SF_PROGRESSIVE,
		148500, 60, 88, 148, 44, 4, 36, 5}
	},
	{"1920x1080@60", FVID2_STD_1080P_60,
	    {FVID2_STD_CUSTOM, 1920, 1080, FVID2_SF_PROGRESSIVE,
		148500, 60, 88, 148, 44, 4, 36, 5}
	},
	{"1080p-50", FVID2_STD_1080P_50,
	    {FVID2_STD_1080P_50,  1920, 1080, FVID2_SF_PROGRESSIVE,
		148500, 50, 528, 148, 44, 4, 36, 5}
	},
	{"1920x1080@50", FVID2_STD_1080P_50,
	    {FVID2_STD_CUSTOM, 1920, 1080, FVID2_SF_PROGRESSIVE,
		148500, 50, 528, 148, 44, 4, 36, 5}
	},
	{"1080p-30", FVID2_STD_1080P_30,
	    {FVID2_STD_1080P_30, 1920, 1080, FVID2_SF_PROGRESSIVE,
		74250, 30, 88, 148, 44, 4, 36, 5}
	},
	{"1920x1080@30", FVID2_STD_1080P_30,
	    {FVID2_STD_CUSTOM, 1920, 1080, FVID2_SF_PROGRESSIVE,
		74250, 30, 88, 148, 44, 4, 36, 5}
	},
	{"720p-60", FVID2_STD_720P_60,
	    {FVID2_STD_720P_60, 1280, 720, FVID2_SF_PROGRESSIVE,
		74250, 60, 110, 220, 40, 5, 20, 5}
	},
	{"1280x720@60", FVID2_STD_720P_60,
	    {FVID2_STD_CUSTOM, 1280, 720, FVID2_SF_PROGRESSIVE,
		74250, 60, 110, 220, 40, 5, 20, 5}
	},
	{"720p-50", FVID2_STD_720P_50,
	    {FVID2_STD_720P_50, 1280, 720, FVID2_SF_PROGRESSIVE,
		74250, 50, 440, 220, 40, 5, 20, 5}
	},
	{"1280x720@50", FVID2_STD_720P_50,
	    { FVID2_STD_CUSTOM, 1280, 720, FVID2_SF_PROGRESSIVE,
		74250, 50, 440, 220, 40, 5, 20, 5}
	},
	{"1080i-60", FVID2_STD_1080I_60,
	    {FVID2_STD_1080I_60, 1920, 1080, FVID2_SF_INTERLACED,
		74250, 60, 88, 148, 44, 2, 15, 5}
	},
	{"1920x1080@60i", FVID2_STD_1080I_60,
	    {FVID2_STD_CUSTOM, 1920, 1080, FVID2_SF_INTERLACED,
		74250, 60, 88, 148, 44, 2, 15, 5}
	},
	{"1080i-50", FVID2_STD_1080I_50,
	    {FVID2_STD_1080I_50, 1920, 1080, FVID2_SF_INTERLACED,
		742500, 50, 528, 148, 44, 2, 15, 5}
	},
	{"1920x1080@50i", FVID2_STD_1080I_50,
	    {FVID2_STD_CUSTOM, 1920, 1080, FVID2_SF_INTERLACED,
		742500, 50, 528, 148, 44, 2, 15, 5}
	},
	/*VGA*/
	{"640x480@60", FVID2_STD_VGA_60,
	    {FVID2_STD_CUSTOM, 640, 480, FVID2_SF_PROGRESSIVE,
		25088, 60, 16, 48, 96, 10, 33, 2}
	},
	{"640x480@72", FVID2_STD_VGA_72,
	    {FVID2_STD_CUSTOM, 640, 480, FVID2_SF_PROGRESSIVE,
		31488, 72, 24, 128, 40, 9, 29, 2}
	},
	{"640x480@75", FVID2_STD_VGA_75,
	    {FVID2_STD_CUSTOM, 640, 480, FVID2_SF_PROGRESSIVE,
		31488, 75, 16, 120, 64, 1, 16, 3}
	},
	{"640x480@85", FVID2_STD_VGA_85,
	    {FVID2_STD_CUSTOM, 640, 480, FVID2_SF_PROGRESSIVE,
		35840, 85, 56, 80, 56, 1, 25, 3}
	},
	/*SVGA*/
	{"800x600@60", FVID2_STD_SVGA_60,
	    {FVID2_STD_CUSTOM, 800, 600, FVID2_SF_PROGRESSIVE,
		39936, 60, 40, 88, 128, 1, 23, 4}
	},
	{"800x600@72", FVID2_STD_SVGA_72,
	    {FVID2_STD_CUSTOM, 800, 600, FVID2_SF_PROGRESSIVE,
		49920, 72, 56, 64, 120, 37, 23, 6}
	},
	{"800x600@75", FVID2_STD_SVGA_75,
	    {FVID2_STD_CUSTOM, 800, 600, FVID2_SF_PROGRESSIVE,
		49400, 75, 16, 160, 80, 1, 21, 3}
	},
	{"800x600@85", FVID2_STD_SVGA_85,
	    {FVID2_STD_CUSTOM, 800, 600, FVID2_SF_PROGRESSIVE,
		56000, 85, 32, 152, 64, 1, 27, 3}
	},
	/*XGA*/
	{"1024x768@60", FVID2_STD_XGA_60,
	    {FVID2_STD_XGA_60, 1024, 768, FVID2_SF_PROGRESSIVE,
		65000, 60, 24, 160, 136, 3, 29, 6}
	},
	{"1024x768@70", FVID2_STD_XGA_70,
	    {FVID2_STD_CUSTOM, 1024, 768, FVID2_SF_PROGRESSIVE,
		74752, 70, 24, 144, 136, 3, 29, 6}
	},
	{"1024x768@75", FVID2_STD_XGA_75,
	    {FVID2_STD_XGA_75, 1024, 768, FVID2_SF_PROGRESSIVE,
		78720, 75, 16, 176, 96, 1, 28, 3}
	},
	{"1024x768@85", FVID2_STD_XGA_85,
	    {FVID2_STD_CUSTOM, 1024, 768, FVID2_SF_PROGRESSIVE,
		94464, 85, 48, 208, 96, 1, 36, 3}
	},
	/*SXGA*/
	{"1280x1024@60", FVID2_STD_SXGA_60,
	    {FVID2_STD_SXGA_60, 1280, 1024, FVID2_SF_PROGRESSIVE,
		108000, 60, 48, 248, 112, 1, 38, 3}
	},
	{"1280x1024@75", FVID2_STD_SXGA_75,
	    {FVID2_STD_SXGA_75, 1280, 1024, FVID2_SF_PROGRESSIVE,
		135000, 75, 16, 248, 144, 1, 38, 3}
	},
	{"1280x1024@85", FVID2_STD_SXGA_85,
	    {FVID2_STD_CUSTOM, 1280, 1024, FVID2_SF_PROGRESSIVE,
		157440, 85, 64, 224, 160, 1, 44, 3}
	},
	/*UXGA*/
	{"1600x1200@60", FVID2_STD_UXGA_60,
	    {FVID2_STD_UXGA_60, 1600, 1200, FVID2_SF_PROGRESSIVE,
		162000, 60, 64, 304, 192, 1, 46, 3}
	}

};
/*use for the venc name*/
static const struct dc_vencname_info venc_name[VPS_DC_MAX_VENC] = {
	{"hdmi", VPS_DC_VENC_HDMI, VPS_DC_HDMI_BLEND, HDMI},
	{"dvo2", VPS_DC_VENC_DVO2, VPS_DC_DVO2_BLEND, DVO2},
	{"sd", VPS_DC_VENC_SD, VPS_DC_SDVENC_BLEND, SDVENC},
	{"hdcomp", VPS_DC_VENC_HDCOMP, VPS_DC_HDCOMP_BLEND, HDCOMP}
};
/*use for pll sysfs*/
static const struct vps_sname_info pllvenc_name[] = {
	 {"rfclk", VPS_SYSTEM_VPLL_OUTPUT_VENC_RF},
	 {"dclk", VPS_SYSTEM_VPLL_OUTPUT_VENC_D},
	 {"aclk", VPS_SYSTEM_VPLL_OUTPUT_VENC_A}
};
/*used for clock source sysfs*/
static const struct vps_sname_info vclksrc_name[] =  {
	{"dclk", VPS_DC_CLKSRC_VENCD},
	{"dclkdiv2", VPS_DC_CLKSRC_VENCD_DIV2},
	{"dclkdiff", VPS_DC_CLKSRC_VENCD_DIV2_DIFF},
	{"aclk", VPS_DC_CLKSRC_VENCA},
	{"aclkdiv2", VPS_DC_CLKSRC_VENCA_DIV2},
	{"aclkdiff", VPS_DC_CLKSRC_VENCA_DIV2_DIFF}
};
/*used for output sysfs*/
static const struct vps_sname_info dfmt_name[VPS_DC_DVOFMT_MAX] = {
	{"single", VPS_DC_DVOFMT_SINGLECHAN},
	{"double", VPS_DC_DVOFMT_DOUBLECHAN},
	{"triple", VPS_DC_DVOFMT_TRIPLECHAN_EMBSYNC},
	{"triplediscrete", VPS_DC_DVOFMT_TRIPLECHAN_DISCSYNC},
	{"doublediscrete", VPS_DC_DVOFMT_DOUBLECHAN_DISCSYNC},

};
/*used for output sysfs*/
static const struct vps_sname_info afmt_name[VPS_DC_A_OUTPUT_MAX] = {
	{"composite", VPS_DC_A_OUTPUT_COMPOSITE},
	{"svideo", VPS_DC_A_OUTPUT_SVIDEO},
	{"component", VPS_DC_A_OUTPUT_COMPONENT},
};
/*used for output sysfs*/
static const struct vps_sname_info datafmt_name[] = {
	{"rgb888", FVID2_DF_RGB24_888},
	{"yuv444p", FVID2_DF_YUV444P},
	{"yuv422spuv", FVID2_DF_YUV422SP_UV},
};
/*used for nodes sysfs*/
static const struct vps_sname_info dc_nodes[] = {
	{"primary", VPS_DC_MAIN_INPUT_PATH},			/*0*/
	{"vcompmux", VPS_DC_VCOMP_MUX},			        /*1*/
	{"hdcompmux", VPS_DC_HDCOMP_MUX},			/*2*/
	{"sdmux", VPS_DC_SDVENC_MUX },			        /*3*/
	{"aux", VPS_DC_AUX_INPUT_PATH},				/*4*/
	{"bp0", VPS_DC_BP0_INPUT_PATH},				/*5*/
	{"bp1", VPS_DC_BP1_INPUT_PATH},				/*6*/
	{"dummy", VPS_DC_MAX_NODE_NUM},				/*7*/
	{"dummy", VPS_DC_MAX_NODE_NUM},				/*8*/
	{"dummy", VPS_DC_MAX_NODE_NUM},				/*9*/
	{"sec1", VPS_DC_SEC1_INPUT_PATH},				/*10*/
	{"dummy", VPS_DC_MAX_NODE_NUM},				/*11*/
	{"dummy", VPS_DC_MAX_NODE_NUM},				/*12*/
	{"dummy", VPS_DC_MAX_NODE_NUM},				/*13*/
	{"vcomp", VPS_DC_VCOMP},				/*14*/
	{"cigcons", VPS_DC_CIG_CONSTRAINED_OUTPUT},		/*15*/
	{"cigin", VPS_DC_CIG_PIP_INPUT},			/*16*/
	{"cigncons", VPS_DC_CIG_NON_CONSTRAINED_OUTPUT},	/*17*/
	{"cigout", VPS_DC_CIG_PIP_OUTPUT},			/*18*/
	{"grpx0", VPS_DC_GRPX0_INPUT_PATH},			/*19*/
	{"grpx1", VPS_DC_GRPX1_INPUT_PATH},			/*20*/
	{"grpx2", VPS_DC_GRPX2_INPUT_PATH},			/*21*/
	{"hdmi",  VPS_DC_HDMI_BLEND},				/*22*/
#ifdef CONFIG_ARCH_TI816X
	{"hdcomp", VPS_DC_HDCOMP_BLEND},			/*23*/
	{"dvo2", VPS_DC_DVO2_BLEND},				/*24*/
	{"sd", VPS_DC_SDVENC_BLEND},			        /*25*/
#else
	{"dvo2", VPS_DC_DVO2_BLEND},				/*23*/
	{"sd", VPS_DC_SDVENC_BLEND},			        /*24*/
#endif
};
#endif

