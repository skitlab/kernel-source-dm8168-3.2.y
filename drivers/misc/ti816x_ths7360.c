#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/i2c/pcf857x.h>
#include <linux/module.h>
#include <mach/board-ti816x.h>


/* Macros for PCF8575 Pins */
#define VPS_PCF8575_PIN0                (0x1)
#define VPS_PCF8575_PIN1                (0x2)
#define VPS_PCF8575_PIN2                (0x4)
#define VPS_PCF8575_PIN3                (0x8)
#define VPS_PCF8575_PIN4                (0x10)
#define VPS_PCF8575_PIN5                (0x20)
#define VPS_PCF8575_PIN6                (0x40)
#define VPS_PCF8575_PIN7                (0x80)

#define VPS_THS7360_SD_MASK             (VPS_PCF8575_PIN2 | VPS_PCF8575_PIN5)

#define VPS_THS7360_SF_MASK             (VPS_PCF8575_PIN0 |                    \
					 VPS_PCF8575_PIN1 |                    \
					 VPS_PCF8575_PIN3 |                    \
					 VPS_PCF8575_PIN4)

struct ths7360_data {
	struct i2c_client *ths7360_client;
	unsigned char ths7360_config[2];
};

static struct ths7360_data *ths7360;

static const struct i2c_device_id pcf8575_ths7360_id[] = {
	{ "pcf8575_ths7360", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, pcf8575_ths7360_id);

int pcf8575_ths7360_sd_enable(enum sd_channel_filter_ctrl ctrl)
{
	struct i2c_msg msg = {
		.addr = ths7360->ths7360_client->addr,
		.flags = 0,
		.len = 2,
	};

	ths7360->ths7360_config[0] &= ~VPS_THS7360_SD_MASK;
	switch (ctrl) {
	case SD_FILTER_ENABLE:
		ths7360->ths7360_config[0] &= ~(VPS_THS7360_SD_MASK);
		break;
	case SD_FILTER_BYPASS:
		ths7360->ths7360_config[0] |= VPS_PCF8575_PIN2;
		break;
	case SD_FILTER_DISABLE:
		ths7360->ths7360_config[0] |= VPS_THS7360_SD_MASK;
		break;
	default:
		return -EINVAL;
	}

	msg.buf = ths7360->ths7360_config;
	return i2c_transfer(ths7360->ths7360_client->adapter, &msg, 1);
}
EXPORT_SYMBOL(pcf8575_ths7360_sd_enable);

int pcf8575_ths7360_hd_enable(enum sf_channel_filter_ctrl ctrl)
{
	int ret_val;

	struct i2c_msg msg = {
		.addr = ths7360->ths7360_client->addr,
		.flags = 0,
		.len = 2,
	};

	ths7360->ths7360_config[0] &= ~VPS_THS7360_SF_MASK;
	switch (ctrl) {
	case SF_FILTER_DISABLE:
		ths7360->ths7360_config[0] |= VPS_PCF8575_PIN4;
		break;
	case SF_FILTER_BYPASS:
		ths7360->ths7360_config[0] |= VPS_PCF8575_PIN3;
		break;
	case SF_FILTER_SD_MODE:
		ths7360->ths7360_config[0] &= ~(VPS_THS7360_SF_MASK);
		break;
	case SF_FILTER_ED_MODE:
		ths7360->ths7360_config[0] |= VPS_PCF8575_PIN0;
		break;
	case SF_FILTER_HD_MODE:
		ths7360->ths7360_config[0] |= VPS_PCF8575_PIN1;
		break;
	case SF_FILTER_TRUE_HD_MODE:
		ths7360->ths7360_config[0] |= VPS_PCF8575_PIN0|VPS_PCF8575_PIN1;
		break;
	default:
		return -EINVAL;
	}
	msg.buf = ths7360->ths7360_config;

	ret_val = i2c_transfer(ths7360->ths7360_client->adapter, &msg, 1);
	return ret_val;
}
EXPORT_SYMBOL(pcf8575_ths7360_hd_enable);

static int pcf8575_ths7360_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	ths7360 = kzalloc(sizeof(struct ths7360_data), GFP_KERNEL);
	if (client->addr == 0x20) {
        ths7360->ths7360_client = client;
	    i2c_set_clientdata(client, ths7360);
	}
	return 0;
}

static int pcf8575_ths7360_remove(struct i2c_client *client)
{
	ths7360 = i2c_get_clientdata(client);
	ths7360->ths7360_client = NULL;
	kfree(ths7360);
	return 0;
}

static struct i2c_driver pcf8575_ths7360_driver = {
	.driver = {
		.name   = "pcf8575_ths7360",
		.owner	= THIS_MODULE,
	},
	.probe          = pcf8575_ths7360_probe,
	.remove         = pcf8575_ths7360_remove,
	.id_table       = pcf8575_ths7360_id,
};

static int __init ti816x_ths7360_init(void)
{
	i2c_add_driver(&pcf8575_ths7360_driver);
	return 0;
}
module_init(ti816x_ths7360_init);

static void __exit ti816x_ths7360_exit(void)
{
	i2c_del_driver(&pcf8575_ths7360_driver);
}
module_exit(ti816x_ths7360_exit);

MODULE_LICENSE("GPL");

