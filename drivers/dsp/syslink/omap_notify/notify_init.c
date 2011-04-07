/*
 * notify_init.c
 *
 * Syslink support functions for TI OMAP processors.
 *
 * Copyright (C) 2008-2009 Texas Instruments, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */


#include <linux/spinlock.h>
#include <linux/semaphore.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/list.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <asm/pgtable.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/slab.h>

/*Linux specific headers*/
#include <plat/iommu.h>
#include <plat/iovmm.h>
#include <asm/page.h>
#include <linux/pagemap.h>
#include <linux/platform_device.h>
#include <linux/clk.h>

#include <syslink/drv_notify.h>
#include <syslink/notify_driver.h>
#include <syslink/notify.h>
#include <syslink/notify_shm_drv.h>

/** ============================================================================
 *  Macros and types
 *  ============================================================================
 */
/*!
 *  @brief  Page size constants
 */
#define PAGE_SIZE_4KB             0x1000
/*!
 *  @brief  Page size constants
 */
#define PAGE_SIZE_64KB            0x10000
/*!
 *  @brief  Page size constants
 */
#define PAGE_SIZE_1MB             0x100000
/*!
 *  @brief  Page size constants
 */
#define PAGE_SIZE_16MB            0x1000000


#define NOTIFY_MMUPAGE_ALIGN(size, psz)  (((size) + psz - 1) & ~(psz - 1))
#define MMU_CAM_PRESERVE          (1 << 3)
#define MMU_CAM_VALID             (1 << 2)
#define MMU_RAM_DEFAULT         0
#define IOPTE_SHIFT     12
#define IOPTE_SIZE      (1 << IOPTE_SHIFT)
#define IOPTE_MASK      (~(IOPTE_SIZE - 1))
#define IOPAGE_MASK     IOPTE_MASK

/* Maximum number of user supported.*/
#define MAX_PROCESSES 32

static  int  syslink_notify_phys_addr ;
module_param_named(notify_pa, syslink_notify_phys_addr, int,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(syslink_notify_phys_addr,
			"Specify the physical address where the notify drivers"
			"will be created. ignore notify driver creation  is not"
			"required at kernel boot time ");

#if (defined(CONFIG_ARCH_OMAP3) || defined(CONFIG_ARCH_TI816X))
static bool  create_dsp_driver;
module_param_named(dsp, create_dsp_driver, bool,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(test, "Specify the physical address where the notify drivers"
			"will be created."
			"kernel boot time creation is not required");
#endif

/* IOMMU Exported function */
extern
void
iopgtable_lookup_entry(struct iommu *obj, u32 da, u32 **ppgd, u32 **ppte);

static struct iommu *notify_iommu_iva2 ;

struct notify_map_table_info {
	u32    actualAddress;
	/*!< Actual address( physical address) */
	u32    mappedAddress;
	/*!< Mapped address */
	u32    size;
	/*!< Size of the region mapped */
	bool      isCached;
	/*!< Is cached? */
} ;

/*Mapped memory information  associated with each remote proc */
static struct notify_map_table_info *notify_map_info;

#define REG(x)              (*((volatile u32 *) (x)))
#define MEM(x)              (*((volatile u32 *) (x)))
#define SYSTEM_CONTROL_ADDR     0x48002000
#define SYSTEM_CONTROL_SIZE     0xC00
#define CONTROL_REVISION        0x00000000
#define CONTROL_IVA2_BOOTADDR   0x00000400
#define CONTROL_IVA2_BOOTMOD    0x00000404

#define PRCM_IVA2_CM_ADDR       0x48004000
#define PRCM_IVA2_CM_SIZE       0x800
#define CM_FCLKEN_IVA2          0x00000000
#define CM_CLKEN_PLL_IVA2       0x00000004
#define CM_IDLEST_IVA2          0x00000020
#define CM_AUTOIDLE_PLL_IVA2    0x00000034
#define CM_CLKSEL1_PLL_IVA2     0x00000040
#define CM_CLKSEL2_PLL_IVA2     0x00000044
#define CM_CLKSTCTRL_IVA2       0x00000048

#define PRCM_IVA2_PRM_ADDR      0x48306000
#define PRCM_IVA2_PRM_SIZE      0x800
#define RM_RSTCTRL_IVA2         0x00000050
#define RM_RSTST_IVA2           0x00000058
#define PM_WKDEP_IVA2           0x000000C8
#define PM_PWSTCTRL_IVA2        0x000000E0
#define PM_PWSTST_IVA2          0x000000E4

#define MAX_WAIT_COUNT          0x50000
#if defined(CONFIG_ARCH_OMAP3430)

int
OMAP3530PWR_off(u32 cm, u32 prm)
{
	return 0;
}

struct clk *clk_handle ;
int
OMAP3530PWR_on(void)
{
	int status = 0;
	u32               i;
	u32               cm;
	u32               prm;

	cm      = (u32)ioremap(0x48004000, 0x800);
	prm     = (u32)ioremap(0x48306000, 0x800);

	/* release IVA2_RST2 reset (MMU, SYSC, local interconnect, etc) */
	REG(prm + RM_RSTCTRL_IVA2) &= ~(1 << 1);
	REG(prm + RM_RSTCTRL_IVA2);

	/* wait for IVA2_RST2 signal to be released */
	for (i = 0; i < MAX_WAIT_COUNT; i++) {
		if ((REG(prm + RM_RSTST_IVA2) & (1 << 9)))
			break;

	}
	clk_handle = clk_get(NULL, "iva2_ck");
	if (clk_handle == NULL)
		printk(KERN_ERR "***clk_get error\n");
	else
		printk(KERN_INFO "***clk_get handle 0x%x\n", clk_handle);

	clk_enable(clk_handle);

	/*! @retval PWRMGR_SUCCESS Operation successful */
	return status;
}

/*!
 *  @brief      This function adds an MMU entry for the specfied address and
 *              size.
 */
int
notify_add_mmu_entry(void *mmu_handle, u32 slave_virt_addr, u32 size)
{
	s32                     status = 0;
	u32  *ppgd = NULL;
	u32  *ppte = NULL;
	s32                     cur_size;
	u32                    master_phys_addr;
	struct iotlb_entry tlb_entry;

	/* Align the addresses to page size */
	size += ((u32)slave_virt_addr & (PAGE_SIZE_4KB - 1));
	master_phys_addr = (u32)slave_virt_addr;
	slave_virt_addr &= ~(PAGE_SIZE_4KB-1);
	master_phys_addr &= ~(PAGE_SIZE_4KB-1);

	/* Align the size as well */
	size = NOTIFY_MMUPAGE_ALIGN(size, PAGE_SIZE_4KB);
	cur_size = size;

	/* Check every page if exists */
	do {
		/* Lookup if the entry exists */

		iopgtable_lookup_entry(mmu_handle,
								slave_virt_addr,
								(u32 **) &ppgd,
								(u32 **) &ppte);
		if (!*ppgd || !*ppte) {
			/* Entry doesnot exists, insert this page */
			tlb_entry.pgsz  = MMU_CAM_PGSZ_4K;
			tlb_entry.prsvd  = MMU_CAM_PRESERVE;
			tlb_entry.valid  = MMU_CAM_VALID;
			tlb_entry.elsz = MMU_RAM_ELSZ_16;
			tlb_entry.endian = MMU_RAM_ENDIAN_LITTLE;
			tlb_entry.mixed = MMU_RAM_DEFAULT;
			tlb_entry.da = slave_virt_addr;
			tlb_entry.pa = master_phys_addr;
			if (iopgtable_store_entry(mmu_handle, &tlb_entry)) {
					printk(KERN_ERR "ERROR in "
					"notify_add_mmu_entry\n");
				status = -1;
			}
		} else if (*ppgd && *ppte) {
			if (master_phys_addr != (*ppte & IOPTE_MASK)) {
				/* Entry doesnot exists, insert this page */
				tlb_entry.pgsz  = MMU_CAM_PGSZ_4K;
				tlb_entry.prsvd = MMU_CAM_PRESERVE;
				tlb_entry.valid = MMU_CAM_VALID;
				tlb_entry.elsz  = MMU_RAM_ELSZ_16;
				tlb_entry.endian = MMU_RAM_ENDIAN_LITTLE;
				tlb_entry.mixed  = MMU_RAM_DEFAULT;
				tlb_entry.da = slave_virt_addr;
				tlb_entry.pa = master_phys_addr;
				if (iopgtable_store_entry(mmu_handle,
								&tlb_entry)) {
						printk(KERN_ERR "ERROR in"
						"notify_add_mmu_entry\n");
					status = -1;
				}
			}
		}
		cur_size -= PAGE_SIZE_4KB;
		slave_virt_addr += PAGE_SIZE_4KB;
		master_phys_addr += PAGE_SIZE_4KB;
	} while (cur_size);

	return status;
}
#endif

unsigned int __initdata notify_pa;

/*
 * ======== notify_init ========
 *  Initialization routine. Executed when the driver is
 *  loaded (as a kernel module), or when the system
 *  is booted (when included as part of the kernel
 *  image).
*/
static int __init notify_init(void)
{
	s32 retval = 0;
	u32     offset = 0;
	u32     memreq;
	u16     i = 0;

	OMAP3530PWR_on();

	multiproc_setup(NULL);

#if 0
	notify_iommu_iva2 = iommu_get("iva2");
	if (notify_iommu_iva2 == NULL) {
		goto setup_fail;
		printk(KERN_ERR "*****iommu_get failed\n");
	} else {
		printk(KERN_INFO "********iommu get passed  notify_iommu_iva2"
			"0x%x\n", notify_iommu_iva2);
	}
#endif
	notify_setup(NULL);

	/* Create notify drivers based on configuration*/
	retval = notify_shm_drv_setup(NULL);
	if (retval) {
		printk(KERN_ERR "notify_init : notify_shm_drv_setup failed\n");
		goto setup_fail;
	}

	notify_map_info = kmalloc(((sizeof(struct notify_map_table_info)) *
					multiproc_get_num_processors()),
					GFP_KERNEL);
	if (!notify_map_info) {
		printk(KERN_ERR "notify_init : kmalloc failed for"
				"notify_map_info\n");
		goto setup_fail;
	}

	/* Create notify drivers by calling notify_attach to each remote
	 * procid
	 */
	if (syslink_notify_phys_addr != (u32)NULL) {
		for (i = 0; i < multiproc_get_num_processors(); i++) {
			if (i != multiproc_self()) {
				notify_map_info[i].actualAddress =
					syslink_notify_phys_addr + offset;
				memreq = notify_shared_mem_req(i, (void *)
					notify_map_info[i].actualAddress);

				notify_map_info[i].size = memreq;
				offset += memreq;
			}
		}

		/* Map the the physical address to kernel virtual space  and
		 * creating notify
		 * drivers by calling notify_attach to each remote procid
		 */
		if (cpu_is_omap343x()) {
			i = multiproc_get_id("DSP");

			notify_iommu_iva2 = iommu_get("iva2");
			if (notify_iommu_iva2 == NULL) {
				goto setup_fail;
				printk(KERN_ERR "*****iommu_get failed\n");
			}


			notify_add_mmu_entry(notify_iommu_iva2,
					notify_map_info[i].actualAddress,
					notify_map_info[i].size);
		}
		for (i = 0; i < multiproc_get_num_processors(); i++) {
			if (i != multiproc_self()) {
				notify_map_info[i].mappedAddress = (u32)
					ioremap_nocache ((dma_addr_t)
					notify_map_info[i].actualAddress,
					notify_map_info[i].size);

				notify_map_info[i].isCached = false;
				/* Calling notify_attach to create driver */
				retval = notify_attach(i, (void *)
					notify_map_info[i].mappedAddress);
				if (retval < 0) {
					printk(KERN_ERR "notify_init : "
					"notify_attach failed for  remote "
					"proc id 0%d\n", i);
					goto notify_attach_fail;
				} else {
					printk(KERN_INFO "notify_init : notify "
						"driver created  for  remote "
						"proc id 0%d at physical "
						"Address 0x%x\n",
						i,
						notify_map_info[i].
								actualAddress);
				}
			}
		}
	}



	return retval;

notify_attach_fail:
	i -= 1;
	for (; i >= 0 ;i--)
		notify_detach(i);

	for (i = 0; i < multiproc_get_num_processors(); i++) {
		if ((i != multiproc_self()) &&
			((void *)notify_map_info[i].mappedAddress != NULL))
			iounmap((unsigned int *)
				notify_map_info[i].mappedAddress);

	}

setup_fail:
	if (!notify_map_info)
		kfree(notify_map_info);

	notify_shm_drv_destroy();

	notify_destroy();

	return retval;
}

/*
 * ======== notify_exit ========
 *  This function is invoked during unlinking of notify
 *  module from the kernel. notify resources are
 *  freed in this function.
 */
static void __exit notify_exit(void)
{
	u32   i = 0;

	for (i = 0; i < multiproc_get_num_processors(); i++) {
		if ((i != multiproc_self()) &&
			((void *)notify_map_info[i].mappedAddress != NULL))
			iounmap((unsigned int *)
				notify_map_info[i].mappedAddress);
	}
}

module_init(notify_init);
module_exit(notify_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("notify : multicore notify implementation");
MODULE_AUTHOR("Texas Instruments");
