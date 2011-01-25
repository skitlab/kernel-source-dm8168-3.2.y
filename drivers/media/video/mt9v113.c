/*
 * drivers/media/video/mt9v113.c
 *
 * Based on TI TVP5146/47 decoder driver
 *
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/slab.h>

#include <media/v4l2-chip-ident.h>
#include <media/v4l2-device.h>
#include <media/v4l2-subdev.h>
#include <linux/v4l2-mediabus.h>

#include <media/mt9v113.h>

#include "mt9v113_regs.h"

/* Private macros for TVP */
#define I2C_RETRY_COUNT                 (5)

/* Debug functions */
static int debug = 1;
module_param(debug, bool, 0644);
MODULE_PARM_DESC(debug, "Debug level (0-1)");

/**
 * enum mt9v113_std - enum for supported standards
 */
enum mt9v113_std {
	MT9V113_STD_VGA = 0,
	MT9V113_STD_QVGA,
	MT9V113_STD_INVALID
};

/**
 * struct mt9v113_std_info - Structure to store standard informations
 * @width: Line width in pixels
 * @height:Number of active lines
 * @video_std: Value to write in REG_VIDEO_STD register
 * @standard: v4l2 standard structure information
 */
struct mt9v113_std_info {
	unsigned long width;
	unsigned long height;
	u8 video_std;
	struct v4l2_standard standard;
};

/**
 * struct mt9v113_decoded - decoder object
 * @pdata: Board specific
 * @subdev: v4l2_subdev associated data
 * @pad: media entity associated data
 * @ver: Chip version
 * @power: Current power state (0: off, 1: on)
 */
struct mt9v113_decoder {
	const struct mt9v113_platform_data *pdata;

	struct v4l2_subdev subdev;
	struct media_pad pad;
	struct v4l2_mbus_framefmt formats[2];
	struct v4l2_subdev_frame_interval frameival;

	int ver;
	unsigned int power:1;
};

#define to_mt9v113_decoder(sd)	container_of(sd, struct mt9v113_decoder, subdev)

/* MT9V113 register set for VGA mode */
static struct mt9v113_reg mt9v113_vga_reg[] = {
	{TOK_WRITE, 0x098C, 0x2739}, /* crop_X0_A */
	{TOK_WRITE, 0x0990, 0x0000}, /* val: 0 */
	{TOK_WRITE, 0x098C, 0x273B}, /* crop_X1_A */
	{TOK_WRITE, 0x0990, 0x027F}, /* val: 639 */
	{TOK_WRITE, 0x098C, 0x273D}, /* crop_Y0_A */
	{TOK_WRITE, 0x0990, 0x0000}, /* val: 0 */
	{TOK_WRITE, 0x098C, 0x273F}, /* crop_Y1_A */
	{TOK_WRITE, 0x0990, 0x01DF}, /* val: 479 */
	{TOK_WRITE, 0x098C, 0x2703}, /* output_width_A */
	{TOK_WRITE, 0x0990, 0x0280}, /* val: 640 */
	{TOK_WRITE, 0x098C, 0x2705}, /* out_height_A */
	{TOK_WRITE, 0x0990, 0x01E0}, /* val: 480 */
	{TOK_WRITE, 0x098C, 0xA103}, /* cmd */
	{TOK_WRITE, 0x0990, 0x0005}, /* val: 5 - Refresh */
	{TOK_DELAY, 0, 100},
	{TOK_TERM, 0, 0},
};

/* MT9V113 default register values */
static struct mt9v113_reg mt9v113_reg_list[] = {
	{TOK_WRITE, 0x0018, 0x4028},
	{TOK_DELAY, 0, 100},
	{TOK_WRITE, 0x001A, 0x0011},
	{TOK_WRITE, 0x001A, 0x0010},
	{TOK_WRITE, 0x0018, 0x4028},
	{TOK_DELAY, 0, 100},
	{TOK_WRITE, 0x098C, 0x02F0},
	{TOK_WRITE, 0x0990, 0x0000},
	{TOK_WRITE, 0x098C, 0x02F2},
	{TOK_WRITE, 0x0990, 0x0210},
	{TOK_WRITE, 0x098C, 0x02F4},
	{TOK_WRITE, 0x0990, 0x001A},
	{TOK_WRITE, 0x098C, 0x2145},
	{TOK_WRITE, 0x0990, 0x02F4},
	{TOK_WRITE, 0x098C, 0xA134},
	{TOK_WRITE, 0x0990, 0x0001},
	{TOK_WRITE, 0x31E0, 0x0001},
	{TOK_WRITE, 0x001A, 0x0210},
	{TOK_WRITE, 0x001E, 0x0777},
	{TOK_WRITE, 0x0016, 0x42DF},
	{TOK_WRITE, 0x0014, 0x2145},
	{TOK_WRITE, 0x0014, 0x2145},
	{TOK_WRITE, 0x0010, 0x0431},
	{TOK_WRITE, 0x0012, 0x0000},
	{TOK_WRITE, 0x0014, 0x244B},
	{TOK_WRITE, 0x0014, 0x304B},
	{TOK_DELAY, 0, 100},
	{TOK_WRITE, 0x0014, 0xB04A},
	{TOK_WRITE, 0x098C, 0xAB1F},
	{TOK_WRITE, 0x0990, 0x00C7},
	{TOK_WRITE, 0x098C, 0xAB31},
	{TOK_WRITE, 0x0990, 0x001E},
	{TOK_WRITE, 0x098C, 0x274F},
	{TOK_WRITE, 0x0990, 0x0004},
	{TOK_WRITE, 0x098C, 0x2741},
	{TOK_WRITE, 0x0990, 0x0004},
	{TOK_WRITE, 0x098C, 0xAB20},
	{TOK_WRITE, 0x0990, 0x0054},
	{TOK_WRITE, 0x098C, 0xAB21},
	{TOK_WRITE, 0x0990, 0x0046},
	{TOK_WRITE, 0x098C, 0xAB22},
	{TOK_WRITE, 0x0990, 0x0002},
	{TOK_WRITE, 0x098C, 0xAB24},
	{TOK_WRITE, 0x0990, 0x0005},
	{TOK_WRITE, 0x098C, 0x2B28},
	{TOK_WRITE, 0x0990, 0x170C},
	{TOK_WRITE, 0x098C, 0x2B2A},
	{TOK_WRITE, 0x0990, 0x3E80},
	{TOK_WRITE, 0x3210, 0x09A8},
	{TOK_WRITE, 0x098C, 0x2306},
	{TOK_WRITE, 0x0990, 0x0315},
	{TOK_WRITE, 0x098C, 0x2308},
	{TOK_WRITE, 0x0990, 0xFDDC},
	{TOK_WRITE, 0x098C, 0x230A},
	{TOK_WRITE, 0x0990, 0x003A},
	{TOK_WRITE, 0x098C, 0x230C},
	{TOK_WRITE, 0x0990, 0xFF58},
	{TOK_WRITE, 0x098C, 0x230E},
	{TOK_WRITE, 0x0990, 0x02B7},
	{TOK_WRITE, 0x098C, 0x2310},
	{TOK_WRITE, 0x0990, 0xFF31},
	{TOK_WRITE, 0x098C, 0x2312},
	{TOK_WRITE, 0x0990, 0xFF4C},
	{TOK_WRITE, 0x098C, 0x2314},
	{TOK_WRITE, 0x0990, 0xFE4C},
	{TOK_WRITE, 0x098C, 0x2316},
	{TOK_WRITE, 0x0990, 0x039E},
	{TOK_WRITE, 0x098C, 0x2318},
	{TOK_WRITE, 0x0990, 0x001C},
	{TOK_WRITE, 0x098C, 0x231A},
	{TOK_WRITE, 0x0990, 0x0039},
	{TOK_WRITE, 0x098C, 0x231C},
	{TOK_WRITE, 0x0990, 0x007F},
	{TOK_WRITE, 0x098C, 0x231E},
	{TOK_WRITE, 0x0990, 0xFF77},
	{TOK_WRITE, 0x098C, 0x2320},
	{TOK_WRITE, 0x0990, 0x000A},
	{TOK_WRITE, 0x098C, 0x2322},
	{TOK_WRITE, 0x0990, 0x0020},
	{TOK_WRITE, 0x098C, 0x2324},
	{TOK_WRITE, 0x0990, 0x001B},
	{TOK_WRITE, 0x098C, 0x2326},
	{TOK_WRITE, 0x0990, 0xFFC6},
	{TOK_WRITE, 0x098C, 0x2328},
	{TOK_WRITE, 0x0990, 0x0086},
	{TOK_WRITE, 0x098C, 0x232A},
	{TOK_WRITE, 0x0990, 0x00B5},
	{TOK_WRITE, 0x098C, 0x232C},
	{TOK_WRITE, 0x0990, 0xFEC3},
	{TOK_WRITE, 0x098C, 0x232E},
	{TOK_WRITE, 0x0990, 0x0001},
	{TOK_WRITE, 0x098C, 0x2330},
	{TOK_WRITE, 0x0990, 0xFFEF},
	{TOK_WRITE, 0x098C, 0xA348},
	{TOK_WRITE, 0x0990, 0x0008},
	{TOK_WRITE, 0x098C, 0xA349},
	{TOK_WRITE, 0x0990, 0x0002},
	{TOK_WRITE, 0x098C, 0xA34A},
	{TOK_WRITE, 0x0990, 0x0090},
	{TOK_WRITE, 0x098C, 0xA34B},
	{TOK_WRITE, 0x0990, 0x00FF},
	{TOK_WRITE, 0x098C, 0xA34C},
	{TOK_WRITE, 0x0990, 0x0075},
	{TOK_WRITE, 0x098C, 0xA34D},
	{TOK_WRITE, 0x0990, 0x00EF},
	{TOK_WRITE, 0x098C, 0xA351},
	{TOK_WRITE, 0x0990, 0x0000},
	{TOK_WRITE, 0x098C, 0xA352},
	{TOK_WRITE, 0x0990, 0x007F},
	{TOK_WRITE, 0x098C, 0xA354},
	{TOK_WRITE, 0x0990, 0x0043},
	{TOK_WRITE, 0x098C, 0xA355},
	{TOK_WRITE, 0x0990, 0x0001},
	{TOK_WRITE, 0x098C, 0xA35D},
	{TOK_WRITE, 0x0990, 0x0078},
	{TOK_WRITE, 0x098C, 0xA35E},
	{TOK_WRITE, 0x0990, 0x0086},
	{TOK_WRITE, 0x098C, 0xA35F},
	{TOK_WRITE, 0x0990, 0x007E},
	{TOK_WRITE, 0x098C, 0xA360},
	{TOK_WRITE, 0x0990, 0x0082},
	{TOK_WRITE, 0x098C, 0x2361},
	{TOK_WRITE, 0x0990, 0x0040},
	{TOK_WRITE, 0x098C, 0xA363},
	{TOK_WRITE, 0x0990, 0x00D2},
	{TOK_WRITE, 0x098C, 0xA364},
	{TOK_WRITE, 0x0990, 0x00F6},
	{TOK_WRITE, 0x098C, 0xA302},
	{TOK_WRITE, 0x0990, 0x0000},
	{TOK_WRITE, 0x098C, 0xA303},
	{TOK_WRITE, 0x0990, 0x00EF},
	{TOK_WRITE, 0x098C, 0xAB20},
	{TOK_WRITE, 0x0990, 0x0024},
	{TOK_WRITE, 0x098C, 0xA103},
	{TOK_WRITE, 0x0990, 0x0006},
	{TOK_DELAY, 0, 100},
	{TOK_WRITE, 0x098C, 0xA103},
	{TOK_WRITE, 0x0990, 0x0005},
	{TOK_DELAY, 0, 100},
	{TOK_WRITE, 0x098C, 0x222D},
	{TOK_WRITE, 0x0990, 0x0088},
	{TOK_WRITE, 0x098C, 0xA408},
	{TOK_WRITE, 0x0990, 0x0020},
	{TOK_WRITE, 0x098C, 0xA409},
	{TOK_WRITE, 0x0990, 0x0023},
	{TOK_WRITE, 0x098C, 0xA40A},
	{TOK_WRITE, 0x0990, 0x0027},
	{TOK_WRITE, 0x098C, 0xA40B},
	{TOK_WRITE, 0x0990, 0x002A},
	{TOK_WRITE, 0x098C, 0x2411},
	{TOK_WRITE, 0x0990, 0x0088},
	{TOK_WRITE, 0x098C, 0x2413},
	{TOK_WRITE, 0x0990, 0x00A4},
	{TOK_WRITE, 0x098C, 0x2415},
	{TOK_WRITE, 0x0990, 0x0088},
	{TOK_WRITE, 0x098C, 0x2417},
	{TOK_WRITE, 0x0990, 0x00A4},
	{TOK_WRITE, 0x098C, 0xA404},
	{TOK_WRITE, 0x0990, 0x0010},
	{TOK_WRITE, 0x098C, 0xA40D},
	{TOK_WRITE, 0x0990, 0x0002},
	{TOK_WRITE, 0x098C, 0xA40E},
	{TOK_WRITE, 0x0990, 0x0003},
	{TOK_WRITE, 0x098C, 0xA103},
	{TOK_WRITE, 0x0990, 0x0006},
	{TOK_DELAY, 0, 100},
	/* test pattern all white*/
	/* {TOK_WRITE, 0x098C, 0xA766},
	{TOK_WRITE, 0x0990, 0x0001},
	*/
	{TOK_WRITE, 0x098C, 0xA103},
	{TOK_WRITE, 0x0990, 0x0005},
	{TOK_DELAY, 0, 100},
	{TOK_TERM, 0, 0},
};

/* List of image formats supported by mt9v113
 * Currently we are using 8 bit mode only, but can be
 * extended to 10/20 bit mode.
 */
static const struct v4l2_fmtdesc mt9v113_fmt_list[] = {
	{
		.index = 0,
		.type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
		.flags = 0,
		.description = "8-bit UYVY 4:2:2 Format",
		.pixelformat = V4L2_PIX_FMT_UYVY,
	},
};

/*
 * Supported standards -
 *
 * Currently supports two standards only, need to add support for rest of the
 * modes, like SECAM, etc...
 */
static struct mt9v113_std_info mt9v113_std_list[] = {
	/* Standard: STD_NTSC_MJ */
	[MT9V113_STD_VGA] = {
		.width = VGA_NUM_ACTIVE_PIXELS,
		.height = VGA_NUM_ACTIVE_LINES,
		.video_std = MT9V113_IMAGE_STD_VGA,
		.standard = {
			.index = 0,
			.id = MT9V113_IMAGE_STD_VGA,
			.name = "VGA",
			.frameperiod = {1001, 30000},
			.framelines = 480
		},
	},
	/* Standard: STD_PAL_BDGHIN */
	[MT9V113_STD_QVGA] = {
		.width = QVGA_NUM_ACTIVE_PIXELS,
		.height = QVGA_NUM_ACTIVE_LINES,
		.video_std = MT9V113_IMAGE_STD_QVGA,
		.standard = {
			.index = 1,
			.id = MT9V113_IMAGE_STD_QVGA,
			.name = "QVGA",
			.frameperiod = {1001, 30000},
			.framelines = 320
		},
	},
	/* Standard: need to add for additional standard */
};
/*
 * Control structure for Auto Gain
 *     This is temporary data, will get replaced once
 *     v4l2_ctrl_query_fill supports it.
 */
static const struct v4l2_queryctrl mt9v113_autogain_ctrl = {
	.id = V4L2_CID_AUTOGAIN,
	.name = "Gain, Automatic",
	.type = V4L2_CTRL_TYPE_BOOLEAN,
	.minimum = 0,
	.maximum = 1,
	.step = 1,
	.default_value = 1,
};

static int mt9v113_read_reg(struct i2c_client *client, unsigned short reg)
{
	int err = 0;
	struct i2c_msg msg[1];
	unsigned char data[2];
	unsigned short val = 0;

	if (!client->adapter)
		return -ENODEV;

	msg->addr = client->addr;
	msg->flags = 0;
	msg->len = I2C_TWO_BYTE_TRANSFER;
	msg->buf = data;
	data[0] = (reg & I2C_TXRX_DATA_MASK_UPPER) >> I2C_TXRX_DATA_SHIFT;
	data[1] = (reg & I2C_TXRX_DATA_MASK);
	err = i2c_transfer(client->adapter, msg, 1);
	if (err >= 0) {
		msg->flags = I2C_M_RD;
		msg->len = I2C_TWO_BYTE_TRANSFER;	/* 2 byte read */
		err = i2c_transfer(client->adapter, msg, 1);
		if (err >= 0) {
			val = ((data[0] & I2C_TXRX_DATA_MASK)
			       << I2C_TXRX_DATA_SHIFT)
			      | (data[1] & I2C_TXRX_DATA_MASK);
		}
	}

	return (int)(0xffff & val);
}



static int mt9v113_write_reg(struct i2c_client *client, unsigned short reg,
			     unsigned short val)
{
	int err = -1; /* FIXME: To enter below loop, err has to be negative */
	int trycnt = 0;
	struct i2c_msg msg[1];
	unsigned char data[4];

	v4l_dbg(1, debug, client,
		 "mt9v113_write_reg reg=0x%x, val=0x%x\n",
		 reg, val);

	if (!client->adapter)
		return -ENODEV;

	while ((err < 0) && (trycnt < I2C_RETRY_COUNT)) {
		trycnt++;
		msg->addr = client->addr;
		msg->flags = 0;
		msg->len = I2C_FOUR_BYTE_TRANSFER;
		msg->buf = data;
		data[0] = (reg & I2C_TXRX_DATA_MASK_UPPER) >>
		    I2C_TXRX_DATA_SHIFT;
		data[1] = (reg & I2C_TXRX_DATA_MASK);
		data[2] = (val & I2C_TXRX_DATA_MASK_UPPER) >>
		    I2C_TXRX_DATA_SHIFT;
		data[3] = (val & I2C_TXRX_DATA_MASK);
		err = i2c_transfer(client->adapter, msg, 1);
	}
	if (err < 0)
		printk(KERN_INFO "\n I2C write failed");

	return err;
}

/*
 * mt9v113_write_regs : Initializes a list of registers
 *		if token is TOK_TERM, then entire write operation terminates
 *		if token is TOK_DELAY, then a delay of 'val' msec is introduced
 *		if token is TOK_SKIP, then the register write is skipped
 *		if token is TOK_WRITE, then the register write is performed
 *
 * reglist - list of registers to be written
 * Returns zero if successful, or non-zero otherwise.
 */
static int mt9v113_write_regs(struct i2c_client *client,
			      const struct mt9v113_reg reglist[])
{
	int err;
	const struct mt9v113_reg *next = reglist;

	for (; next->token != TOK_TERM; next++) {
		if (next->token == TOK_DELAY) {
			msleep(next->val);
			continue;
		}

		if (next->token == TOK_SKIP)
			continue;

		err = mt9v113_write_reg(client, next->reg, next->val);
		if (err < 0) {
			v4l_err(client, "Write failed. Err[%d]\n", err);
			return err;
		}
	}
	return 0;
}

/*
 * Configure the mt9v113 with the current register settings
 * Returns zero if successful, or non-zero otherwise.
 */
static int mt9v113_configure(struct v4l2_subdev *subdev)
{
	struct i2c_client *client = v4l2_get_subdevdata(subdev);
	int err;

	/* common register initialization */
	err =
	    mt9v113_write_regs(client, mt9v113_reg_list);
	if (err)
		return err;

	return 0;
}

/*
 * Configure the MT9V113 to VGA mode
 * Returns zero if successful, or non-zero otherwise.
 */
static int mt9v113_vga_mode(struct v4l2_subdev *subdev)
{
	struct i2c_client *client = v4l2_get_subdevdata(subdev);
	int err;

	err =
	    mt9v113_write_regs(client, mt9v113_vga_reg);
	if (err)
		return err;

	return 0;
}

/*
 * Detect if an mt9v113 is present, and if so which revision.
 * A device is considered to be detected if the chip ID (LSB and MSB)
 * registers match the expected values.
 * Any value of the rom version register is accepted.
 * Returns ENODEV error number if no device is detected, or zero
 * if a device is detected.
 */
static int mt9v113_detect(struct v4l2_subdev *subdev)
{
	struct mt9v113_decoder *decoder = to_mt9v113_decoder(subdev);
	struct i2c_client *client = v4l2_get_subdevdata(subdev);
	unsigned short val = 0;

	val = mt9v113_read_reg(client, REG_CHIP_ID);

	v4l_dbg(1, debug, client, "chip id detected 0x%x\n", val);

	if (MT9V113_CHIP_ID != val) {
		/* We didn't read the values we expected, so this must not be
		 * MT9V113.
		 */
		v4l_err(client,
			"chip id mismatch read 0x%x, expecting 0x%x\n", val,
			MT9V113_CHIP_ID);
		return -ENODEV;
	}

	decoder->ver = val;

	v4l_info(client,
			"%s found at 0x%x (%s)\n", client->name,
			client->addr << 1,
			client->adapter->name);

	return 0;
}

/**
 * mt9v113_queryctrl - V4L2 decoder interface handler for VIDIOC_QUERYCTRL ioctl
 * @s: pointer to standard V4L2 device structure
 * @qctrl: standard V4L2 v4l2_queryctrl structure
 *
 * If the requested control is supported, returns the control information.
 * Otherwise, returns -EINVAL if the control is not supported.
 */
static int mt9v113_queryctrl(struct v4l2_subdev *subdev,
			     struct v4l2_queryctrl *qctrl)
{
	struct i2c_client *client = v4l2_get_subdevdata(subdev);
	int err = -EINVAL;

	if (qctrl == NULL)
		return err;

	switch (qctrl->id) {
	case V4L2_CID_BRIGHTNESS:
		/* Brightness supported is same as standard one (0-255),
		 * so make use of standard API provided.
		 */
		err = v4l2_ctrl_query_fill(qctrl, 0, 255, 1, 128);
		break;
	case V4L2_CID_CONTRAST:
	case V4L2_CID_SATURATION:
		/* Saturation and Contrast supported is -
		 *	Contrast: 0 - 255 (Default - 128)
		 *	Saturation: 0 - 255 (Default - 128)
		 */
		err = v4l2_ctrl_query_fill(qctrl, 0, 255, 1, 128);
		break;
	case V4L2_CID_HUE:
		/* Hue Supported is -
		 *	Hue - -180 - +180 (Default - 0, Step - +180)
		 */
		err = v4l2_ctrl_query_fill(qctrl, -180, 180, 180, 0);
		break;
	case V4L2_CID_AUTOGAIN:
		/* Autogain is either 0 or 1*/
		memcpy(qctrl, &mt9v113_autogain_ctrl,
				sizeof(struct v4l2_queryctrl));
		err = 0;
		break;
	default:
		v4l_err(client, "invalid control id %d\n", qctrl->id);
		return err;
	}

	v4l_dbg(1, debug, client,
			"Query Control: %s : Min - %d, Max - %d, Def - %d",
			qctrl->name,
			qctrl->minimum,
			qctrl->maximum,
			qctrl->default_value);

	return err;
}

/**
 * mt9v113_g_ctrl - V4L2 decoder interface handler for VIDIOC_G_CTRL ioctl
 * @s: pointer to standard V4L2 device structure
 * @ctrl: pointer to v4l2_control structure
 *
 * If the requested control is supported, returns the control's current
 * value from the decoder. Otherwise, returns -EINVAL if the control is not
 * supported.
 */
static int mt9v113_g_ctrl(struct v4l2_subdev *subdev,
			  struct v4l2_control *ctrl)
{
	struct i2c_client *client = v4l2_get_subdevdata(subdev);

	if (ctrl == NULL)
		return -EINVAL;

	switch (ctrl->id) {
	case V4L2_CID_BRIGHTNESS:
		ctrl->value = mt9v113_reg_list[REG_BRIGHTNESS].val;
		break;
	case V4L2_CID_CONTRAST:
		ctrl->value = mt9v113_reg_list[REG_CONTRAST].val;
		break;
	case V4L2_CID_SATURATION:
		ctrl->value = mt9v113_reg_list[REG_SATURATION].val;
		break;
	case V4L2_CID_HUE:
		ctrl->value = mt9v113_reg_list[REG_HUE].val;
		if (ctrl->value == 0x7F)
			ctrl->value = 180;
		else if (ctrl->value == 0x80)
			ctrl->value = -180;
		else
			ctrl->value = 0;

		break;
	case V4L2_CID_AUTOGAIN:
		ctrl->value = mt9v113_reg_list[REG_AFE_GAIN_CTRL].val;
		if ((ctrl->value & 0x3) == 3)
			ctrl->value = 1;
		else
			ctrl->value = 0;

		break;
	default:
		v4l_err(client, "invalid control id %d\n", ctrl->id);
		return -EINVAL;
	}

	v4l_dbg(1, debug, client,
			"Get Control: ID - %d - %d",
			ctrl->id, ctrl->value);
	return 0;
}

/**
 * mt9v113_s_ctrl - V4L2 decoder interface handler for VIDIOC_S_CTRL ioctl
 * @s: pointer to standard V4L2 device structure
 * @ctrl: pointer to v4l2_control structure
 *
 * If the requested control is supported, sets the control's current
 * value in HW. Otherwise, returns -EINVAL if the control is not supported.
 */
static int mt9v113_s_ctrl(struct v4l2_subdev *subdev,
			  struct v4l2_control *ctrl)
{
	struct i2c_client *client = v4l2_get_subdevdata(subdev);
	int err = -EINVAL, value;

	if (ctrl == NULL)
		return err;

	value = (__s32) ctrl->value;

	switch (ctrl->id) {
	case V4L2_CID_BRIGHTNESS:
		if (ctrl->value < 0 || ctrl->value > 255) {
			v4l_err(client, "invalid brightness setting %d\n",
					ctrl->value);
			return -ERANGE;
		}
		err = mt9v113_write_reg(client, REG_BRIGHTNESS,
				value);
		if (err)
			return err;
		mt9v113_reg_list[REG_BRIGHTNESS].val = value;
		break;
	case V4L2_CID_CONTRAST:
		if (ctrl->value < 0 || ctrl->value > 255) {
			v4l_err(client, "invalid contrast setting %d\n",
					ctrl->value);
			return -ERANGE;
		}
		err = mt9v113_write_reg(client, REG_CONTRAST,
				value);
		if (err)
			return err;
		mt9v113_reg_list[REG_CONTRAST].val = value;
		break;
	case V4L2_CID_SATURATION:
		if (ctrl->value < 0 || ctrl->value > 255) {
			v4l_err(client, "invalid saturation setting %d\n",
					ctrl->value);
			return -ERANGE;
		}
		err = mt9v113_write_reg(client, REG_SATURATION,
				value);
		if (err)
			return err;
		mt9v113_reg_list[REG_SATURATION].val = value;
		break;
	case V4L2_CID_HUE:
		if (value == 180)
			value = 0x7F;
		else if (value == -180)
			value = 0x80;
		else if (value == 0)
			value = 0;
		else {
			v4l_err(client, "invalid hue setting %d\n",
					ctrl->value);
			return -ERANGE;
		}
		err = mt9v113_write_reg(client, REG_HUE,
				value);
		if (err)
			return err;
		mt9v113_reg_list[REG_HUE].val = value;
		break;
	case V4L2_CID_AUTOGAIN:
		if (value == 1)
			value = 0x0F;
		else if (value == 0)
			value = 0x0C;
		else {
			v4l_err(client, "invalid auto gain setting %d\n",
					ctrl->value);
			return -ERANGE;
		}
		err = mt9v113_write_reg(client, REG_AFE_GAIN_CTRL,
				value);
		if (err)
			return err;
		mt9v113_reg_list[REG_AFE_GAIN_CTRL].val = value;
		break;
	default:
		v4l_err(client, "invalid control id %d\n", ctrl->id);
		return err;
	}

	v4l_dbg(1, debug, client,
			"Set Control: ID - %d - %d",
			ctrl->id, ctrl->value);

	return err;
}

static int mt9v113_enum_mbus_code(struct v4l2_subdev *subdev,
				  struct v4l2_subdev_fh *fh,
				  struct v4l2_subdev_mbus_code_enum *code)
{
	u32 pad = code->pad;
	u32 index = code->index;

	memset(code, 0, sizeof(*code));
	code->index = index;
	code->pad = pad;

	if (index != 0)
		return -EINVAL;

	code->code = V4L2_MBUS_FMT_UYVY8_2X8;
	return 0;
}

static int mt9v113_get_pad_format(struct v4l2_subdev *subdev,
				  struct v4l2_subdev_fh *fh,
				  struct v4l2_subdev_format *fmt)
{
	struct mt9v113_decoder *decoder = to_mt9v113_decoder(subdev);

	fmt->format = decoder->formats[fmt->which];
	return 0;
}

static int mt9v113_set_pad_format(struct v4l2_subdev *subdev,
				  struct v4l2_subdev_fh *fh,
				  struct v4l2_subdev_format *fmt)
{
	struct mt9v113_decoder *decoder = to_mt9v113_decoder(subdev);

	if (fmt->format.code != V4L2_MBUS_FMT_UYVY8_2X8)
		return -EINVAL;

	/* FIXME: Only VGA? */
	fmt->format.width = mt9v113_std_list[MT9V113_STD_VGA].width;
	fmt->format.height = mt9v113_std_list[MT9V113_STD_VGA].height;

	fmt->format.field = V4L2_FIELD_NONE;
	fmt->format.colorspace = V4L2_COLORSPACE_JPEG;

	decoder->formats[fmt->which] = fmt->format;

	return 0;
}

static int mt9v113_g_frame_interval(struct v4l2_subdev *subdev,
				    struct v4l2_subdev_frame_interval *fi)
{
	struct mt9v113_decoder *decoder = to_mt9v113_decoder(subdev);

	memset(fi, 0, sizeof(*fi));
	*fi = decoder->frameival;

	return 0;
}

static int mt9v113_s_frame_interval(struct v4l2_subdev *subdev,
				    struct v4l2_subdev_frame_interval *fi)
{
	struct mt9v113_decoder *decoder = to_mt9v113_decoder(subdev);
	int err = 0;

	/*
	 * FIXME: "Functionality" inherited from old codebase
	 * (crappy hardcoding to force a framerate...)
	 */
	fi->interval = mt9v113_std_list[MT9V113_STD_VGA].standard.frameperiod;
	decoder->frameival = *fi;

	return err;
}

static int mt9v113_dev_init(struct v4l2_subdev *subdev)
{
	struct mt9v113_decoder *decoder = to_mt9v113_decoder(subdev);
	int rval;

	rval = decoder->pdata->s_power(subdev, 1);
	if (rval)
		return rval;

	rval = mt9v113_detect(subdev);

	decoder->pdata->s_power(subdev, 0);

	return rval;
}

static int mt9v113_enum_frame_size(struct v4l2_subdev *subdev,
				   struct v4l2_subdev_fh *fh,
				   struct v4l2_subdev_frame_size_enum *fse)
{
	/* Is requested pixelformat not found on sensor? */
	if (fse->code != V4L2_MBUS_FMT_UYVY8_2X8)
		return -EINVAL;

	/* Check that the index we are being asked for is not
	   out of bounds. */
	/* FIXME: Only VGA? */
	if (fse->index != 0)
		return -EINVAL;

	fse->min_width = mt9v113_std_list[MT9V113_STD_VGA].width;
	fse->min_height = mt9v113_std_list[MT9V113_STD_VGA].height;

	/* No ranges for both dimensions */
	fse->max_width = fse->min_width;
	fse->max_height = fse->min_height;

	return 0;
}

static int mt9v113_enum_frame_interval(struct v4l2_subdev *subdev,
				   struct v4l2_subdev_fh *fh,
				   struct v4l2_subdev_frame_interval_enum *fie)
{
	/* Check that the requested format is one we support */
	if (fie->code != V4L2_MBUS_FMT_UYVY8_2X8)
		return -EINVAL;

	/* Check that the index we are being asked for is not
	   out of bounds. */
	/* FIXME: Only 29.997 fps? */
	if (fie->index != 0)
		return -EINVAL;

	fie->interval = mt9v113_std_list[MT9V113_STD_VGA].standard.frameperiod;
	return 0;
}

static int mt9v113_s_stream(struct v4l2_subdev *subdev, int streaming)
{
	/*
	 * FIXME: We should put here the specific reg setting to turn on
	 * streaming in sensor.
	 */
	return 0;
}

/* --------------------------------------------------------------------------
 * V4L2 subdev core operations
 */
static int mt9v113_g_chip_ident(struct v4l2_subdev *subdev,
			       struct v4l2_dbg_chip_ident *chip)
{
	struct i2c_client *client = v4l2_get_subdevdata(subdev);

	return v4l2_chip_ident_i2c_client(client, chip, V4L2_IDENT_MT9V113, 0);
}

static int mt9v113_s_config(struct v4l2_subdev *subdev, int irq,
			   void *platform_data)
{
	struct mt9v113_decoder *decoder = to_mt9v113_decoder(subdev);
	int rval;

	if (platform_data == NULL)
		return -ENODEV;

	decoder->pdata = platform_data;

	rval = mt9v113_dev_init(subdev);
	if (rval)
		return rval;

	decoder->formats[V4L2_SUBDEV_FORMAT_ACTIVE].width =
				VGA_NUM_ACTIVE_PIXELS;
	decoder->formats[V4L2_SUBDEV_FORMAT_ACTIVE].height =
				VGA_NUM_ACTIVE_LINES;
	decoder->formats[V4L2_SUBDEV_FORMAT_ACTIVE].code =
				V4L2_MBUS_FMT_UYVY8_2X8;

	decoder->frameival.interval =
			mt9v113_std_list[MT9V113_STD_VGA].standard.frameperiod;
	return 0;
}

/**
 * mt9v113_s_power - Set power function
 * @subdev: pointer to standard V4L2 subdev structure
 * @on: power state to which device is to be set
 *
 * Sets devices power state to requested state, if possible.
 */
static int mt9v113_s_power(struct v4l2_subdev *subdev, int on)
{
	struct mt9v113_decoder *decoder = to_mt9v113_decoder(subdev);
	struct i2c_client *client = v4l2_get_subdevdata(subdev);
	int rval;

	if (on) {
		rval = decoder->pdata->s_power(subdev, 1);
		if (rval)
			goto out;
		rval = mt9v113_configure(subdev);
		if (rval) {
			decoder->pdata->s_power(subdev, 0);
			goto out;
		}
		rval = mt9v113_vga_mode(subdev);
		if (rval) {
			decoder->pdata->s_power(subdev, 0);
			goto out;
		}
	} else {
		rval = decoder->pdata->s_power(subdev, 0);
		if (rval)
			goto out;
	}

	decoder->power = on;
out:
	if (rval < 0)
		v4l_err(client, "Unable to set target power state\n");

	return rval;
}

static const struct v4l2_subdev_video_ops mt9v113_video_ops = {
	.s_stream = mt9v113_s_stream,
	.g_frame_interval = mt9v113_g_frame_interval,
	.s_frame_interval = mt9v113_s_frame_interval,
};

static const struct v4l2_subdev_core_ops mt9v113_core_ops = {
	.g_chip_ident = mt9v113_g_chip_ident,
	.s_config = mt9v113_s_config,
	.queryctrl = mt9v113_queryctrl,
	.g_ctrl = mt9v113_g_ctrl,
	.s_ctrl = mt9v113_s_ctrl,
	.s_power = mt9v113_s_power,
};

static const struct v4l2_subdev_pad_ops mt9v113_pad_ops = {
	.enum_mbus_code = mt9v113_enum_mbus_code,
	.enum_frame_size = mt9v113_enum_frame_size,
	.enum_frame_interval = mt9v113_enum_frame_interval,
	.get_fmt = mt9v113_get_pad_format,
	.set_fmt = mt9v113_set_pad_format,
};

static const struct v4l2_subdev_ops mt9v113_ops = {
	.core = &mt9v113_core_ops,
	.video = &mt9v113_video_ops,
	.pad = &mt9v113_pad_ops,
};

static const struct media_entity_operations mt9v113_entity_ops = {
	.set_power = v4l2_subdev_set_power,
};

/**
 * mt9v113_probe - decoder driver i2c probe handler
 * @client: i2c driver client device structure
 *
 * Register decoder as an i2c client device and V4L2
 * device.
 */
static int mt9v113_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	struct mt9v113_decoder *decoder;
	int ret;

	/* Check if the adapter supports the needed features */
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -EIO;

	if (!client->dev.platform_data) {
		v4l_err(client, "No platform data!!\n");
		return -ENODEV;
	}

	decoder = kzalloc(sizeof(*decoder), GFP_KERNEL);
	if (decoder == NULL)
		return -ENOMEM;

	v4l2_i2c_subdev_init(&decoder->subdev, client, &mt9v113_ops);

	decoder->pad.flags = MEDIA_PAD_FLAG_OUTPUT;
	decoder->subdev.entity.ops = &mt9v113_entity_ops;
	ret = media_entity_init(&decoder->subdev.entity, 1, &decoder->pad, 0);
	if (ret < 0)
		kfree(decoder);

	return ret;
}

/**
 * mt9v113_remove - decoder driver i2c remove handler
 * @client: i2c driver client device structure
 *
 * Unregister decoder as an i2c client device and V4L2
 * device. Complement of mt9v113_probe().
 */
static int __exit mt9v113_remove(struct i2c_client *client)
{
	struct v4l2_subdev *subdev = i2c_get_clientdata(client);
	struct mt9v113_decoder *decoder = to_mt9v113_decoder(subdev);

	v4l2_device_unregister_subdev(&decoder->subdev);
	media_entity_cleanup(&decoder->subdev.entity);
	kfree(decoder);
	return 0;
}

static const struct i2c_device_id mt9v113_id[] = {
	{ MT9V113_MODULE_NAME, 0 },
	{ },
};

MODULE_DEVICE_TABLE(i2c, mt9v113_id);

static struct i2c_driver mt9v113_i2c_driver = {
	.driver = {
		.name = MT9V113_MODULE_NAME,
		.owner = THIS_MODULE,
	},
	.probe = mt9v113_probe,
	.remove = __exit_p(mt9v113_remove),
	.id_table = mt9v113_id,
};

static int __init mt9v113_init(void)
{
	return i2c_add_driver(&mt9v113_i2c_driver);
}

static void __exit mt9v113_cleanup(void)
{
	i2c_del_driver(&mt9v113_i2c_driver);
}

module_init(mt9v113_init);
module_exit(mt9v113_cleanup);

MODULE_AUTHOR("Texas Instruments");
MODULE_DESCRIPTION("MT9V113 camera sensor driver");
MODULE_LICENSE("GPL");
