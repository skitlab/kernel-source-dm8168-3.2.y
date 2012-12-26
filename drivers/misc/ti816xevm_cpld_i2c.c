#include <linux/init.h>
#include <linux/module.h>

#include <linux/i2c.h>

#define TI816X_EVM_CIR_UART BIT(5)

static const struct i2c_device_id cpld_reg_ids[] = {
		{ "cpld_reg0", 0, },
		{ },
};

MODULE_DEVICE_TABLE(i2c, cpld_reg_ids);

static struct i2c_client *cpld_reg0_client;

/* CPLD Register 0 Client: used for I/O Control */
static int cpld_reg0_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	u8 data;
	struct i2c_msg msg[2] = {
		{
			.addr = client->addr,
			.flags = I2C_M_RD,
			.len = 1,
			.buf = &data,
		},
		{
			.addr = client->addr,
			.flags = 0,
			.len = 1,
			.buf = &data,
		},
	};

	cpld_reg0_client = client;

	/* Clear UART CIR to enable cir operation. */
		i2c_transfer(client->adapter, msg, 1);
		data &= ~(TI816X_EVM_CIR_UART);
		i2c_transfer(client->adapter, msg + 1, 1);
	return 0;
}

static struct i2c_driver ti816xevm_cpld_driver = {
	.driver.name    = "cpld_reg0",
	.id_table       = cpld_reg_ids,
	.probe          = cpld_reg0_probe,
};

static int __init cpld_reg0_init(void) {
	i2c_add_driver(&ti816xevm_cpld_driver);
	return 0;
}

static void __exit cpld_reg0_exit(void) {
	i2c_del_driver(&ti816xevm_cpld_driver);
}

module_init(cpld_reg0_init);
module_exit(cpld_reg0_exit);

MODULE_DESCRIPTION("CPLD driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:cpld_reg0");
