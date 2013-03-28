#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/i2c/pcf857x.h>
#include <linux/module.h>
#include <mach/board-ti816x.h>

#define VPS_VC_IO_EXP_SEL_VIN0_S1_MASK      (0x04u)
#define VPS_VC_IO_EXP_THS7368_DISABLE_MASK  (0x10u)
#define VPS_VC_IO_EXP_THS7368_BYPASS_MASK   (0x20u)
#define VPS_VC_IO_EXP_THS7368_FILTER1_MASK  (0x40u)
#define VPS_VC_IO_EXP_THS7368_FILTER2_MASK  (0x80u)
#define VPS_VC_IO_EXP_THS7368_FILTER_SHIFT  (0x06u)

struct ths7368_data {
	struct i2c_client *ths7368_client;
	unsigned char ths7368_port[2];
};

static struct ths7368_data *ths7368;

static const struct i2c_device_id pcf8575_ths7368_id[] = {
	{ "pcf8575_ths7368", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, pcf8575_ths7368_id);

int pcf8575_ths7368_select_video_decoder(int vid_decoder_id)
{
	int ret = 0;
	struct i2c_msg msg = {
			.addr = ths7368->ths7368_client->addr,
			.flags = 0,
			.len = 2,
		};
	msg.buf = ths7368->ths7368_port;
	if (VPS_SEL_TVP7002_DECODER == vid_decoder_id)
		ths7368->ths7368_port[1] &= ~VPS_VC_IO_EXP_SEL_VIN0_S1_MASK;
	else
		ths7368->ths7368_port[1] |= VPS_VC_IO_EXP_SEL_VIN0_S1_MASK;
	ret = (i2c_transfer(ths7368->ths7368_client->adapter, &msg, 1));

	if (ret < 0)
		printk(KERN_ERR "I2C: Transfer failed at %s %d with error code: %d\n",
			__func__, __LINE__, ret);
	return ret;
}
EXPORT_SYMBOL(pcf8575_ths7368_select_video_decoder);

int pcf8575_ths7368_set_tvp7002_filter(enum fvid2_standard standard)
{
	int filter_sel;
	int ret;
	struct i2c_msg msg = {
			.addr = ths7368->ths7368_client->addr,
			.flags = 0,
			.len = 2,
		};

	ths7368->ths7368_port[0] &= ~(VPS_VC_IO_EXP_THS7368_DISABLE_MASK
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
	ths7368->ths7368_port[0] |=
		(filter_sel << VPS_VC_IO_EXP_THS7368_FILTER_SHIFT);
	msg.buf = ths7368->ths7368_port;
	ret =  (i2c_transfer(ths7368->ths7368_client->adapter, &msg, 1));
	if (ret < 0) {
		printk(KERN_ERR "I2C: Transfer failed at %s %d with error code: %d\n",
			__func__, __LINE__, ret);
		return ret;
	}
	return 0;
}
EXPORT_SYMBOL(pcf8575_ths7368_set_tvp7002_filter);

static int pcf8575_ths7368_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	ths7368 = kzalloc(sizeof(struct ths7368_data), GFP_KERNEL);
	if (client->addr == 0x21) {
        ths7368->ths7368_client = client;
        ths7368->ths7368_port[0] = 0xFF;
		ths7368->ths7368_port[1] = 0x2F;
	    i2c_set_clientdata(client, ths7368);
	}
	return 0;
}

static int pcf8575_ths7368_remove(struct i2c_client *client)
{
	ths7368 = i2c_get_clientdata(client);
	ths7368->ths7368_client = NULL;
	kfree(ths7368);
	return 0;
}

static struct i2c_driver pcf8575_ths7368_driver = {
	.driver = {
		.name   = "pcf8575_ths7368",
		.owner	= THIS_MODULE,
	},
	.probe          = pcf8575_ths7368_probe,
	.remove         = pcf8575_ths7368_remove,
	.id_table       = pcf8575_ths7368_id,
};

static int __init ti816x_ths7368_init(void)
{
	i2c_add_driver(&pcf8575_ths7368_driver);
	return 0;
}
module_init(ti816x_ths7368_init);

static void __exit ti816x_ths7368_exit(void)
{
	i2c_del_driver(&pcf8575_ths7368_driver);
}
module_exit(ti816x_ths7368_exit);

MODULE_LICENSE("GPL");

