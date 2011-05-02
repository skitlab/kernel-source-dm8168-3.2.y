/*
 * SmartReflex Voltage Control driver
 *
 * Copyright (C) 2011 Texas Instruments, Inc. - http://www.ti.com/
 * Author: AnilKumar Ch <anilkumar@ti.com>
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
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
#include <linux/regulator/consumer.h>

#include <plat/ti81xx.h>
#include <plat/irqs-ti81xx.h>
#include <plat/smartreflex.h>

#define MARGIN			0
#define SENS_PER_VDOMAIN	2
#define	NUM_VOLTAGE_DOMAINS	1

struct ti816x_sr_sensors {
	char				*name;
	struct clk			*fck;
	u32				nvalue;
	s32				e2v_gain;
	u32				base_addr;
	s32				irq_processed;
	struct timer_list		timer;
};

struct ti816x_sr {
	int				is_autocomp_active;
	u32				init_volt;
	u32				curr_volt;
	int				nvalue_count;
	int				sr_irq_delay;
	struct ti816x_sr_sensors	sen[SENS_PER_VDOMAIN];
	struct regulator		*reg;
	struct work_struct		work;
};

static struct ti816x_sr srcore = {
	.sen[0] = {
		.name		= "hvt",
		.nvalue		= 0,
		.e2v_gain	= 195,
		.base_addr	= TI816X_SR0_BASE,
	},
	.sen[1] = {
		.name		= "svt",
		.nvalue		= 0,
		.e2v_gain	= 279,
		.base_addr	= TI816X_SR1_BASE,
	},
	.curr_volt		= 0,
	.init_volt		= 0,
	.nvalue_count		= SENS_PER_VDOMAIN * NUM_VOLTAGE_DOMAINS,
	.is_autocomp_active	= 0,
	.sr_irq_delay		= 2000,
};

static inline void sr_write_reg(u32 srbase, int offset, u32 value)
{
	omap_writel(value, srbase + offset);
}

static inline void sr_modify_reg(u32 srbase, int offset, u32 mask,
				u32 value)
{
	u32 reg_val;

	reg_val = omap_readl(srbase + offset);
	reg_val &= ~mask;
	reg_val |= (value&mask);

	omap_writel(reg_val, srbase + offset);
}

static inline u32 sr_read_reg(u32 srbase, int offset)
{
	return omap_readl(srbase + offset);
}

static int get_errvolt(s32 srid)
{
	u32 srbase, senerror_reg;
	s32 e2vgain, mvoltage;
	s8 terror;

	if (srid == SRHVT) {
		srbase	= srcore.sen[0].base_addr;
		e2vgain = srcore.sen[0].e2v_gain;
	} else {
		srbase	= srcore.sen[1].base_addr;
		e2vgain = srcore.sen[1].e2v_gain;
	}
	senerror_reg = sr_read_reg(srbase, SENERROR_V2);
	senerror_reg = (senerror_reg & 0x0000FF00);
	senerror_reg = senerror_reg >> 8;

	terror = senerror_reg & 0x000000FF;

	/* convert from binary to % error x 1000mv */
	mvoltage = ((terror + MARGIN) * e2vgain) >> 7;

	return mvoltage * 1000;
}

static void set_voltage(struct work_struct *work)
{
	u32 prev_volt, new_volt;
	s32 hvt_dvolt, svt_dvolt;

	/* Get the current voltage from GPIO */
	prev_volt = regulator_get_voltage(srcore.reg);

	hvt_dvolt = get_errvolt(SRHVT);
	svt_dvolt = get_errvolt(SRSVT);

	if (hvt_dvolt > svt_dvolt)
		new_volt = prev_volt + hvt_dvolt;
	else
		new_volt = prev_volt + svt_dvolt;

	/* SRCONFIG - disable SR */
	sr_modify_reg(srcore.sen[0].base_addr, SRCONFIG,
			SRCONFIG_SRENABLE, ~SRCONFIG_SRENABLE);

	/* SRCONFIG - disable SR */
	sr_modify_reg(srcore.sen[1].base_addr, SRCONFIG,
			SRCONFIG_SRENABLE, ~SRCONFIG_SRENABLE);

	regulator_set_voltage(srcore.reg, new_volt, new_volt);

	/* SRCONFIG - enable SR */
	sr_modify_reg(srcore.sen[0].base_addr, SRCONFIG,
			SRCONFIG_SRENABLE, SRCONFIG_SRENABLE);

	/* SRCONFIG - enable SR */
	sr_modify_reg(srcore.sen[1].base_addr, SRCONFIG,
			SRCONFIG_SRENABLE, SRCONFIG_SRENABLE);
}


static void irq_sr_timer(unsigned long data)
{
	/* Enable the interrupt */
	sr_modify_reg(srcore.sen[(int)data].base_addr, IRQENABLE_SET,
				IRQENABLE_MCUBOUNDSINT,
				IRQENABLE_MCUBOUNDSINT);
}

static irqreturn_t srcore_class2_irq(int irq, void *dev_id)
{
	int idx;

	if (irq == TI81XX_IRQ_SMRFLX0)
		idx = 0;
	else
		idx = 1;
	srcore.sen[idx].irq_processed = 1;

	/* Clear MCUBounds Interrupt */
	sr_modify_reg(srcore.sen[idx].base_addr, IRQSTATUS,
				IRQSTATUS_MCBOUNDSINT,
				IRQSTATUS_MCBOUNDSINT);

	/* Disable the interrupt and enable after timer expires */
	sr_modify_reg(srcore.sen[idx].base_addr, IRQENABLE_CLR,
				IRQENABLE_MCUBOUNDSINT,
				IRQENABLE_MCUBOUNDSINT);

	if (!timer_pending(&srcore.sen[idx].timer)) {
			srcore.sen[idx].timer.data = (unsigned long)idx;
			srcore.sen[idx].timer.function = irq_sr_timer;
			srcore.sen[idx].timer.expires = jiffies +
				msecs_to_jiffies(srcore.sr_irq_delay);
			add_timer(&srcore.sen[idx].timer);
	} else {
		/* Timer of this interrupt should not pending*/
		BUG();
	}

	if ((srcore.sen[0].irq_processed == 1) &&
			(srcore.sen[1].irq_processed == 1)) {
		schedule_work(&srcore.work);
		srcore.sen[0].irq_processed = 0;
		srcore.sen[1].irq_processed = 0;
	}
	return IRQ_HANDLED;
}

static int sr_clk_enable(struct clk *fck)
{
	if (clk_enable(fck) != 0) {
		pr_err("Could not enable sr_fck\n");
		return -EINVAL;
	}

	return 0;
}

static int sr_clk_disable(struct clk *fck)
{
	clk_disable(fck);

	return 0;
}

static int sr_set_nvalues(struct ti816x_sr *sr)
{
	/* Read HVT values for CORE from EFUSE */
	sr->sen[0].nvalue = omap_readl(CONTROL_FUSE_SMRT_HVT) & 0xFFFFFF;

	/* Read SVT values for CORE from EFUSE */
	sr->sen[1].nvalue = omap_readl(CONTROL_FUSE_SMRT_SVT) & 0xFFFFFF;

	if ((sr->sen[0].nvalue == 0) || (sr->sen[1].nvalue == 0))
		return -EINVAL;

	return 0;
}

static void sr_configure(u32 srbase, s32 srid)
{
	u32 sr_config;

	/* Enable MCUDisableAck Interrupt */
	sr_modify_reg(srbase, IRQSTATUS_RAW,
			IRQSTATUSRAW_MCUDISABLEACKINT,
			IRQSTATUSRAW_MCUDISABLEACKINT);

	sr_config = SRCONFIG_ACCUM_DATA |
			SRCLKLENGTH_27MHZ_SYSCLK |
			SRCONFIG_SENENABLE | SRCONFIG_ERRGEN_EN |
			SRCONFIG_MINMAXAVG_EN |
			SRCONFIG_SENNENABLE |
			SRCONFIG_SENPENABLE;

	sr_write_reg(srbase, SRCONFIG, sr_config);

	sr_write_reg(srbase, AVGWEIGHT, AVGWEIGHT_SENPAVGWEIGHT_MASK |
			AVGWEIGHT_SENNAVGWEIGHT_MASK);

	if (srid == SRHVT)
		sr_modify_reg(srbase, ERRCONFIG_V2, (SR_ERRWEIGHT_MASK |
			SR_ERRMAXLIMIT_MASK | SR_ERRMINLIMIT_MASK),
			ERRCONFIG_ERRWEIGHT | ERRCONFIG_ERRMAXLIMIT |
			ERRCONFIG_HVT_ERRMINLIMIT);
	else
		sr_modify_reg(srbase, ERRCONFIG_V2, (SR_ERRWEIGHT_MASK |
			SR_ERRMAXLIMIT_MASK | SR_ERRMINLIMIT_MASK),
			ERRCONFIG_ERRWEIGHT | ERRCONFIG_ERRMAXLIMIT |
			ERRCONFIG_SVT_ERRMINLIMIT);
}

static void sr_enable(u32 srbase, u32 nvalue)
{
	/* SRCONFIG - disable SR */
	sr_modify_reg(srbase, SRCONFIG, SRCONFIG_SRENABLE,
					~SRCONFIG_SRENABLE);

	if (nvalue == 0)
		pr_err("OPP doesn't support SmartReflex\n");

	sr_write_reg(srbase, NVALUERECIPROCAL, nvalue);

	/* Clear MCUDisableAck Interrupt */
	sr_modify_reg(srbase, IRQSTATUS,
			IRQSTATUS_MCUDISABLEACKINT,
			IRQSTATUS_MCUDISABLEACKINT);
	/* Disable MCUDisableAck interrupt */
	sr_modify_reg(srbase, IRQENABLE_CLR,
			IRQSTATUS_MCUDISABLEACKINT,
			IRQSTATUS_MCUDISABLEACKINT);
	/* Enable the interrupt */
	sr_modify_reg(srbase, IRQENABLE_SET,
			IRQENABLE_MCUBOUNDSINT,
			IRQENABLE_MCUBOUNDSINT);

	/* SRCONFIG - enable SR */
	sr_modify_reg(srbase, SRCONFIG, SRCONFIG_SRENABLE,
					SRCONFIG_SRENABLE);
}

static void sr_disable(u32 srbase)
{
	/* Disable the interrupt */
	sr_modify_reg(srbase, IRQENABLE_CLR,
			IRQENABLE_MCUBOUNDSINT,
			IRQENABLE_MCUBOUNDSINT);
	/* SRCONFIG - disable SR */
	sr_modify_reg(srbase, SRCONFIG, SRCONFIG_SRENABLE,
					~SRCONFIG_SRENABLE);
}

static void sr_start_coreautocomp(struct ti816x_sr *sr)
{
	if ((sr->sen[0].nvalue == 0) || (sr->sen[1].nvalue == 0)) {
		pr_err("SmartReflex Driver: Un-Characterized"
					" silicon found\n");
		return;
	}

	if (sr->is_autocomp_active == 1) {
		pr_warning("SR VDD autocomp is already active\n");
		return;
	}

	init_timer(&sr->sen[0].timer);
	init_timer(&sr->sen[1].timer);

	sr_clk_enable(sr->sen[0].fck);
	sr_clk_enable(sr->sen[1].fck);
	sr_configure(sr->sen[0].base_addr, SRHVT);
	sr_configure(sr->sen[1].base_addr, SRSVT);

	sr_enable(sr->sen[0].base_addr, sr->sen[0].nvalue);
	sr_enable(sr->sen[1].base_addr, sr->sen[1].nvalue);

	sr->is_autocomp_active = 1;
}

static void sr_stop_coreautocomp(struct ti816x_sr *sr)
{
	if (sr->is_autocomp_active == 0) {
		pr_warning("SR VDD autocomp is not active\n");
		return;
	}

	del_timer_sync(&sr->sen[0].timer);
	del_timer_sync(&sr->sen[1].timer);
	sr->is_autocomp_active = 0;
	sr_disable(sr->sen[0].base_addr);
	sr_disable(sr->sen[1].base_addr);
	sr_clk_disable(sr->sen[0].fck);
	sr_clk_disable(sr->sen[1].fck);

	sr->curr_volt = sr->init_volt;
	regulator_set_voltage(srcore.reg, sr->init_volt, sr->init_volt);
}

/* Sysfs interface to select SR CORE auto compensation */
static ssize_t sr_core_autocomp_show(struct kobject *kobj,
					struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", srcore.is_autocomp_active);
}

static ssize_t sr_core_autocomp_store(struct kobject *kobj,
					struct kobj_attribute *attr,
					const char *buf, size_t n)
{
	unsigned short value;

	if (sscanf(buf, "%hu", &value) != 1 || (value > 1)) {
		pr_err("sr_core_autocomp: Invalid value\n");
		return -EINVAL;
	}

	if (value == 0)
		sr_stop_coreautocomp(&srcore);
	else
		sr_start_coreautocomp(&srcore);

	return n;
}

static struct kobj_attribute sr_core_autocomp = {
	.attr = {
		.name = __stringify(sr_core_autocomp),
		.mode = 0644,
		},
	.show = sr_core_autocomp_show,
	.store = sr_core_autocomp_store,
};

static int sr_debugfs_entires(struct ti816x_sr *sr_info)
{
	struct dentry *dbg_dir, *sen_dir;
	int i;

	dbg_dir = debugfs_create_dir("smartreflex", NULL);
	if (IS_ERR(dbg_dir)) {
		pr_err("Unable to create debugfs directory\n");
		return PTR_ERR(dbg_dir);
	}

	(void) debugfs_create_u32("initial_voltage", S_IRUGO, dbg_dir,
				&sr_info->init_volt);
	(void) debugfs_create_u32("current_voltage", S_IRUGO, dbg_dir,
				&sr_info->curr_volt);
	(void) debugfs_create_x32("interrupt_delay", S_IRUGO | S_IWUGO,
				dbg_dir, &sr_info->sr_irq_delay);

	for (i = 0; i < sr_info->nvalue_count; i++) {

		sen_dir = debugfs_create_dir(sr_info->sen[i].name, dbg_dir);
		if (IS_ERR(sen_dir)) {
			pr_err("Unable to create debugfs directory\n");
			return PTR_ERR(sen_dir);
		}

		(void) debugfs_create_x32("err2voltgain", S_IRUGO | S_IWUGO,
					sen_dir, &sr_info->sen[i].e2v_gain);
		(void) debugfs_create_x32("nvalue", S_IRUGO | S_IWUGO,
					sen_dir, &sr_info->sen[i].nvalue);
	}
	return 0;
}

/* SR driver init */
static int __init sr_class2_init(void)
{
	int ret = 0;
	struct regulator *reg;

	INIT_WORK(&srcore.work, set_voltage);

	srcore.sen[0].fck = clk_get(NULL, "smartreflex_corehvt_fck");
	if (IS_ERR(srcore.sen[0].fck)) {
		pr_err("Could not get corehvt_fck\n");
		return PTR_ERR(srcore.sen[0].fck);
	}

	srcore.sen[1].fck = clk_get(NULL, "smartreflex_coresvt_fck");
	if (IS_ERR(srcore.sen[1].fck)) {
		pr_err("Could not get coresvt_fck\n");
		ret = PTR_ERR(srcore.sen[1].fck);
		goto fail_svt_clk_get;
	}

	ret = sr_set_nvalues(&srcore);
	if (ret) {
		ret = sr_debugfs_entires(&srcore);
		if (ret) {
			pr_err("Debug FS entires are created\n");
			goto fail_hvt_req_irq;
		}
	}

	reg = regulator_get(NULL, "vdd_avs");
	if (!IS_ERR(reg))
		srcore.reg = reg;

	/* Read current GPIO value and voltage */
	srcore.init_volt = regulator_get_voltage(srcore.reg);
	srcore.curr_volt = srcore.init_volt;

	ret = request_irq(TI81XX_IRQ_SMRFLX0, srcore_class2_irq,
				IRQF_DISABLED, "sr1", NULL);
	if (ret) {
		pr_err("Could not install SR1 ISR\n");
		goto fail_hvt_req_irq;
	}

	ret = request_irq(TI81XX_IRQ_SMRFLX1, srcore_class2_irq,
				IRQF_DISABLED, "sr2", NULL);
	if (ret) {
		pr_err("Could not install SR2 ISR\n");
		goto fail_svt_req_irq;
	}

	ret = sysfs_create_file(power_kobj, &sr_core_autocomp.attr);
	if (ret) {
		pr_err("subsys_create_file failed: %d\n", ret);
		goto fail_sysfs;
	}
	return ret;

fail_sysfs:
	free_irq(TI81XX_IRQ_SMRFLX1, NULL);
fail_svt_req_irq:
	free_irq(TI81XX_IRQ_SMRFLX0, NULL);
fail_hvt_req_irq:
	clk_put(srcore.sen[1].fck);
fail_svt_clk_get:
	clk_put(srcore.sen[0].fck);
	return ret;
}

static void __exit sr_class2_exit(void)
{
	clk_put(srcore.sen[0].fck);
	clk_put(srcore.sen[1].fck);

	free_irq(TI81XX_IRQ_SMRFLX0, NULL);
	free_irq(TI81XX_IRQ_SMRFLX1, NULL);

	sysfs_remove_file(power_kobj, &sr_core_autocomp.attr);
}

MODULE_AUTHOR("Texas Instruments, Inc.");
MODULE_DESCRIPTION("SmartReflex Class2 driver");
MODULE_LICENSE("GPL");

module_init(sr_class2_init);
module_exit(sr_class2_exit);
