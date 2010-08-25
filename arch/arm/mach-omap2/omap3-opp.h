#ifndef __OMAP3_OPP_H_
#define __OMAP3_OPP_H_

#include <plat/omap-pm.h>

/**
 * omap3_pm_init_opp_table - OMAP opp table lookup called after cpu is detected.
 * Initialize the basic opp table here, board files could choose to modify opp
 * table after the basic initialization
 */
extern int omap3_pm_init_opp_table(void);

#endif
