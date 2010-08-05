//#include <common.h>
#include "hdmi_cfg.h"
#include "hdmi.h"

static Vps_HdmiInitParams initParams;
static void * handle = 0x0;

void configureHdVenc1080i60(int useEmbeddedSync)
{
	volatile unsigned int *vencHd_D_Base	=	NULL;

	vencHd_D_Base = (volatile unsigned int *)0x48106000;

    if (useEmbeddedSync != 0x0)
    {
        *vencHd_D_Base = 0x4002A033;
    }
    else
    {
        *vencHd_D_Base = 0x4003A03A;
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
	*vencHd_D_Base = 0x3F245013;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x587800C0;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x00000230;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x000C39E7;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x587800C1;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x0001586D;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x0021C247;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x0500021C;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x05001232;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x00234234;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x587800C0;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x0001586D;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x0021C247;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x0500021C;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x05001232;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x00000000;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x00000000;

}


#if 0
void configureHdVenc1080P30(int useEmbeddedSync)
{
	volatile unsigned int *vencHd_D_Base	=	NULL;

	vencHd_D_Base = (volatile unsigned int *)0x48106000;

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

#if 0
void configureHdVenc720P60(int useEmbeddedSync)
{
	volatile unsigned int *vencHd_D_Base	=	NULL;

	vencHd_D_Base = (volatile unsigned int *)0x48106000;

    /**vencHd_D_Base = 0x40023043;  - Prob embedded syncs */
    /**vencHd_D_Base = 0x4003A013;  - Color space conversion done */
    if (useEmbeddedSync != 0x0)
    {
        *vencHd_D_Base = 0x4002A033;
    }
    else
    {
        *vencHd_D_Base = 0x4003A033;
    }

	vencHd_D_Base++;
	*vencHd_D_Base = 0x1FD01E24;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x02DC020C;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x00DA004A;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x020C1E6C;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x02001F88;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x00200000;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x1B6C0C77;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x1C0C0C30;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x1C0C0C30;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x842EE672;    /* 0x28 */
	vencHd_D_Base++;
	*vencHd_D_Base = 0x3F000018;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x50500103;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x000002E8;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x000C39E7;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x50500172;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x0001A64B;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x002D0000;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x05000000;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x00003000;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x00000000;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x5050016A;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x0001A64B;
	vencHd_D_Base++;
	*vencHd_D_Base = 0x002D0000;
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

void configureHdmi(void)
{
    int status;

    handle  =   0x0;
    initParams.wpBaseAddr       =   HDMI_WP_0_REGS;
    initParams.coreBaseAddr     =   HDMI_CORE_0_REGS;
    initParams.phyBaseAddr      =   HDMI_PHY_0_REGS;
    initParams.pllCtrlBaseAddr  =   HDMI_PLLCTRL_0_REGS;
    initParams.interruptNo      =   INTC_EVENTID_HDMI;
    initParams.vencId           =   0;
    initParams.encoderId        =   0;

    if (VpsHal_hdmiInit(&initParams) != 0x0)
    {
        while(1);
    }

    handle = VpsHal_hdmiOpen( 0,
                              0x0,
                              &status,
                              0x0);
    if (handle == 0x0)
    {
        while(1);
    }
}


void startHdmi(void)
{
    int status;

    status =  VpsHal_hdmiStart( handle, 0x0);

    if (handle == 0x0)
    {
        while(1);
    }

	//printk("Started HDMI \n");
}
