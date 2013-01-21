/*
 * TI81XX powerdomain control
 */

#include <linux/io.h>
#include <linux/errno.h>
#include <linux/delay.h>

#include "powerdomain.h"
#include <plat/prcm.h>
#include "prm-regbits-34xx.h"

static int ti81xx_pwrdm_set_next_pwrst(struct powerdomain *pwrdm, u8 pwrst)
{
	omap2_prm_rmw_mod_reg_bits(OMAP_POWERSTATE_MASK,
				(pwrst << OMAP_POWERSTATE_SHIFT),
				pwrdm->prcm_offs, TI81XX_PM_PWSTCTRL);
	return 0;
}

static int ti81xx_pwrdm_read_next_pwrst(struct powerdomain *pwrdm)
{
	return omap2_prm_read_mod_bits_shift(pwrdm->prcm_offs,
					TI81XX_PM_PWSTCTRL,
					OMAP_POWERSTATE_MASK);
}

static int ti81xx_pwrdm_read_pwrst(struct powerdomain *pwrdm)
{
	/* Hack to fix GFX pwstst and rstctrl reg offsets to be removed
	 * if the register offsets are made same as other pwrdms
	 */
	return omap2_prm_read_mod_bits_shift(pwrdm->prcm_offs,
					(pwrdm->prcm_offs == TI814X_PRM_GFX_MOD) ? TI81XX_RM_RSTCTRL :
					TI81XX_PM_PWSTST,
					OMAP_POWERSTATEST_MASK);
}

static int ti81xx_pwrdm_read_logic_pwrst(struct powerdomain *pwrdm)
{
	/* Hack to fix GFX pwstst and rstctrl reg offsets to be removed */
	return omap2_prm_read_mod_bits_shift(pwrdm->prcm_offs,
					(pwrdm->prcm_offs == TI814X_PRM_GFX_MOD) ? TI81XX_RM_RSTCTRL :
					TI81XX_PM_PWSTST,
					OMAP3430_LOGICSTATEST_MASK);
}

static int ti81xx_pwrdm_wait_transition(struct powerdomain *pwrdm)
{
	u32 c = 0;

	/*
	 * REVISIT: pwrdm_wait_transition() may be better implemented
	 * via a callback and a periodic timer check -- how long do we expect
	 * powerdomain transitions to take?
	 */

	/* XXX Is this udelay() value meaningful? */
	/* Hack to fix GFX pwstst and rstctrl reg offsets to be removed */
	while ((omap2_prm_read_mod_reg(pwrdm->prcm_offs,
						(pwrdm->prcm_offs == TI814X_PRM_GFX_MOD) ? TI81XX_RM_RSTCTRL :
						TI81XX_PM_PWSTST) &
			OMAP_INTRANSITION_MASK) &&
		(c++ < PWRDM_TRANSITION_BAILOUT))
		udelay(1);

	if (c > PWRDM_TRANSITION_BAILOUT) {
		printk(KERN_ERR "powerdomain: waited too long for "
			"powerdomain %s to complete transition\n", pwrdm->name);
		return -EAGAIN;
	}

	pr_debug("powerdomain: completed transition in %d loops\n", c);

	return 0;
}

struct pwrdm_ops ti81xx_pwrdm_operations = {
	.pwrdm_set_next_pwrst	= ti81xx_pwrdm_set_next_pwrst,
	.pwrdm_read_next_pwrst	= ti81xx_pwrdm_read_next_pwrst,
	.pwrdm_read_pwrst	= ti81xx_pwrdm_read_pwrst,
	.pwrdm_read_logic_pwrst	= ti81xx_pwrdm_read_logic_pwrst,
	.pwrdm_wait_transition	= ti81xx_pwrdm_wait_transition,
};
