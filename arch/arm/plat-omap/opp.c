/*
 * OMAP OPP Interface
 *
 * Copyright (C) 2009-2010 Texas Instruments Incorporated.
 *	Nishanth Menon
 *	Romit Dasgupta <romit@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/cpufreq.h>

#include <plat/opp_twl_tps.h>
#include <plat/opp.h>

/**
 * struct omap_opp - OMAP OPP description structure
 * @enabled:	true/false - marking this OPP as enabled/disabled
 * @rate:	Frequency in hertz
 * @u_volt:	Nominal voltage in microvolts corresponding to this OPP
 * @opp_id:	opp identifier (deprecated)
 *
 * This structure stores the OPP information for a given domain.
 */
struct omap_opp {
	bool enabled;
	unsigned long rate;
	unsigned long u_volt;
	u8 opp_id;
};

/* This maintains pointers to the start of each OPP array. */
static struct omap_opp *_opp_list[OPP_TYPES_MAX];

/* Detect end of opp array */
#define OPP_TERM(opp) (!(opp)->rate && !(opp)->u_volt && !(opp)->enabled)

/**
 * opp_get_voltage() - Gets the voltage corresponding to an opp
 * @opp:	opp for which voltage has to be returned for
 *
 * Return voltage in micro volt corresponding to the opp, else
 * return 0
 */
unsigned long opp_get_voltage(const struct omap_opp *opp)
{
	if (unlikely(!opp || IS_ERR(opp)) || !opp->enabled) {
		pr_err("%s: Invalid parameters being passed\n", __func__);
		return 0;
	}
	return opp->u_volt;
}

/**
 * opp_get_freq() - Gets the frequency corresponding to an opp
 * @opp:	opp for which frequency has to be returned for
 *
 * Return frequency in hertz corresponding to the opp, else
 * return 0
 */
unsigned long opp_get_freq(const struct omap_opp *opp)
{
	if (unlikely(!opp || IS_ERR(opp)) || !opp->enabled) {
		pr_err("%s: Invalid parameters being passed\n", __func__);
		return 0;
	}
	return opp->rate;
}

/**
 * opp_find_by_opp_id - look up OPP by OPP ID (deprecated)
 * @opp_type:	OPP type where we want the look up to happen.
 * @opp_id:	OPP ID to search for
 *
 * Returns the struct omap_opp pointer corresponding to the given OPP
 * ID @opp_id, or returns NULL on error.
 */
struct omap_opp * __deprecated opp_find_by_opp_id(enum opp_t opp_type,
						  u8 opp_id)
{
	struct omap_opp *opps;
	int i = 0;

	if (unlikely(opp_type >= OPP_TYPES_MAX || !opp_id))
		return ERR_PTR(-EINVAL);
	opps = _opp_list[opp_type];

	if (!opps)
		return ERR_PTR(-ENOENT);

	while (!OPP_TERM(&opps[i])) {
		if (opps[i].enabled && (opps[i].opp_id == opp_id))
			return &opps[i];

		i++;
	}

	return ERR_PTR(-ENOENT);
}

/**
 * opp_get_opp_id() - Provide OPP ID corresponding to an OPP (deprecated)
 * @opp:	opp for which frequency has to be returned for
 *
 * Returns an OPP ID for the OPP required, if error, returns 0
 */
u8 __deprecated opp_get_opp_id(struct omap_opp *opp)
{
	if (unlikely(!opp || IS_ERR(opp)) || !opp->enabled) {
		pr_err("%s: Invalid parameter being passed\n", __func__);
		return 0;
	}
	return opp->opp_id;
}

/**
 * opp_get_opp_count() - Get number of opps enabled in the opp list
 * @opp_type:	OPP type we want to count
 *
 * This functions returns the number of opps if there are any OPPs enabled,
 * else returns corresponding error value.
 */
int opp_get_opp_count(enum opp_t opp_type)
{
	u8 n = 0;
	struct omap_opp *oppl;

	if (unlikely(opp_type >= OPP_TYPES_MAX)) {
		pr_err("%s: Invalid parameters being passed\n", __func__);
		return -EINVAL;
	}

	oppl = _opp_list[opp_type];
	if (!oppl)
		return -ENOENT;

	while (!OPP_TERM(oppl)) {
		if (oppl->enabled)
			n++;
		oppl++;
	}
	return n;
}

/**
 * opp_find_freq_exact() - search for an exact frequency
 * @opp_type:	OPP type we want to search in.
 * @freq:	frequency to search for
 * @enabled:	enabled/disabled OPP to search for
 *
 * Searches for exact match in the opp list and returns handle to the matching
 * opp if found, else returns ERR_PTR in case of error and should be handled
 * using IS_ERR.
 *
 * Note enabled is a modifier for the search. if enabled=true, then the match is
 * for exact matching frequency and is enabled. if false, the match is for exact
 * frequency which is disabled.
 */
struct omap_opp *opp_find_freq_exact(enum opp_t opp_type,
				     unsigned long freq, bool enabled)
{
	struct omap_opp *oppl;

	if (unlikely(opp_type >= OPP_TYPES_MAX)) {
		pr_err("%s: Invalid parameters being passed\n", __func__);
		return ERR_PTR(-EINVAL);
	}

	oppl = _opp_list[opp_type];

	if (!oppl)
		return ERR_PTR(-ENOENT);

	while (!OPP_TERM(oppl)) {
		if ((oppl->rate == freq) && (oppl->enabled == enabled))
			break;
		oppl++;
	}

	return OPP_TERM(oppl) ? ERR_PTR(-ENOENT) : oppl;
}

/**
 * opp_find_freq_ceil() - Search for an rounded ceil freq
 * @opp_type:	OPP type where we want to search in
 * @freq:	Start frequency
 *
 * Search for the matching ceil *enabled* OPP from a starting freq
 * for a domain.
 *
 * Returns *opp and *freq is populated with the match, else
 * returns NULL opp if no match, else returns ERR_PTR in case of error.
 *
 * Example usages:
 *	* find match/next highest available frequency *
 *	freq = 350000;
 *	opp = opp_find_freq_ceil(OPP_MPU, &freq))
 *	if (IS_ERR(opp))
 *		pr_err("unable to find a higher frequency\n");
 *	else
 *		pr_info("match freq = %ld\n", freq);
 *
 *	* print all supported frequencies in ascending order *
 *	freq = 0; * Search for the lowest enabled frequency *
 *	while (!IS_ERR(opp = opp_find_freq_ceil(OPP_MPU, &freq)) {
 *		pr_info("freq = %ld\n", freq);
 *		freq++; * for next higher match *
 *	}
 */
struct omap_opp *opp_find_freq_ceil(enum opp_t opp_type, unsigned long *freq)
{
	struct omap_opp *oppl;

	if (unlikely(opp_type >= OPP_TYPES_MAX || !freq ||
		 IS_ERR(freq))) {
		pr_err("%s: Invalid parameters being passed\n", __func__);
		return ERR_PTR(-EINVAL);
	}

	oppl = _opp_list[opp_type];

	if (!oppl)
		return ERR_PTR(-ENOENT);

	while (!OPP_TERM(oppl)) {
		if (oppl->enabled && oppl->rate >= *freq)
			break;
		oppl++;
	}

	if (OPP_TERM(oppl))
		return ERR_PTR(-ENOENT);

	*freq = oppl->rate;

	return oppl;
}

/**
 * opp_find_freq_floor() - Search for an rounded floor freq
 * @opp_type:	OPP type we want to search in
 * @freq:	Start frequency
 *
 * Search for the matching floor *enabled* OPP from a starting freq
 * for a domain.
 *
 * Returns *opp and *freq is populated with the next match, else
 * returns NULL opp if no match, else returns ERR_PTR in case of error.
 *
 * Example usages:
 *	* find match/next lowest available frequency
 *	freq = 350000;
 *	opp = opp_find_freq_floor(OPP_MPU, &freq)))
 *	if (IS_ERR(opp))
 *		pr_err ("unable to find a lower frequency\n");
 *	else
 *		pr_info("match freq = %ld\n", freq);
 *
 *	* print all supported frequencies in descending order *
 *	freq = ULONG_MAX; * search highest enabled frequency *
 *	while (!IS_ERR(opp = opp_find_freq_floor(OPP_MPU, &freq)) {
 *		pr_info("freq = %ld\n", freq);
 *		freq--; * for next lower match *
 *	}
 */
struct omap_opp *opp_find_freq_floor(enum opp_t opp_type, unsigned long *freq)
{
	struct omap_opp *prev_opp, *oppl;

	if (unlikely(opp_type >= OPP_TYPES_MAX || !freq ||
		 IS_ERR(freq))) {
		pr_err("%s: Invalid parameters being passed\n", __func__);
		return ERR_PTR(-EINVAL);
	}
	oppl = _opp_list[opp_type];

	if (!oppl)
		return ERR_PTR(-ENOENT);

	prev_opp = oppl;
	while (!OPP_TERM(oppl)) {
		if (oppl->enabled) {
			if (oppl->rate > *freq)
				break;
			prev_opp = oppl;
		}
		oppl++;
	}

	if (prev_opp->rate > *freq)
		return ERR_PTR(-ENOENT);

	*freq = prev_opp->rate;

	return prev_opp;
}

/* wrapper to reuse converting opp_def to opp struct */
static void omap_opp_populate(struct omap_opp *opp,
			      const struct omap_opp_def *opp_def)
{
	opp->rate = opp_def->freq;
	opp->enabled = opp_def->enabled;
	opp->u_volt = opp_def->u_volt;
}

/**
 * opp_add()  - Add an OPP table from a table definitions
 * @opp_type:	OPP type under which we want to add our new OPP.
 * @opp_def:	omap_opp_def to describe the OPP which we want to add to list.
 *
 * This function adds an opp definition to the opp list and returns status.
 */
int opp_add(enum opp_t opp_type, const struct omap_opp_def *opp_def)
{
	struct omap_opp *opp, *oppt, *oppr, *oppl;
	u8 n, i, ins;

	if (unlikely(opp_type >= OPP_TYPES_MAX || !opp_def ||
			 IS_ERR(opp_def))) {
		pr_err("%s: Invalid params being passed\n", __func__);
		return -EINVAL;
	}

	n = 0;
	oppl = _opp_list[opp_type];

	if (!oppl)
		return -ENOENT;

	opp = oppl;
	while (!OPP_TERM(opp)) {
		n++;
		opp++;
	}

	/*
	 * Allocate enough entries to copy the original list, plus the new
	 * OPP, plus the concluding terminator
	 */
	oppr = kzalloc(sizeof(struct omap_opp) * (n + 2), GFP_KERNEL);
	if (!oppr) {
		pr_err("%s: No memory for new opp array\n", __func__);
		return -ENOMEM;
	}

	/* Simple insertion sort */
	opp = _opp_list[opp_type];
	oppt = oppr;
	ins = 0;
	i = 1;
	do {
		if (ins || opp->rate < opp_def->freq) {
			memcpy(oppt, opp, sizeof(struct omap_opp));
			opp++;
		} else {
			omap_opp_populate(oppt, opp_def);
			ins++;
		}
		oppt->opp_id = i;
		oppt++;
		i++;
	} while (!OPP_TERM(opp));

	/* If nothing got inserted, this belongs to the end */
	if (!ins) {
		omap_opp_populate(oppt, opp_def);
		oppt->opp_id = i;
		oppt++;
	}

	_opp_list[opp_type] = oppr;

	/* Terminator implicitly added by kzalloc() */

	/* Free the old list */
	kfree(oppl);

	return 0;
}

/**
 * opp_init_list() - Initialize an opp list from the opp definitions
 * @opp_type:	OPP type to initialize this list for.
 * @opp_defs:	Initial opp definitions to create the list.
 *
 * This function creates a list of opp definitions and returns status.
 * This list can be used to further validation/search/modifications. New
 * opp entries can be added to this list by using opp_add().
 *
 * In the case of error, suitable error code is returned.
 */
int __init opp_init_list(enum opp_t opp_type,
				 const struct omap_opp_def *opp_defs)
{
	struct omap_opp_def *t = (struct omap_opp_def *)opp_defs;
	struct omap_opp *oppl;
	u8 n = 0, i = 1;

	if (unlikely(opp_type >= OPP_TYPES_MAX || !opp_defs ||
			 IS_ERR(opp_defs))) {
		pr_err("%s: Invalid params being passed\n", __func__);
		return -EINVAL;
	}
	/* Grab a count */
	while (t->enabled || t->freq || t->u_volt) {
		n++;
		t++;
	}

	/*
	 * Allocate enough entries to copy the original list, plus the
	 * concluding terminator
	 */
	oppl = kzalloc(sizeof(struct omap_opp) * (n + 1), GFP_KERNEL);
	if (!oppl) {
		pr_err("%s: No memory for opp array\n", __func__);
		return -ENOMEM;
	}


	_opp_list[opp_type] = oppl;
	while (n) {
		omap_opp_populate(oppl, opp_defs);
		oppl->opp_id = i;
		n--;
		oppl++;
		opp_defs++;
		i++;
	}

	/* Terminator implicitly added by kzalloc() */

	return 0;
}

/**
 * opp_enable() - Enable a specific OPP
 * @opp:	Pointer to opp
 *
 * Enables a provided opp. If the operation is valid, this returns 0, else the
 * corresponding error value.
 *
 * OPP used here is from the the opp_is_valid/opp_has_freq or other search
 * functions
 */
int opp_enable(struct omap_opp *opp)
{
	if (unlikely(!opp || IS_ERR(opp))) {
		pr_err("%s: Invalid parameters being passed\n", __func__);
		return -EINVAL;
	}
	opp->enabled = true;
	return 0;
}

/**
 * opp_disable() - Disable a specific OPP
 * @opp:	Pointer to opp
 *
 * Disables a provided opp. If the operation is valid, this returns 0, else the
 * corresponding error value.
 *
 * OPP used here is from the the opp_is_valid/opp_has_freq or other search
 * functions
 */
int opp_disable(struct omap_opp *opp)
{
	if (unlikely(!opp || IS_ERR(opp))) {
		pr_err("%s: Invalid parameters being passed\n", __func__);
		return -EINVAL;
	}
	opp->enabled = false;
	return 0;
}

/**
 * opp_init_cpufreq_table() - create a cpufreq table for a domain
 * @opp_type:	OPP type to initialize this list for
 * @table:	Cpufreq table returned back to caller
 *
 * Generate a cpufreq table for a provided domain - this assumes that the
 * opp list is already initialized and ready for usage
 */
void opp_init_cpufreq_table(enum opp_t opp_type,
			    struct cpufreq_frequency_table **table)
{
	int i = 0, j;
	int opp_num;
	struct cpufreq_frequency_table *freq_table;
	struct omap_opp *opp;

	if (opp_type >= OPP_TYPES_MAX) {
		pr_warning("%s: failed to initialize frequency"
				"table\n", __func__);
		return;
	}

	opp_num = opp_get_opp_count(opp_type);
	if (opp_num < 0) {
		pr_err("%s: no opp table?\n", __func__);
		return;
	}

	freq_table = kmalloc(sizeof(struct cpufreq_frequency_table) *
			     (opp_num + 1), GFP_ATOMIC);
	if (!freq_table) {
		pr_warning("%s: failed to allocate frequency"
				"table\n", __func__);
		return;
	}

	opp = _opp_list[opp_type];
	opp += opp_num;
	for (j = opp_num; j >= 0; j--) {
		if (opp->enabled) {
			freq_table[i].index = i;
			freq_table[i].frequency = opp->rate / 1000;
			i++;
		}
		opp--;
	}

	freq_table[i].index = i;
	freq_table[i].frequency = CPUFREQ_TABLE_END;

	*table = &freq_table[0];
}
