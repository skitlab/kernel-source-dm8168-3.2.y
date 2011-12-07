/*
 * Code for TI8148 EVM.
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
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/i2c/at24.h>
#include <linux/i2c/qt602240_ts.h>
#include <linux/i2c/pcf857x.h>
#include <linux/regulator/machine.h>
#include <linux/mfd/tps65910.h>

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <plat/mcspi.h>
#include <plat/irqs.h>
#include <plat/board.h>
#include <plat/common.h>
#include <plat/asp.h>
#include <plat/usb.h>
#include <plat/mmc.h>
#include <plat/gpmc.h>
#include <plat/nand.h>
#include <plat/hdmi_lib.h>
#include <mach/board-ti814x.h>

#include "board-flash.h"
#include "clock.h"
#include "mux.h"
#include "hsmmc.h"
#include "control.h"

#define GPIO_TSC               31

#ifdef CONFIG_OMAP_MUX
static struct omap_board_mux board_mux[] __initdata = {
	{ .reg_offset = OMAP_MUX_TERMINATOR },
};
#else
#define board_mux     NULL
#endif

static struct omap2_hsmmc_info mmc[] = {
	{
		.mmc		= 1,
		.caps		= MMC_CAP_4_BIT_DATA,
		.gpio_cd	= -EINVAL, /* Dedicated pins for CD and WP */
		.gpio_wp	= -EINVAL,
		.ocr_mask	= MMC_VDD_33_34,
	},
	{}	/* Terminator */
};

#define GPIO_LCD_PWR_DOWN	0

static int setup_gpio_ioexp(struct i2c_client *client, int gpio_base,
	 unsigned ngpio, void *context) {
	int ret = 0;
	int gpio = gpio_base + GPIO_LCD_PWR_DOWN;

	ret = gpio_request(gpio, "lcd_power");
	if (ret) {
		printk(KERN_ERR "%s: failed to request GPIO for LCD Power"
			": %d\n", __func__, ret);
		return ret;
	}

	gpio_export(gpio, true);
	gpio_direction_output(gpio, 0);

	return 0;
}

/* IO expander data */
static struct pcf857x_platform_data io_expander_data = {
	.gpio_base	= 4 * 32,
	.setup		= setup_gpio_ioexp,
};
static struct i2c_board_info __initdata ti814x_i2c_boardinfo1[] = {
	{
		I2C_BOARD_INFO("pcf8575_1", 0x21),
	},

};

#define VPS_VC_IO_EXP_RESET_DEV_MASK        (0x0Fu)
#define VPS_VC_IO_EXP_SEL_VIN0_S1_MASK      (0x04u)
#define VPS_VC_IO_EXP_THS7368_DISABLE_MASK  (0x10u)
#define VPS_VC_IO_EXP_THS7368_BYPASS_MASK   (0x20u)
#define VPS_VC_IO_EXP_THS7368_FILTER1_MASK  (0x40u)
#define VPS_VC_IO_EXP_THS7368_FILTER2_MASK  (0x80u)
#define VPS_VC_IO_EXP_THS7368_FILTER_SHIFT  (0x06u)


static const struct i2c_device_id pcf8575_video_id[] = {
	{ "pcf8575_1", 0 },
	{ }
};
static struct i2c_client *pcf8575_client;
static unsigned char pcf8575_port[2] = {0x4F, 0x7F};
int vps_ti814x_select_video_decoder(int vid_decoder_id);

static int pcf8575_video_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	pcf8575_client = client;
	vps_ti814x_select_video_decoder(0);
	return 0;
}

static int __devexit pcf8575_video_remove(struct i2c_client *client)
{
	pcf8575_client = NULL;
	return 0;
}

static struct i2c_driver pcf8575_driver = {
	.driver = {
		.name   = "pcf8575_1",
	},
	.probe          = pcf8575_video_probe,
	.remove         = pcf8575_video_remove,
	.id_table       = pcf8575_video_id,
};

int ti814x_pcf8575_init(void)
{
	i2c_add_driver(&pcf8575_driver);
	return 0;
}
EXPORT_SYMBOL(ti814x_pcf8575_init);

int ti814x_pcf8575_exit(void)
{
	i2c_del_driver(&pcf8575_driver);
	return 0;
}
EXPORT_SYMBOL(ti814x_pcf8575_exit);
#define VPS_VC_IO_EXP_RESET_DEV_MASK        (0x0Fu)
#define VPS_VC_IO_EXP_SEL_VIN0_S1_MASK      (0x04u)
#define VPS_VC_IO_EXP_THS7368_DISABLE_MASK  (0x10u)
#define VPS_VC_IO_EXP_THS7368_BYPASS_MASK   (0x20u)
#define VPS_VC_IO_EXP_THS7368_FILTER1_MASK  (0x40u)
#define VPS_VC_IO_EXP_THS7368_FILTER2_MASK  (0x80u)
#define VPS_VC_IO_EXP_THS7368_FILTER_SHIFT  (0x06u)
int vps_ti814x_select_video_decoder(int vid_decoder_id)
{
	int ret = 0;
	struct i2c_msg msg = {
			.addr = pcf8575_client->addr,
			.flags = 0,
			.len = 2,
		};
	msg.buf = pcf8575_port;
	if (VPS_SEL_TVP7002_DECODER == vid_decoder_id)
		pcf8575_port[1] &= ~VPS_VC_IO_EXP_SEL_VIN0_S1_MASK;
	else
		pcf8575_port[1] |= VPS_VC_IO_EXP_SEL_VIN0_S1_MASK;
	ret = (i2c_transfer(pcf8575_client->adapter, &msg, 1));
	if (ret < 0)
		printk(KERN_ERR "I2C: Transfer failed at %s %d with error code: %d\n",
			__func__, __LINE__, ret);
	return ret;
}
EXPORT_SYMBOL(vps_ti814x_select_video_decoder);

#define I2C_RETRY_COUNT 10u
int vps_ti814x_set_tvp7002_filter(enum fvid2_standard standard)
{
	int filter_sel;
	int ret;
	struct i2c_msg msg = {
			.addr = pcf8575_client->addr,
			.flags = 0,
			.len = 2,
		};

	pcf8575_port[0] &= ~(VPS_VC_IO_EXP_THS7368_DISABLE_MASK
		| VPS_VC_IO_EXP_THS7368_BYPASS_MASK
		| VPS_VC_IO_EXP_THS7368_FILTER1_MASK
		| VPS_VC_IO_EXP_THS7368_FILTER2_MASK);
	switch (standard) {
	case FVID2_STD_1080P_60:
	case FVID2_STD_1080P_50:
	case FVID2_STD_SXGA_60:
	case FVID2_STD_SXGA_75:
	case FVID2_STD_SXGAP_60:
	case FVID2_STD_SXGAP_75:
	case FVID2_STD_UXGA_60:
		filter_sel = 0x03u;  /* Filter2: 1, Filter1: 1 */
		break;
	case FVID2_STD_1080I_60:
	case FVID2_STD_1080I_50:
	case FVID2_STD_1080P_24:
	case FVID2_STD_1080P_30:
	case FVID2_STD_720P_60:
	case FVID2_STD_720P_50:
	case FVID2_STD_SVGA_60:
	case FVID2_STD_SVGA_72:
	case FVID2_STD_SVGA_75:
	case FVID2_STD_SVGA_85:
	case FVID2_STD_XGA_60:
	case FVID2_STD_XGA_70:
	case FVID2_STD_XGA_75:
	case FVID2_STD_XGA_85:
	case FVID2_STD_WXGA_60:
	case FVID2_STD_WXGA_75:
	case FVID2_STD_WXGA_85:
		filter_sel = 0x01u;  /* Filter2: 0, Filter1: 1 */
		break;
	case FVID2_STD_480P:
	case FVID2_STD_576P:
	case FVID2_STD_VGA_60:
	case FVID2_STD_VGA_72:
	case FVID2_STD_VGA_75:
	case FVID2_STD_VGA_85:
		filter_sel = 0x02u;  /* Filter2: 1, Filter1: 0 */
		break;
	case FVID2_STD_NTSC:
	case FVID2_STD_PAL:
	case FVID2_STD_480I:
	case FVID2_STD_576I:
	case FVID2_STD_D1:
		filter_sel = 0x00u;  /* Filter2: 0, Filter1: 0 */
		break;

	default:
		filter_sel = 0x01u;  /* Filter2: 0, Filter1: 1 */
		break;
	}
	pcf8575_port[0] |=
		(filter_sel << VPS_VC_IO_EXP_THS7368_FILTER_SHIFT);
	msg.buf = pcf8575_port;
	ret =  (i2c_transfer(pcf8575_client->adapter, &msg, 1));
	if (ret < 0) {
		printk(KERN_ERR "I2C: Transfer failed at %s %d with error code: %d\n",
			__func__, __LINE__, ret);
		return ret;
	}
	return ret;
}
EXPORT_SYMBOL(vps_ti814x_set_tvp7002_filter);
/* Touchscreen platform data */
static struct qt602240_platform_data ts_platform_data = {
	.x_line		= 18,
	.y_line		= 12,
	.x_size		= 800,
	.y_size		= 480,
	.blen		= 0x01,
	.threshold	= 30,
	.voltage	= 2800000,
	.orient		= QT602240_HORIZONTAL_FLIP,
};

static struct at24_platform_data eeprom_info = {
	.byte_len       = (256*1024) / 8,
	.page_size      = 64,
	.flags          = AT24_FLAG_ADDR16,
};

static struct regulator_consumer_supply ti8148evm_mpu_supply =
	REGULATOR_SUPPLY("mpu", "mpu.0");

/*
 * DM814x/AM387x (TI814x) devices have restriction that none of the supply to
 * the device should be turned of.
 *
 * NOTE: To prevent turning off regulators not explicitly consumed by drivers
 * depending on it, ensure following:
 *	1) Set always_on = 1 for them OR
 *	2) Avoid calling regulator_has_full_constraints()
 *
 * With just (2), there will be a warning about incomplete constraints.
 * E.g., "regulator_init_complete: incomplete constraints, leaving LDO8 on"
 *
 * In either cases, the supply won't be disabled.
 *
 * We are taking approach (1).
 */
static struct regulator_init_data tps65911_reg_data[] = {
	/* VRTC */
	{
		.constraints = {
			.min_uV = 1800000,
			.max_uV = 1800000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* VIO -VDDA 1.8V */
	{
		.constraints = {
			.min_uV = 1500000,
			.max_uV = 1500000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* VDD1 - MPU */
	{
		.constraints = {
			.min_uV = 600000,
			.max_uV = 1500000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
			.always_on = 1,
		},
		.num_consumer_supplies	= 1,
		.consumer_supplies	= &ti8148evm_mpu_supply,
	},

	/* VDD2 - DSP */
	{
		.constraints = {
			.min_uV = 600000,
			.max_uV = 1500000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* VDDCtrl - CORE */
	{
		.constraints = {
			.min_uV = 600000,
			.max_uV = 1400000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
			.always_on = 1,
		},
	},

	/* LDO1 - VDAC */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* LDO2 - HDMI */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* LDO3 - GPIO 3.3V */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* LDO4 - PLL 1.8V */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
			.always_on = 1,
		},
	},

	/* LDO5 - SPARE */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* LDO6 - CDC */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
			.always_on = 1,
		},
	},

	/* LDO7 - SPARE */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* LDO8 - USB 1.8V */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},
};

static struct tps65910_board __refdata tps65911_pdata = {
	.irq				= 0,	/* No support currently */
	.gpio_base			= 0,	/* No support currently */
	.tps65910_pmic_init_data	= tps65911_reg_data,
};

static struct i2c_board_info __initdata ti814x_i2c_boardinfo[] = {
	{
		I2C_BOARD_INFO("eeprom", 0x50),
		.platform_data	= &eeprom_info,
	},
	{
		I2C_BOARD_INFO("cpld", 0x23),
	},
	{
		I2C_BOARD_INFO("tlv320aic3x", 0x18),
	},
	{
		I2C_BOARD_INFO("IO Expander", 0x20),
	},
	{
		I2C_BOARD_INFO("tlc59108", 0x40),
	},
	{
		I2C_BOARD_INFO("pcf8575", 0x21),
		.platform_data = &io_expander_data,
	},
	{
		I2C_BOARD_INFO("qt602240_ts", 0x4A),
		.platform_data = &ts_platform_data,
	},
	{
		I2C_BOARD_INFO("tps65911", 0x2D),
		.platform_data = &tps65911_pdata,
	},
};

static void __init ti814x_tsc_init(void)
{
	int error;

	omap_mux_init_signal("mlb_clk.gpio0_31", TI814X_PULL_DIS | (1 << 18));

	error = gpio_request(GPIO_TSC, "ts_irq");
	if (error < 0) {
		printk(KERN_ERR "%s: failed to request GPIO for TSC IRQ"
			": %d\n", __func__, error);
		return;
	}

	gpio_direction_input(GPIO_TSC);
	ti814x_i2c_boardinfo[6].irq = gpio_to_irq(GPIO_TSC);

	gpio_export(31, true);
}

static void __init ti814x_evm_i2c_init(void)
{
	/* There are 4 instances of I2C in TI814X but currently only one
	 * instance is being used on the TI8148 EVM
	 */
	omap_register_i2c_bus(1, 100, ti814x_i2c_boardinfo,
				ARRAY_SIZE(ti814x_i2c_boardinfo));
	omap_register_i2c_bus(3, 100, ti814x_i2c_boardinfo1,
				ARRAY_SIZE(ti814x_i2c_boardinfo1));
}

static u8 ti8148_iis_serializer_direction[] = {
	TX_MODE,	RX_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
};

static struct snd_platform_data ti8148_evm_snd_data = {
	.tx_dma_offset	= 0x46800000,
	.rx_dma_offset	= 0x46800000,
	.op_mode	= DAVINCI_MCASP_IIS_MODE,
	.num_serializer = ARRAY_SIZE(ti8148_iis_serializer_direction),
	.tdm_slots	= 2,
	.serial_dir	= ti8148_iis_serializer_direction,
	.asp_chan_q	= EVENTQ_2,
	.version	= MCASP_VERSION_2,
	.txnumevt	= 1,
	.rxnumevt	= 1,
};

/* NOR Flash partitions */
static struct mtd_partition ti814x_evm_norflash_partitions[] = {
	/* bootloader (U-Boot, etc) in first 5 sectors */
	{
		.name		= "bootloader",
		.offset		= 0,
		.size		= 2 * SZ_128K,
		.mask_flags	= MTD_WRITEABLE, /* force read-only */
	},
	/* bootloader params in the next 1 sectors */
	{
		.name		= "env",
		.offset		= MTDPART_OFS_APPEND,
		.size		= SZ_128K,
		.mask_flags	= 0,
	},
	/* kernel */
	{
		.name		= "kernel",
		.offset		= MTDPART_OFS_APPEND,
		.size		= 2 * SZ_2M,
		.mask_flags	= 0
	},
	/* file system */
	{
		.name		= "filesystem",
		.offset		= MTDPART_OFS_APPEND,
		.size		= 25 * SZ_2M,
		.mask_flags	= 0
	},
	/* reserved */
	{
		.name		= "reserved",
		.offset		= MTDPART_OFS_APPEND,
		.size		= MTDPART_SIZ_FULL,
		.mask_flags	= 0
	}
};

/* NAND flash information */
static struct mtd_partition ti814x_nand_partitions[] = {
/* All the partition sizes are listed in terms of NAND block size */
	{
		.name           = "U-Boot-min",
		.offset         = 0,    /* Offset = 0x0 */
		.size           = SZ_128K,
	},
	{
		.name           = "U-Boot",
		.offset         = MTDPART_OFS_APPEND,/* Offset = 0x0 + 128K */
		.size           = 18 * SZ_128K,
	},
	{
		.name           = "U-Boot Env",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0x260000 */
		.size           = 1 * SZ_128K,
	},
	{
		.name           = "Kernel",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0x280000 */
		.size           = 34 * SZ_128K,
	},
	{
		.name           = "File System",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0x6C0000 */
		.size           = 1601 * SZ_128K,
	},
	{
		.name           = "Reserved",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0xCEE0000 */
		.size           = MTDPART_SIZ_FULL,
	},
};

/* SPI fLash information */
struct mtd_partition ti8148_spi_partitions[] = {
	/* All the partition sizes are listed in terms of erase size */
	{
		.name		= "U-Boot-min",
		.offset		= 0,	/* Offset = 0x0 */
		.size		= 32 * SZ_4K,
		.mask_flags	= MTD_WRITEABLE, /* force read-only */
	},
	{
		.name		= "U-Boot",
		.offset		= MTDPART_OFS_APPEND, /* 0x0 + (32*4K) */
		.size		= 64 * SZ_4K,
		.mask_flags	= MTD_WRITEABLE, /* force read-only */
	},
	{
		.name		= "U-Boot Env",
		.offset		= MTDPART_OFS_APPEND, /* 0x40000 + (32*4K) */
		.size		= 2 * SZ_4K,
	},
	{
		.name		= "Kernel",
		.offset		= MTDPART_OFS_APPEND, /* 0x42000 + (32*4K) */
		.size		= 640 * SZ_4K,
	},
	{
		.name		= "File System",
		.offset		= MTDPART_OFS_APPEND, /* 0x2C2000 + (32*4K) */
		.size		= MTDPART_SIZ_FULL, /* size ~= 1.1 MiB */
	}
};

const struct flash_platform_data ti8148_spi_flash = {
	.type		= "w25x32",
	.name		= "spi_flash",
	.parts		= ti8148_spi_partitions,
	.nr_parts	= ARRAY_SIZE(ti8148_spi_partitions),
};

struct spi_board_info __initdata ti8148_spi_slave_info[] = {
	{
		.modalias	= "m25p80",
		.platform_data	= &ti8148_spi_flash,
		.irq		= -1,
		.max_speed_hz	= 75000000,
		.bus_num	= 1,
		.chip_select	= 0,
	},
};

void __init ti8148_spi_init(void)
{
	spi_register_board_info(ti8148_spi_slave_info,
				ARRAY_SIZE(ti8148_spi_slave_info));
}

static struct omap_musb_board_data musb_board_data = {
	.interface_type		= MUSB_INTERFACE_ULPI,
#ifdef CONFIG_USB_MUSB_OTG
	.mode           = MUSB_OTG,
#elif defined(CONFIG_USB_MUSB_HDRC_HCD)
	.mode           = MUSB_HOST,
#elif defined(CONFIG_USB_GADGET_MUSB_HDRC)
	.mode           = MUSB_PERIPHERAL,
#endif
	.power		= 500,
	.instances	= 1,
};

static struct platform_device vpss_device = {
	.name = "vpss",
	.id = -1,
	.dev = {
		.platform_data = NULL,
	},
};
static void __init ti8148_evm_init_irq(void)
{
	omap2_init_common_infrastructure();
	omap2_init_common_devices(NULL, NULL);
	omap_init_irq();
	gpmc_init();
}

static void __init ti814x_vpss_init(void)
{
	/*FIXME add platform data here*/

	if (platform_device_register(&vpss_device))
		printk(KERN_ERR "failed to register ti814x_vpss device\n");
	else
		printk(KERN_INFO "registered ti814x_vpss device\n");
}

static struct platform_device ti814x_hdmi_plat_device = {
	.name = "TI81XX_HDMI",
	.id = -1,
	.num_resources = 0,
	.dev = {
		/*.release = ti81xx_hdmi_platform_release,*/
		.platform_data = NULL,
	}
};

#ifdef CONFIG_SND_SOC_TI81XX_HDMI
static struct snd_hdmi_platform_data ti8148_snd_hdmi_pdata = {
	.dma_addr = TI81xx_HDMI_WP + HDMI_WP_AUDIO_DATA,
	.channel = 53,
	.data_type = 4,
	.acnt = 4,
	.fifo_level = 0x20,
};

static struct platform_device ti8148_hdmi_audio_device = {
	.name   = "hdmi-dai",
	.id     = -1,
	.dev = {
		.platform_data = &ti8148_snd_hdmi_pdata,
	}
};

static struct platform_device ti8148_hdmi_codec_device = {
	.name   = "hdmi-dummy-codec",
	.id     = -1,
};

static struct platform_device *ti8148_devices[] __initdata = {
	&ti8148_hdmi_audio_device,
	&ti8148_hdmi_codec_device,
};
#endif

static void __init ti814x_hdmi_init(void)
{

	if (platform_device_register(&ti814x_hdmi_plat_device))
		printk(KERN_ERR "Could not register TI814x onchip-HDMI device\n");
	else
		printk(KERN_INFO "registered TI814x on-chip HDMI device\n");
	/*FIXME add platform data here*/
}

static void __init ti8148_evm_init(void)
{
	int bw; /* bus-width */

	ti814x_mux_init(board_mux);
	omap_serial_init();
	ti814x_tsc_init();
	ti814x_evm_i2c_init();
	ti81xx_register_mcasp(0, &ti8148_evm_snd_data);

	omap2_hsmmc_init(mmc);

	/* nand initialisation */
	if (cpu_is_ti814x()) {
		u32 *control_status = TI81XX_CTRL_REGADDR(0x40);
		if (*control_status & (1<<16))
			bw = 0; /*8-bit nand if BTMODE BW pin on board is ON*/
		else
			bw = 2; /*16-bit nand if BTMODE BW pin on board is OFF*/

		board_nand_init(ti814x_nand_partitions,
			ARRAY_SIZE(ti814x_nand_partitions), 0, bw);
	} else
		board_nand_init(ti814x_nand_partitions,
		ARRAY_SIZE(ti814x_nand_partitions), 0, NAND_BUSWIDTH_16);

	/* initialize usb */
	usb_musb_init(&musb_board_data);

	ti8148_spi_init();
	ti814x_vpss_init();
	ti814x_hdmi_init();
#ifdef CONFIG_SND_SOC_TI81XX_HDMI
	platform_add_devices(ti8148_devices, ARRAY_SIZE(ti8148_devices));
#endif
	regulator_use_dummy_regulator();
	board_nor_init(ti814x_evm_norflash_partitions,
		ARRAY_SIZE(ti814x_evm_norflash_partitions), 0);
}

static void __init ti8148_evm_map_io(void)
{
	omap2_set_globals_ti814x();
	ti81xx_map_common_io();
}

MACHINE_START(TI8148EVM, "ti8148evm")
	/* Maintainer: Texas Instruments */
	.boot_params	= 0x80000100,
	.map_io		= ti8148_evm_map_io,
	.reserve         = ti81xx_reserve,
	.init_irq	= ti8148_evm_init_irq,
	.init_machine	= ti8148_evm_init,
	.timer		= &omap_timer,
MACHINE_END
