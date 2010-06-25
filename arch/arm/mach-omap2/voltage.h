/*
 * OMAP Voltage Management Routines
 *
 * Author: Thara Gopinath	<thara@ti.com>
 *
 * Copyright (C) 2009 Texas Instruments, Inc.
 * Thara Gopinath <thara@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ARCH_ARM_MACH_OMAP2_VOLTAGE_H
#define __ARCH_ARM_MACH_OMAP2_VOLTAGE_H

extern u32 enable_sr_vp_debug;
extern struct dentry *pm_dbg_main_dir;

/* VoltageDomain instances */
#define VDD1	0
#define VDD2	1

#define VOLTSCALE_VPFORCEUPDATE		1
#define VOLTSCALE_VCBYPASS		2

/* Voltage SR Parameters for OMAP3*/
#define OMAP3_SRI2C_SLAVE_ADDR			0x12
#define OMAP3_VDD1_SR_CONTROL_REG		0x00
#define OMAP3_VDD2_SR_CONTROL_REG		0x01

/*
 * Omap3 VP register specific values. Maybe these need to come from
 * board file or PMIC data structure
 */
#define OMAP3_VP_CONFIG_ERROROFFSET		0x00
#define	OMAP3_VP_VSTEPMIN_SMPSWAITTIMEMIN	0x3C
#define OMAP3_VP_VSTEPMIN_VSTEPMIN		0x1
#define OMAP3_VP_VSTEPMAX_SMPSWAITTIMEMAX	0x3C
#define OMAP3_VP_VSTEPMAX_VSTEPMAX		0x04
#define OMAP3_VP_VLIMITTO_TIMEOUT_US		0x200

/*
 * Omap3430 specific VP register values. Maybe these need to come from
 * board file or PMIC data structure
 */
#define OMAP3430_VP1_VLIMITTO_VDDMIN		0x14
#define OMAP3430_VP1_VLIMITTO_VDDMAX		0x42
#define OMAP3430_VP2_VLIMITTO_VDDMAX		0x2C
#define OMAP3430_VP2_VLIMITTO_VDDMIN		0x18

/*
 * Omap3630 specific VP register values. Maybe these need to come from
 * board file or PMIC data structure
 */
#define OMAP3630_VP1_VLIMITTO_VDDMIN		0x18
#define OMAP3630_VP1_VLIMITTO_VDDMAX		0x3C
#define OMAP3630_VP2_VLIMITTO_VDDMIN		0x18
#define OMAP3630_VP2_VLIMITTO_VDDMAX		0x30

/* TODO OMAP4 VP register values if the same file is used for OMAP4*/

/**
 * omap_volt_data - Omap voltage specific data.
 *
 * @voltage_nominal	: The possible voltage value in uV
 * @sr_nvalue		: Smartreflex N target value at voltage <voltage>
 * @sr_errminlimit	: Error min limit value for smartreflex. This value
 *			  differs at differnet opp and thus is linked
 *			  with voltage.
 * @vp_errorgain	: Error gain value for the voltage processor. This
 *			  field also differs according to the voltage/opp.
 */
struct omap_volt_data {
	u32	volt_nominal;
	u32	sr_nvalue;
	u8	sr_errminlimit;
	u8	vp_errgain;
};

/**
 * omap_volt_pmic_info - PMIC specific data required by the voltage driver.
 *
 * @slew_rate	: PMIC slew rate (in uv/us)
 * @step_size	: PMIC voltage step size (in uv)
 */
struct omap_volt_pmic_info {
      int slew_rate;
      int step_size;
};

/* Various voltage controller related info */
struct omap_volt_vc_data {
	u16 clksetup;
	u16 voltsetup_time1;
	u16 voltsetup_time2;
	u16 voltoffset;
	u16 voltsetup2;
/* PRM_VC_CMD_VAL_0 specific bits */
	u16 vdd0_on;
	u16 vdd0_onlp;
	u16 vdd0_ret;
	u16 vdd0_off;
/* PRM_VC_CMD_VAL_1 specific bits */
	u16 vdd1_on;
	u16 vdd1_onlp;
	u16 vdd1_ret;
	u16 vdd1_off;
};

unsigned long omap_voltageprocessor_get_curr_volt(int vp_id);
void omap_voltageprocessor_enable(int vp_id);
void omap_voltageprocessor_disable(int vp_id);
int omap_voltage_scale(int vdd, unsigned long target_volt);
void omap_reset_voltage(int vdd);
int omap_get_voltage_table(int vdd, struct omap_volt_data **volt_data);
struct omap_volt_data *omap_get_volt_data(int vdd, unsigned long volt);
void omap_voltage_register_pmic(struct omap_volt_pmic_info *pmic_info);
unsigned long get_curr_voltage(int vdd);
#ifdef CONFIG_PM
void omap_voltage_init_vc(struct omap_volt_vc_data *setup_vc);
#else
static inline void omap_voltage_init_vc(struct omap_volt_vc_data *setup_vc) {}
#endif

#endif
