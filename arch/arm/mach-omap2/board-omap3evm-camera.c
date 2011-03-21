/*
 * OMAP3EVM: Driver for Leopard Module Board
 *
 * Copyright (C) 2011 Texas Instruments Inc
 * Author: Vaibhav Hiremath <hvaibhav@ti.com>
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
 */

#include <linux/io.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>

#include <mach/gpio.h>

#include <media/mt9t111.h>

#include <../drivers/media/video/isp/isp.h>

#include "devices.h"

#define CAM_USE_XCLKA			0

#define T2_GPIO_2			194
#define nCAM_VD_SEL			157
#define nCAM_VD_EN			200

static struct regulator *omap3evm_1v8;
static struct regulator *omap3evm_2v8;


/* mux id to enable/disable signal routing to different peripherals */
enum omap3evm_cam_mux {
	MUX_EN_TVP5146 = 0,
	MUX_EN_CAMERA_SENSOR,
	MUX_EN_EXP_CAMERA_SENSOR,
	MUX_INVALID,
};

/**
 * @brief omap3evm_set_mux - Sets mux to enable/disable signal routing to
 *                             different peripherals present on new EVM board
 *
 * @param mux_id - enum, mux id to enable/disable
 * @param value - enum, ENABLE_MUX for enabling and DISABLE_MUX for disabling
 *
 */
static void omap3evm_set_mux(enum omap3evm_cam_mux mux_id, bool on)
{
	/*
	 * Directly disable fisrt level GPIO buffer
	 */
	if (on)
		gpio_set_value_cansleep(T2_GPIO_2, 0);
	else
		gpio_set_value_cansleep(T2_GPIO_2, 1);


	switch (mux_id) {
	/*
	 * JP1 jumper need to configure to choose betweek on-board
	 * camera sensor conn and on-board LI-3MC02 camera sensor.
	 */
	case MUX_EN_CAMERA_SENSOR:
		/* Set nCAM_VD_EN (T2_GPIO8) = 0 */
		gpio_set_value_cansleep(nCAM_VD_EN, 0);
		/* Set nCAM_VD_SEL (GPIO157) = 0 */
		gpio_set_value(nCAM_VD_SEL, 0);
		break;

	case MUX_EN_EXP_CAMERA_SENSOR:
		/* Set nCAM_VD_EN (T2_GPIO8) = 0 */
		gpio_set_value_cansleep(nCAM_VD_EN, 1);

		break;

	case MUX_EN_TVP5146:
	default:
		/* Set nCAM_VD_EN (T2_GPIO8) = 0 */
		gpio_set_value_cansleep(nCAM_VD_EN, 0);
		/* Set nCAM_VD_SEL (GPIO157) = 1 */
		gpio_set_value(nCAM_VD_SEL, 1);
		break;
	}
}


static int omap3evm_mt9t111_s_power(struct v4l2_subdev *subdev, u32 on)
{
	struct isp_device *isp = v4l2_dev_to_isp_device(subdev->v4l2_dev);

	if (!omap3evm_1v8 || !omap3evm_2v8) {
		dev_err(isp->dev, "No regulator available\n");
		return -ENODEV;
	}

	omap3evm_set_mux(MUX_EN_CAMERA_SENSOR, on);

	if (on) {
		/* Turn on VDD */
		regulator_enable(omap3evm_1v8);
		mdelay(1);
		regulator_enable(omap3evm_2v8);

		mdelay(50);
		/* Enable EXTCLK */
		if (isp->platform_cb.set_xclk)
			isp->platform_cb.set_xclk(isp, 24000000, CAM_USE_XCLKA);
		udelay(5);
	} else {
		/*
		 * Power Down Sequence
		 */
		if (regulator_is_enabled(omap3evm_1v8))
			regulator_disable(omap3evm_1v8);
		if (regulator_is_enabled(omap3evm_2v8))
			regulator_disable(omap3evm_2v8);

		if (isp->platform_cb.set_xclk)
			isp->platform_cb.set_xclk(isp, 0, CAM_USE_XCLKA);
	}

	return 0;
}

static int omap3evm_mt9t111_configure_interface(struct v4l2_subdev *subdev,
					      u32 pixclk)
{
	struct isp_device *isp = v4l2_dev_to_isp_device(subdev->v4l2_dev);

	if (isp->platform_cb.set_pixel_clock)
		isp->platform_cb.set_pixel_clock(isp, pixclk);

	return 0;
}

static struct mt9t111_platform_data omap3evm_mt9t111_platform_data = {
	.s_power		= omap3evm_mt9t111_s_power,
	.configure_interface	= omap3evm_mt9t111_configure_interface,
};


#define MT9T111_I2C_BUS_NUM		2

static struct i2c_board_info omap3evm_camera_i2c_devices[] = {
	{
		I2C_BOARD_INFO(MT9T111_MODULE_NAME, MT9T111_I2C_ADDR),
		.platform_data = &omap3evm_mt9t111_platform_data,
	},
};

static struct isp_subdev_i2c_board_info omap3evm_camera_primary_subdevs[] = {
	{
		.board_info = &omap3evm_camera_i2c_devices[0],
		.i2c_adapter_id = MT9T111_I2C_BUS_NUM,
	},
	{ NULL, 0 },
};

static struct isp_v4l2_subdevs_group omap3evm_camera_subdevs[] = {
	{
		.subdevs = omap3evm_camera_primary_subdevs,
		.interface = ISP_INTERFACE_PARALLEL,
		.bus = {
			.parallel = {
				.data_lane_shift	= 1,
				.clk_pol		= 0,
				.bridge			= 3,
			},
		},
	},
	{ NULL, 0 },
};

static struct isp_platform_data omap3evm_isp_platform_data = {
	.subdevs = omap3evm_camera_subdevs,
};

static int __init omap3evm_cam_init(void)
{
	int ret = 0;

	/*
	 * Regulator supply required for camera interface
	 */
	omap3evm_1v8 = regulator_get(NULL, "vio_1v8");
	if (IS_ERR(omap3evm_1v8)) {
		printk(KERN_ERR "vio_1v8 regulator missing\n");
		return PTR_ERR(omap3evm_1v8);
	}
	omap3evm_2v8 = regulator_get(NULL, "cam_2v8");
	if (IS_ERR(omap3evm_2v8)) {
		printk(KERN_ERR "cam_2v8 regulator missing\n");
		ret = PTR_ERR(omap3evm_2v8);
		goto err_1;
	}

	/*
	 * First level GPIO enable: T2_GPIO.2
	 */
	ret = gpio_request(T2_GPIO_2, "T2_GPIO.2");
	if (ret) {
		printk(KERN_ERR "failed to get t2_gpio.2\n");
		goto err_2;
	}
	gpio_direction_output(T2_GPIO_2, 0);

	/*
	 * nCAM_VD_SEL (GPIO157)
	 */
	ret = gpio_request(nCAM_VD_SEL, "cam_vd_sel");
	if (ret) {
		printk(KERN_ERR "failed to get cam_vd_sel\n");
		goto err_3;
	}
	gpio_direction_output(nCAM_VD_SEL, 0);

	/*
	 * EXP_nCAM_VD_EN (T2_GPIO.8)
	 */
	ret = gpio_request(nCAM_VD_EN, "cam_vd_en");
	if (ret) {
		printk(KERN_ERR "failed to get cam_vd_en\n");
		goto err_4;
	}
	gpio_direction_output(nCAM_VD_EN, 0);

	omap3_init_camera(&omap3evm_isp_platform_data);

	printk(KERN_INFO "omap3evm camer init done successfully...\n");
	return 0;

err_4:
	gpio_free(nCAM_VD_SEL);
err_3:
	gpio_free(T2_GPIO_2);
err_2:
	regulator_put(omap3evm_2v8);
err_1:
	regulator_put(omap3evm_1v8);

	return ret;
}

static void __exit omap3evm_cam_exit(void)
{
	gpio_free(nCAM_VD_EN);
	gpio_free(nCAM_VD_SEL);
	gpio_free(T2_GPIO_2);

	if (regulator_is_enabled(omap3evm_1v8))
		regulator_disable(omap3evm_1v8);
	regulator_put(omap3evm_1v8);
	if (regulator_is_enabled(omap3evm_2v8))
		regulator_disable(omap3evm_2v8);
	regulator_put(omap3evm_2v8);
}

module_init(omap3evm_cam_init);
module_exit(omap3evm_cam_exit);

MODULE_AUTHOR("Texas Instruments");
MODULE_DESCRIPTION("omap3evm Camera Module");
MODULE_LICENSE("GPL");
