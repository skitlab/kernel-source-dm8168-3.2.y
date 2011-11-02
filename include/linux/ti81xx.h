#ifndef __TI81XX_H__
#define __TI81XX_H__

#ifdef __KERNEL__

#include <linux/videodev2.h>
#include <linux/vps_cfgVipParser.h>

#define TVP7002_INST0 "tvp7002"
#define TVP7002_INST1 "tvp7002"
#define TI81XXVIN_NUM_INSTANCES		4
#define TI81XXVIN_MAX_DV_PRESETS	8


enum ti81xxvin_if_type {
TI81XXVIN_IF_BT656,
TI81XXVIN_IF_BT1120,
TI81XXVIN_IF_RAW_BAYER
};


struct ti81xxvin_interface {
	u8 clk_polarity;
	u8 hs_polarity;
	u8 vs_polarity;
	u8 fid_polarity;
	u8 sog_polarity;
};

struct ti81xxvin_subdev_info {
	const char *name;
	struct i2c_board_info board_info;
	/* How the subdevice is connected to VIP Port
	   Like embedded sync, discrete sync, 8/16/24 bit mode etc.
	*/
	struct Vps_VipPortConfig vip_port_cfg;
	/* How VIP should be configured for the subdevice */
	struct Vps_VipConfig vip_cfg;

	u32 video_capture_mode;
	/* How is video captured, like Embedded Sync, discrete sync,
	   single channel etc For details see #Vps_CaptVideoCaptureMode
	 */
	u32 video_if_mode;
	/* How is decoder interfaced, 8bit, 16bit, 24 bit etc.
	   For details see #Vps_CaptVideoIfMode
	 */
	u32 input_data_format;
	/** Input source color data format, valid values are given below \n
	    FVID2_DF_YUV422P, ( 'P' is not relavent for input data format) \n
	    FVID2_DF_YUV444P, ( 'P' is not relavent for input data format) \n
	    FVID2_DF_RGB24_888.
	    For valid values see #FVID2_DataFormat.
	 */
};


struct ti81xxvin_input {
	struct v4l2_input input;
	const char *subdev_name;
	const struct v4l2_dv_preset *dv_presets;
	int num_dv_presets;
};


struct ti81xxvin_inst_config {
	const struct ti81xxvin_input *inputs;
	int input_count;
};

struct ti81xxvin_config {
	struct ti81xxvin_inst_config inst_config[TI81XXVIN_NUM_INSTANCES];
	struct ti81xxvin_subdev_info *subdev_info;
	int subdev_count;
	const char *card_name;
};
#endif /*  #ifdef __KERNEL__ */
#endif /* #ifndef __TI81XX_H__ */
