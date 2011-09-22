#ifndef __ASM_ARCH_TI81XX_H
#define __ASM_ARCH_TI81XX_H

#include <linux/videodev2.h>

#define TVP7002_INST0 "tvp7002"

#if 0
enum hdvpss_if_type {
HDVPSS_IF_BT656,
HDVPSS_IF_BT1120,
HDVPSS_IF_RAW_BAYER
};
#endif

struct __intface {
#error
	u8 clk_polarity;
	u8 hs_polarity;
	u8 vs_polarity;
	u8 fid_polarity;
	u8 sog_polarity;
};

#if 1
struct hdvpss_subdev_info {
	const char *name;
	struct i2c_board_info board_info;
};


struct hdvpss_input {
	struct v4l2_input input;
	const char *subdev_name;
};


struct hdvpss_capture_inst_config {
	const struct hdvpss_input *inputs;
	int input_count;
};

struct ti81xxvin_config {
	struct hdvpss_capture_inst_config inst_config[HDVPSS_CAPTURE_MAX_CHANNELS];
	struct hdvpss_subdev_info *subdev_info;
	int subdev_count;
	const char *card_name;
};
#endif
#endif /* #ifndef __ASM_ARCH_TI81XX_H */
