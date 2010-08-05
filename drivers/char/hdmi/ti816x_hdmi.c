/*
 * TI816X HDMI Driver.
 *
 * Copyright (C) 2010 Texas Instruments, Incorporated
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
#include <linux/fs.h>           /*     everything... */
#include <linux/errno.h>        /*     error codes     */
#include <linux/types.h>        /*     size_t */
#include <linux/cdev.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/ti816x_hdmi.h>

#include "hdmi_cfg.h"
#include "hdmi.h"

#define TI816X_HDMI_DRIVER_NAME 	"TI816X_HDMI"

struct ti816x_hdmi_params
{
	/* Handle to library */
	void * hdmi_lib_handle;
	/* Other parameters */
	u32 wp_v_addr;
	u32 core_v_addr;
	u32 phy_v_addr;
	Vps_HdmiConfigParams *cfg;
	int i;
};

/* Global var */
struct ti816x_hdmi_params hdmi_obj;
static Vps_HdmiInitParams initParams;

#if 1
static u32 hdmi_mode = 4;
#endif
static Vps_HdmiConfigParams param720p60  = HDMI_HAL_INIT_8BIT_720P_60_16_9_HD;
static Vps_HdmiConfigParams param1080p30 = HDMI_HAL_INIT_8BIT_1080P_30_16_9_HD;
static Vps_HdmiConfigParams param1080i60 = HDMI_HAL_INIT_8BIT_1080I_60_16_9_HD;
static Vps_HdmiConfigParams param1080p60 = HDMI_HAL_INIT_8BIT_1080P_60_16_9_HD;

#if 0
module_param(hdmi_mode, uint, S_IRUGO);

MODULE_PARM_DESC(hdmi_mode, "Resolution required 1-720p60 2-1080p30 3-1080i60 4-1080p60 (default:2)");
#endif

#if 1
static void configureHdVenc1080P30(int useEmbeddedSync);
#endif

static int ti816x_hdmi_major;
static struct cdev ti816x_hdmi_cdev;
static dev_t ti816x_hdmi_dev_id;
static struct device *ti816x_hdmi_device;
static struct class *ti816x_hdmi_class = NULL;

static void ti816x_hdmi_platform_release(struct device *device);
static int __init ti816x_hdmi_probe(struct device *device);
static int ti816x_hdmi_remove(struct device *device);
static int ti816x_hdmi_open(struct inode *inode, struct file *filp);
static int ti816x_hdmi_release(struct inode *inode, struct file *filp);
static int ti816x_hdmi_ioctl(struct inode *inode, struct file *file,
                      unsigned int cmd, unsigned long arg);

static struct file_operations ti816x_hdmi_fops = {
        .owner = THIS_MODULE,
        .open = ti816x_hdmi_open,
        .release = ti816x_hdmi_release,
        .ioctl = ti816x_hdmi_ioctl,
};

static struct device_driver ti816x_hdmi_driver = {
        .name = TI816X_HDMI_DRIVER_NAME,
        .bus = &platform_bus_type,
        .probe = ti816x_hdmi_probe,
        .remove = ti816x_hdmi_remove,
};

static struct platform_device ti816x_hdmi_plat_device = {
        .name = TI816X_HDMI_DRIVER_NAME,
        .id = 2,
        .dev = {
           .release = ti816x_hdmi_platform_release,
        }
};

/*
 * ti816x_hdmi_open: This function opens hdmi driver.
 */
static int ti816x_hdmi_open(struct inode *inode, struct file *filp)
{
	int ret = 0;

	/* Call library to open HDMI */
	if (hdmi_obj.cfg != NULL){
		hdmi_obj.hdmi_lib_handle = VpsHal_hdmiOpen(0,hdmi_obj.cfg,&ret,0x0);

	}

	if ((ret == 0x0) && (hdmi_obj.hdmi_lib_handle != NULL)) {

		printk("TI816x_hdmi: Opend\n");
		filp->private_data = &hdmi_obj;

		ret = VpsHal_hdmiStart(hdmi_obj.hdmi_lib_handle, 0x0);
	}
	else{
		printk("TI816x_hdmi: Could not open\n");
	}

	return ret;
}

/*
 * ti816x_hdmi_release: This function releases hdmi driver.
 */
static int ti816x_hdmi_release(struct inode *inode, struct file *filp)
{
	int ret = 0;
	struct ti816x_hdmi_params *params =
		(struct ti816x_hdmi_params *)filp->private_data;

	printk("TI816x_hdmi: Release\n");

	/* Call close of the library */
	if (!ret) {
		if (params) {
			/* kfree(params);*/
			filp->private_data = NULL;
		}
	}

	return ret;
}

/*
 * ti816x_hdmi_ioctl: This function will process IOCTL commands sent by
 * the application.
 */
static int ti816x_hdmi_ioctl(struct inode *inode, struct file *file,
                      unsigned int cmd, unsigned long arg)
{
	printk("TI816x_hdmi: Ioctl\n");
	return 0;
}

static void ti816x_hdmi_platform_release(struct device *device)
{
        /* this is called when the reference count goes to zero */
}
static int __init ti816x_hdmi_probe(struct device *device)
{
	printk("TI816x_hdmi: probe\n");
        return 0;
}
static int ti816x_hdmi_remove(struct device *device)
{
	printk("TI816x_hdmi: remove\n");
        return 0;
}

/**
 * ti816x_hdmi_init() - Initialize TI816X HDMI Driver
 */
int __init ti816x_hdmi_init(void)
{
	int result;

	if ((hdmi_mode < 0x1) || (hdmi_mode > 0x4)){
		printk("\n In correct resolution selected \n");
		printk("1-> 720p60 2-> 1080p30 3-> 1080i60 4->1080p60\n");
		result = -1;
		goto err_exit;
	}
	/* Get the major number for this module */
	result = alloc_chrdev_region(&ti816x_hdmi_dev_id, 0, 1, TI816X_HDMI_DRIVER_NAME);
	if (result) {
		printk("TI816x_HDMI: Cound not register region\n");
		goto err_exit;
	}

	ti816x_hdmi_major = MAJOR(ti816x_hdmi_dev_id);
/*     printk("Major Number %d MinorNumber %d\n",  MAJOR(ti816x_hdmi_dev_id),  MINOR(ti816x_hdmi_dev_id));*/

	/* initialize character device */
	cdev_init(&ti816x_hdmi_cdev, &ti816x_hdmi_fops);
	ti816x_hdmi_cdev.owner = THIS_MODULE;
	ti816x_hdmi_cdev.ops = &ti816x_hdmi_fops;

	/* add char driver */
	result = cdev_add(&ti816x_hdmi_cdev, ti816x_hdmi_dev_id, 1);
	if (result) {
		printk("TI816x_hdmi: Could not add hdmi char driver\n");
		goto err_remove_region;
	}

        /* register driver as a platform driver */
	result = driver_register(&ti816x_hdmi_driver);
        if (result) {
		printk("TI816x_hdmi: Cound register driver\n");
                goto err_remove_cdev;
        }

        /* register the drive as a platform device */
	result = platform_device_register(&ti816x_hdmi_plat_device);
        if (result) {
		printk("TI816x_hdmi: Cound register as platform device\n");
                goto err_driver_unregister;
        }

	ti816x_hdmi_class = class_create(THIS_MODULE, TI816X_HDMI_DRIVER_NAME);
	if (IS_ERR(ti816x_hdmi_class)) {
		result = -EIO;
		printk("TI816x_hdmi: Could not create class\n");
		goto err_remove_platform_device;
	}

	ti816x_hdmi_device = device_create(ti816x_hdmi_class, NULL,
				ti816x_hdmi_dev_id, NULL,
				TI816X_HDMI_DRIVER_NAME);
	if(IS_ERR(ti816x_hdmi_device)) {
		result = -EIO;
		printk("TI816x_hdmi: Cound not create device file\n");
		goto err_remove_class;
	}

	/* Initialize the global strucutres... */
	hdmi_obj.hdmi_lib_handle = NULL;
	hdmi_obj.wp_v_addr = (int) ioremap(HDMI_WP_0_REGS, sizeof(CSL_Hdmi_WpRegs) + 1);
	if (hdmi_obj.wp_v_addr == 0x0){
		printk("TI816x_hdmi: Could not ioremap for WP\n");
		goto err_remove_class;
	} else {
/*		printk("Wrapper at address %x", hdmi_obj.wp_v_addr);*/
	}
	hdmi_obj.core_v_addr = (int) ioremap(HDMI_CORE_0_REGS, sizeof(CSL_Hdmi_CoreRegs) + 1);
	if (hdmi_obj.core_v_addr == 0x0){
		printk("TI816x_hdmi: Could not ioremap for Core\n");
		goto err_remove_class;
	} else {
/*		printk("Core at address %x", hdmi_obj.core_v_addr);*/
	}

	hdmi_obj.phy_v_addr = (int) ioremap(HDMI_PHY_0_REGS, sizeof(CSL_Hdmi_PhyRegs) + 1);
	if (hdmi_obj.phy_v_addr == 0x0){
		printk("TI816x_hdmi: Could not ioremap for PHY\n");
		goto err_remove_class;
	} else {
/*		printk("PHY at address %x", hdmi_obj.phy_v_addr);*/
	}

#if 1
	printk("hdmi :: Initializing the venc \n");
	configureHdVenc1080P30(0);
	printk("hdmi :: Venc initialized \n");
#endif

	/* Initialize the HDMI library */
	initParams.wpBaseAddr       =   (u32) hdmi_obj.wp_v_addr;
	initParams.coreBaseAddr     =   (u32) hdmi_obj.core_v_addr;
	initParams.phyBaseAddr      =   (u32) hdmi_obj.phy_v_addr;
	initParams.pllCtrlBaseAddr  =   (int) ioremap(HDMI_PLLCTRL_0_REGS, sizeof(CSL_Hdmi_PllCtrlRegs) + 1);
	if (initParams.pllCtrlBaseAddr == 0x0){
		printk("TI816x_hdmi: Could not ioremap for PLL\n");
		goto err_remove_class;
	}

	initParams.interruptNo      =   0x7;
	initParams.vencId           =   0;
	initParams.encoderId        =   0;

/*	printk("hdmi :: Initializing driver \n");*/
	if (VpsHal_hdmiInit(&initParams) != 0x0){
		printk("TI816x_hdmi: Init failed\n");
		goto err_remove_class;
	}
	switch (hdmi_mode) {
		case 1:
			hdmi_obj.cfg = &param720p60;
			printk("Configured HDMI for 720 P 60\n");
		break;
		case 2:
			hdmi_obj.cfg = &param1080p30;
			printk("Configured HDMI for 1080 P 30\n");
		break;
		case 3:
			hdmi_obj.cfg = &param1080i60;
			printk("Configured HDMI for 1080 I 60\n");
		break;
		case 4:
			hdmi_obj.cfg = &param1080p60;
			printk("Configured HDMI for 1080 P 60\n");
		break;
		default:
			hdmi_obj.cfg = NULL;
		break;
	}
	printk("hdmi :: Initialized \n");
	return 0;

err_remove_class:
	class_destroy(ti816x_hdmi_class);
err_remove_platform_device:
	platform_device_unregister(&ti816x_hdmi_plat_device);
err_driver_unregister:
	driver_unregister(&ti816x_hdmi_driver);
err_remove_cdev:
	cdev_del(&ti816x_hdmi_cdev);
err_remove_region:
	unregister_chrdev_region(ti816x_hdmi_dev_id, 1);
err_exit:
	return result;
}

/**
 * ti816x_hdmi_exit() - Perform clean before unload
 */
void __exit ti816x_hdmi_exit(void)
{
	/* TODO: unmap HDMI registers uses using iounmap api */
	device_destroy(ti816x_hdmi_class, ti816x_hdmi_dev_id);
	class_destroy(ti816x_hdmi_class);
	platform_device_unregister(&ti816x_hdmi_plat_device);
	driver_unregister(&ti816x_hdmi_driver);
	cdev_del(&ti816x_hdmi_cdev);
	unregister_chrdev_region(ti816x_hdmi_dev_id, 1);
	iounmap((int *)hdmi_obj.wp_v_addr);
	iounmap((int *)hdmi_obj.core_v_addr);
	iounmap((int *)hdmi_obj.phy_v_addr);
}

#if 1
static void configureHdVenc1080P30(int useEmbeddedSync)
{
	volatile unsigned int *vencHd_D_Base	=	NULL;
#if 0
    volatile unsigned int *clkc_mod = NULL;

    clkc_mod =  (volatile unsigned int *) ioremap(0x48100000, 0x200);

    clkc_mod = (volatile unsigned int *) (((unsigned int )clkc_mod) + 0x100);

    *clkc_mod   =   0xFFFFFFFF;

    clkc_mod = (volatile unsigned int *) (((unsigned int )clkc_mod) + 0x14);

    *clkc_mod = 0xC000E;
    clkc_mod++;
    *clkc_mod = 0xF;
#endif

    vencHd_D_Base = (volatile unsigned int *) ioremap(0x48106000, 0x80);
    if (useEmbeddedSync != 0x0)
    {
        *vencHd_D_Base = 0x4002A033;
    }
    else
    {
        *vencHd_D_Base = 0x4003A033;
    }
	vencHd_D_Base++;
	*vencHd_D_Base = 0x003F0275;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x1EA500BB;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x1F9901C2;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x1FD71E67;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x004001C2;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x00200200;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x1B6C0C77;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x1C0C0C30;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x1C0C0C30;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x84465898;    /* 0x28 */
	vencHd_D_Base++;
	*vencHd_D_Base = 0x3F000028;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x587800BF;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x00000460;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x000C39E7;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x58780118;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x0002A86D;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x00438000;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x05000000;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x00003000;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x00000000;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x58780110;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x0002A86D;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x00438000;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x05000000;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x00003000;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x00000000;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x00000000;

}
#endif

module_init(ti816x_hdmi_init);
module_exit(ti816x_hdmi_exit);
MODULE_LICENSE("GPL");

