/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2010 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/


/**
 *  \file vpshal_hdmi.c
 *
 *  \brief Provides functions to configure HDMI peripheral.
 *
 */

/**
 * Key notes
 * 1. Wrapper doesn't generate interrupts for all the events, generates for HPD.
 *    Using core interrupt instead.
 * 2. DVI mode is not configurable, operates in HDMI mode only, control in
 *    HDMI_CTRL
 * 3. The Core system should operate as a SLAVE. MASTER/SLAVE mode depends on
 *    core/wrapper integration.
 *
 *	IMPORTANT - Input clock is 1/2 of what is expected. I am mutiplying by 2 in PHY.
 *		TMDS_CTRL3.CLKMULT_CTL = b10
 */
/**
 * Know Issues
 * 1. BIOS - Hwi_create () -> Hwi_delete () -> Hwi_create () - assert hit.
 *    Multi- open dosent work
 * 2. PLL config dosent cosider pixel repeatation.
 */
/*
 * Clarifications required
 * 1. DE_CTRL register description missing
 * 2. In core to specify the bus width there are 2 register, please clarify
 *    VID_ACEN.WIDE_BUS and SYS_CTRL1.BSEL
 *    Right now setting SYS_CTRL1 to 1 and WIDE_BUS to 8/10 - programe both TBD. Send mail to Anthony
 *		confirmed on 01062010 from Anthony
 * 3. Input signal interface is not complete - pending in data line if btw
 *    hdvenc - wrapper - core  [Done] - 8/10 bit / channel - apps selectable
 * 4. VID_MODE.DEMUX - Demux 1 to 2 channel - [Done] - Muti-video support - with
 *    embedded syns - not supported.
 * 5. Register description of coefficients for RGB2YCbCr
 * 5. Spec clarification on video size specification - deleted in the spec,
 *    coded for specifying the video size [Done] Not required - deleted this.
 * 6. When core is powered up using sys_ctrl1 bit PD, the interrupt from core
 *    to wrapper is reset.
 */

/*
 * Open items
 * 1. Handle DDC bus hangups / lockups during EDID Read
 * 2. DE Generation - either generate it, extract from BT656 signals or sink
 *    HD Venc directly [Done] Coded for all 3 options
 * 3. Custom co-efficients for RGB to YCbCr CSC
 * 4. Error handling in ISR, right now audio is not supported, most of the error
 *    reported is for audio. No error handling as of now. [Done]
 * 6. PLL - degradation error - would require to handle this. YES TBD
 * 7. defaults for all modes defined in dcModeInfo [Done]
 * 8. On configuration of PHY, we should wait for TCLK to settle down. How do we
 *    do that, wait for clock stable in Core? No PLL Lock of PLL Controller TBD
 * 9. Start Stop should be called in IOCTL - right now called from HDVENC
 *    Start and Stop - Not required
 * 10. When updating setMode you do not require mutil-task protection. Taken
 *     care by DC.
 */


/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "hdmi_cfg.h"
#include "hdmi.h"
#include "hdmi_defaults.h"



#define USE_SILICON_RECOM_2306


/* ========================================================================== */
/*                              Local Configurations                          */
/* ========================================================================== */
#define VPS_HAL_HDMI_DDC_CMD_TIMEOUT            (0xFFFFFu)
/**< Timeout periods used to wait for a DDC operation to complete */
#define VPS_HAL_HDMI_WP_RESET_TIMEOUT           (0xFFFFFu)
/**< Timeout periods used to wait for a DDC opeation to complete */
#define VPS_HAL_HDMI_PHY_2_WP_PLL_LOCK_TIMEOUT  (0xFFFFFu)
/**< Timeout periods used to wait TCLK to stabilize - TCLK would be generated
     by PHY to operate wrapper */

/* ========================================================================== */
/*                              Local Defines                                 */
/* ========================================================================== */


#define hdmiTrace       (0)

#define CB_NONE         (0x0)
/**< Classify the callback that would required to call-no callback called */
#define CB_HPD          (0x01u)
/**< Classify the callback that would required to call-sink detect/removed CB */
#define CB_ERROR        (0x02u)
/**< Classify the callback that would required to call-no callback called */
#define NONE_GUARD      (0x0)
/**< Used to identify resource that would be released - none in this case */
#define BOTH_GUARDS     (0x1u)
/**< Used to identify resource that would be released - Configuration guard and
     instance guard */
#define INST_GUARD      (0x2u)
/**< Used to identify resource that would be released - Instance guard */
#define HDMI_CTRL_PACKET_MODE_24BITS_PIXEL  (0x4u)
/**< Defines used to configure the number of bits/pixel that would sent to
     packetizer */
#define HDMI_CTRL_PACKET_MODE_30BITS_PIXEL  (0x5u)
/**< Defines used to configure the number of bits/pixel that would sent to
     packetizer */
#define HDMI_CTRL_PACKET_MODE_36BITS_PIXEL  (0x6u)
/**< Defines used to configure the number of bits/pixel that would sent to
     packetizer */
#define HDMI_VID_MODE_DITHER_TO_24_BITS_MODE (0x0u)
/**< Defines to used to determine the dithering width */
#define HDMI_VID_MODE_DITHER_TO_30_BITS_MODE (0x1u)
/**< Defines to used to determine the dithering width */
#define HDMI_VID_MODE_DITHER_TO_36_BITS_MODE (0x2u)
/**< Defines to used to determine the dithering width */
#define HDMI_TMDS_CTRL_IP_CLOCK_MULTIPLIER_AUDIO    (0x1u)
/**< Defines the multiplier value used to multiply the input clock IDCK, in order
     to support higher sampling rates / channels audio */

#define HDMI_AVI_INFOFRAME_PKT_TYPE         (0x82u)
/**< AVI Info frame header - packet type - defined by standard */
#define HDMI_AVI_INFOFRAME_PKT_VER          (0x02)
/**< AVI Info frame header - packet version - defined by standard */
#define HDMI_AVI_INFOFRAME_PKT_LEN          (0x0D)
/**< AVI Info frame header - packet version - defined by standard */
#define HDMI_AVI_INFOFRAME_Y0_Y1_MASK       (0x60u)
/**< Mask to set/extract Y0-1 bit of first byte of AVI info packet */
#define HDMI_AVI_INFOFRAME_A0_MASK          (0x10u)
/**< Mask to set/extract A0 bit of first byte of AVI info packet */
#define HDMI_AVI_INFOFRAME_B0_B1_MASK       (0x0Cu)
/**< Mask to set/extract B0-1 bit of first byte of AVI info packet */
#define HDMI_AVI_INFOFRAME_S0_S1_MASK       (0x03u)
/**< Mask to set/extract S0-1 bit of first byte of AVI info packet */
#define HDMI_AVI_INFOFRAME_C0_C1_MASK       (0xC0u)
/**< Mask to set/extract C0-1 bit of first byte of AVI info packet */
#define HDMI_AVI_INFOFRAME_M0_M1_MASK       (0x30u)
/**< Mask to set/extract M0-1 bit of first byte of AVI info packet */
#define HDMI_AVI_INFOFRAME_R0_R3_MASK       (0x0Fu)
/**< Mask to set/extract R0-3 bit of first byte of AVI info packet */
#define HDMI_AVI_INFOFRAME_ITC_MASK         (0x80u)
/**< Mask to set/extract ITC bit of first byte of AVI info packet */
#define HDMI_AVI_INFOFRAME_EC2_EC0_MASK     (0x70u)
/**< Mask to set/extract EC0-3 bit of first byte of AVI info packet */
#define HDMI_AVI_INFOFRAME_Q1_Q0_MASK       (0x0Cu)
/**< Mask to set/extract Q0-1 bit of first byte of AVI info packet */
#define HDMI_AVI_INFOFRAME_SC1_SC0_MASK     (0x03u)
/**< Mask to set/extract SC0-1 bit of first byte of AVI info packet */
#define HDMI_AVI_INFOFRAME_VIC6_VIC0_MASK   (0x7Fu)
/**< Mask to set/extract VIC6-0 bit of first byte of AVI info packet */
#define HDMI_AVI_INFOFRAME_PR3_PR0_MASK     (0x0Fu)
/**< Mask to set/extract PR3-0 bit of first byte of AVI info packet */
#define HDMI_AVI_INFOFRAME_CONST_0x100      (0x100u)
/**< Constant used to calculate AVI info frame checksum */
#define HDMI_SHIFT_BY_1                     (0x01u)
/**< Shift bits */
#define HDMI_SHIFT_BY_2                     (0x02u)
/**< Shift bits */
#define HDMI_SHIFT_BY_3                     (0x03u)
/**< Shift bits */
#define HDMI_SHIFT_BY_4                     (0x04u)
/**< Shift bits */
#define HDMI_SHIFT_BY_5                     (0x05u)
/**< Shift bits */
#define HDMI_SHIFT_BY_6                     (0x06u)
/**< Shift bits */
#define HDMI_SHIFT_BY_8                     (0x08u)
/**< Shift bits */

#define HDMI_MINIMUM_PIXELS_SEC             (25000000u)
/**< HDMI standard Mandates that at a minimum there should be 25 MPixels/sec. */
#define HDMI_PIXEL_REPLECATED_ONCE          (0x2)
/**< Each pixel would be sent twice */
#define HDMI_PIXEL_REPLECATED_FOUR_TIMES    (0x4)
/**< Each pixel would be sent four times */

/* Standard resolutions column X row X FPS */
#define HDMI_VIDEO_STAND_NTSC               (858 * 525 * 30)
#define HDMI_VIDEO_STAND_PAL                (858 * 625 * 25)
#define HDMI_VIDEO_STAND_720P60             (1650 * 750 * 60)
#define HDMI_VIDEO_STAND_1080P60            (2200 * 1125 * 60)
#define HDMI_VIDEO_STAND_1080I60            (2200 * 1125 * 30)
#define HDMI_VIDEO_STAND_1080P30            (2200 * 1125 * 30)

/* ========================================================================== */
/*                              Local Structure                               */
/* ========================================================================== */

/**
 *  enum HdmiObjectState
 *  \brief Enumeration that defines various states of HDMI HAL object.
 */
typedef enum
{
    HDMI_HAL_INST_UN_INITIALIZED    =   0x0,
    /**< Object instance un initialized */
    HDMI_HAL_INST_INITIALIZED,
    /**< Object instance initialized */
    HDMI_HAL_INST_OPENED,
    /**< Object instance opened */
    HDMI_HAL_INST_CLOSED
    /**< Object instance closed */
} HdmiObjectState;


/**
 *  enum HdmiObject_t
 *  \brief Defines an HAL HDMI object, self contained instance depicting the
 *         state, operations enabled, configurations among others.
 */
typedef struct
{
    u32                      instance;
    /**< Instance of HDMI Transmitter that this object defines */
    HdmiObjectState             state;
    /**< Defines the current state of the instance */
    CSL_Hdmi_CoreRegs           *coreRegOvrLay;
    /**< Pointer to register structure of the peripheral core */
    CSL_Hdmi_WpRegs             *wpRegOvrLay;
    /**< Pointer to register strucutre of the peripheral wrappers */
    CSL_Hdmi_PhyRegs            *phyRegOvrLay;
    /**< Pointer to register structure that describe HDMI PHY */
    CSL_Hdmi_PllCtrlRegs        *pllRegOvrLay;
    /**< Pointer to register structure that describe PLL Controller */
    u32                      interruptNo;
    /**< Associated interrupt number */
/*    Semaphore_Handle            configGuard; */
    /**< The audio could be configured by a different HAL, guard to ensure
         exclusive access required while configuring video aspects */
/*    Semaphore_Handle            guard;*/
    /**< exclusive access - Making it multi-task safe */
    Bool                        isReceiverSensed;
    /**< Flag to indicate that this instance of HDMI has receiver connected to
         it */
    Bool                        isSclClocked;
    /**< DDC Command, SCL Clock resets any of the devices on the DDC bus, should
         clocked once. On receiverSense, before the next DDC command, the HAL
         would clock once and set this flag */
    Bool                        isStreaming;
    /**< Indicates the current status of the video stream. FALSE indicates that
         no active streams and could be re/configured. Streaming otherwise,
         re/configuration could not be applied */
    Vps_HdmiConfigParams        config;
    /**< HDMI configuration - holds the current configuration */
#if INCLUDE_BIOS_INT_HANDLER
    Hwi_Handle                  hwiHandle;
#endif
    /**< DSP/BIOS Interrupt handlers handle */
    Bool                        isVSyncCountTracked;
    /**< Flag to enable count the number of vSync, used by HDCP to decided when
         audio requires to be enabled */
    u32                      vSyncCounter;
    /**< If isVSyncCountTracked is TRUE, place holder to track the number of
         vSync seen - i.e. coming into wrapper /sent - to core i,e.
         When generating the sync signal we would count the outgoing vSync
         else the in-coming */
    Bool                        isInterlaced;
    /**< TRUE implies that currently configured for interlaced display  */
    u32                      pairedVencId;
    /**< DC assigned identifier of the VENC, that is paired with this instance
         of HDMI */
    u32                      encInstEd;
    /**< DC assigned identifier for this instance of encoder, i.e. HDMI instaces
         identifier as assigned by DC */
}HdmiObject_t;

/* ========================================================================== */
/*                         Local Function Declarations                        */
/* ========================================================================== */
static void     hdmiMainIsr   (UArg arg);

static int    configurePll            (HdmiObject_t   *instHandle);
static int    configurePhy            (HdmiObject_t   *instHandle);
static int    configureWrapper        (HdmiObject_t   *instHandle);
static int    configureCoreInput      (HdmiObject_t   *instHandle);
static int    configureCoreDataPath   (HdmiObject_t   *instHandle);
static int    configureCore           (HdmiObject_t   *instHandle);
static int    configureHdmiPolicies   (HdmiObject_t   *instHandle);
static int    configureAVIInfoFrame   (HdmiObject_t   *instHandle,
                                         Bool           transmitOnce);
static int    configureControlPacket  (HdmiObject_t   *instHandle);
static int    configureCscYCbCr2RGB   (HdmiObject_t   *instHandle);

static int    validateInfoFrameCfg    (Vps_HdmiInfoFrameCfg       *config);
static int    validateHdmiCoreIpConfig(Vps_HdmiCoreInputConfig    *config);
static int    validateHdmiWpConfig    (Vps_HdmiWpConfig           *config);
static int    validateHdmiPathConfig  (Vps_HdmiCoreDataPathConfig *config);
static int    checkAndCopyCfg         (HdmiObject_t               *instPtr,
                                         Vps_HdmiConfigParams       *config);

static int    waitForTCLKtoStabalize   (HdmiObject_t      *instHandle);
static int    determinePixelRepeatation(HdmiObject_t      *instHandle);


static int    VpsHal_hdmiEdidRead     (VpsHal_Handle           handle,
                                         Vps_HdmiEdidReadParams  *rParams,
                                         HDMI_Ptr                     args);
#ifdef EDID_USE_LEGACY_READ
static int    VpsHal_hdmiEdidRead_legacy (VpsHal_Handle           handle,
                                            Vps_HdmiEdidReadParams  *rParams,
                                            HDMI_Ptr                     args);
#endif /* EDID_USE_LEGACY_READ */
 static int   VpsHal_hdmiAttachHpdCb  (VpsHal_Handle handle,
                                         Vps_Hdmi_CbFxn cbFxn,
                                         HDMI_Ptr cbArgs,
                                         HDMI_Ptr args);
static int    VpsHal_hdmiGetConfig(VpsHal_Handle handle,
                                     Vps_HdmiConfigParams *config,
                                     HDMI_Ptr     args);
 static int   VpsHal_hdmiSetConfig(VpsHal_Handle handle,
                                     Vps_HdmiConfigParams *config,
                                     HDMI_Ptr     args);

void HDMI_GT_0trace ( u32    maskType,
                 u32    classtype,
                 char * infoString);
void HDMI_GT_1trace ( u32    maskType,
                 u32    classtype,
                 char * infoString,
                 u32    val1);
void HDMI_GT_assert(u32 a, u32 b);

#if ((hdmiTrace & (HDMI_GT_DEBUG | GT_TraceState_Enable)) ==   \
                  (HDMI_GT_DEBUG | GT_TraceState_Enable))
static void initializeDebugInfo (HdmiObject_t *instPtr);

#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
static HdmiObject_t     HdmiObjectsPool[HDMI_PER_CNT];
/**< Pool of HDMI objects */
static HdmiObjectState  HdmiHalInitState    = HDMI_HAL_INST_UN_INITIALIZED;
/**< Flag to track HAL status */
static Vps_HdmiConfigParams defaultConfig   = HDMI_HAL_INIT_8BIT_1080P_60_16_9_HD;
/**< Default configuration to start with */
static const Vps_HdmiPllCtrlParams  hdmiPllCtrlCfg[VPS_HDMI_MAX_MODE] =   {    \
                                    VPS_HDMI_PLLCTRL_NOT_SUPPORTED,            \
                                    VPS_HDMI_PLLCTRL_NOT_SUPPORTED,            \
                                    VPS_HDMI_PLLCTRL_10BIT_IF_1080P_60,        \
                                    VPS_HDMI_PLLCTRL_10BIT_IF_720P_60,         \
                                    VPS_HDMI_PLLCTRL_10BIT_IF_1080I_60,        \
                                    VPS_HDMI_PLLCTRL_10BIT_IF_1080P_30};
/**< Configuration for all supported modes */
/* ========================================================================== */
/*                                  Debug Info                                */
/* ========================================================================== */
#if (0)
CSL_WP_Debug_HdmiRegs       debugWpRegs;
/**< Points to wrapper register */
CSL_CORE_Debug_HdmiRegs     debugCoreRegs;
/**< Points to core register */
CSL_CORE_Debug_DdcRegs      debugDdcRegs;
/**< Points to cores ddc register */
CAL_CORE_Debug_AVIInfoRegs  debugAviInfoRegs;
/**< Points to cores AVI Info frame register */
#endif

/* ========================================================================== */
/*                            Local Functions                                 */
/* ========================================================================== */

/**
 *  hdmiMainIsr
 *  \brief  Interrupt servicing function, handle's hot plug detect of the
 *          wrapper and/or core and vSync of the core.
 *          At this point, no errors are handled, only possible error would be
 *          of re-calibration of PLL. When audio is enabled, audio FIFOs over/
 *          underflow errors could be handled here.
 *          When HDCP is supported, this is right place to handle HDCP errors.
 */
static void hdmiMainIsr(UArg arg)
{
    HdmiObject_t    *instHandle  =   (HdmiObject_t *) arg;
    CSL_Hdmi_WpRegs *wpRegs     =   NULL;
    u32          callCb      =   CB_NONE;
    volatile u32 irqStatus;

    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>hdmiMainIsr");
    while(TRUE)
    {
        HDMI_GT_assert(hdmiTrace, (instHandle != NULL));
        HDMI_GT_assert(hdmiTrace, (instHandle->wpRegOvrLay != NULL));
        /*
         * Step 1. Take a copy of the interrupt status registers and clear em
         * Step 2. HOT Plug detect - mark the internal flag and let apps know
         * Step X. Would require to handle error-define what to do on errors TBD
         * Step 3. On wrapper error - if requested let the apps know TBD
         * Step 4. On Core interrupt - call the core interrupt handler
         * Step 5. If enabled to track vSync, count them
         */
        /* Step 1 - Take a copy of status and clear pending interrupts */
        wpRegs      =   instHandle->wpRegOvrLay;
        irqStatus   =   wpRegs->IRQSTATUS;
        wpRegs->IRQSTATUS   =   irqStatus;

        /* Step 2 */
        if ((irqStatus & CSL_HDMI_WP_IRQSTATUS_PHY_LINK_DISCONNECT_PEND_MASK) ==
            CSL_HDMI_WP_IRQSTATUS_PHY_LINK_DISCONNECT_PEND_MASK)
        {
            instHandle->isReceiverSensed =   FALSE;
            callCb                       =   CB_HPD;
        }
        if ((irqStatus & CSL_HDMI_WP_IRQSTATUS_PHY_LINK_CONNECT_PEND_MASK) ==
            CSL_HDMI_WP_IRQSTATUS_PHY_LINK_CONNECT_PEND_MASK)
        {
            instHandle->isReceiverSensed =   TRUE;
            callCb                       =   CB_HPD;
        }

        /* Step 4 */
        if ((irqStatus & CSL_HDMI_WP_IRQSTATUS_CORE_INTR_PEND_MASK) ==
            CSL_HDMI_WP_IRQSTATUS_CORE_INTR_PEND_MASK)
        {
            /* Right now, we are handling HPD and RSEN interrupts from the core
               when required to handle more interrupts update this section */
            if ((instHandle->coreRegOvrLay->INTR1 & CSL_HDMI_INTR1_HPD_MASK) ==
                    CSL_HDMI_INTR1_HPD_MASK)
            {
                if (instHandle->isReceiverSensed != TRUE)
                {
                    /* Why was this not sensed by wrappers interrupt ? */
                    instHandle->isReceiverSensed = TRUE;
                    callCb                       = CB_HPD;
                    HDMI_GT_0trace(hdmiTrace, HDMI_GT_DEBUG, "Dected a sink");
                }
            }
            else
            {
                /* Check for un-plug */
                if (instHandle->isReceiverSensed == TRUE)
                {
                    /* Why was this not sensed by wrappers interrupt ? */
                    instHandle->isReceiverSensed = FALSE;
                    callCb                       = CB_HPD;
                    /* Ensure to disable the wrapper */
                    instHandle->wpRegOvrLay->VIDEO_CFG &=
                                        (~(CSL_HDMI_WP_VIDEO_CFG_ENABLE_MASK));
                    HDMI_GT_0trace(hdmiTrace, HDMI_GT_DEBUG, "Attached sink is removed");
                }
            }

            /* Track the number of VSYNC seen */
            if ((instHandle->coreRegOvrLay->INTR1 &
                                                CSL_HDMI_INTR2_VSYNC_REC_MASK)
                    == CSL_HDMI_INTR2_VSYNC_REC_MASK)
            {
                if (instHandle->isVSyncCountTracked == TRUE)
                {
                    instHandle->vSyncCounter++;
                    /* Wrap around? doesn't matter, the user-HDCP enable will
                       handle it, as it would require to get the count first -
                       start the stream - periodically get the count and enable
                       audio / encryption when required */
                }
            }
            /* Clear the interrupt */
            instHandle->coreRegOvrLay->INTR4 |=
                                        (CSL_HDMI_INTR4_REG_INTR4_STAT1_MASK |
                                         CSL_HDMI_INTR4_REG_INTR4_STAT2_MASK);
        }
        break;
    }

    /* Step 2 and 3 */
    if (callCb != CB_NONE)
    {
        if ((callCb == CB_HPD) && (instHandle->config.cbOnHPD != NULL))
        {
            instHandle->config.cbOnHPD( instHandle->isReceiverSensed,
                                        instHandle->config.cbArgs);
        }
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "hdmiMainIsr<<<<");
}


/**
 *  configurePll
 *  \brief  This function configures the PLL. The provides clocks required by
 *          PHY and the IP Core. Essentially, PCLK * 1.25 if in 10 bit mode
 *
 *  \par CAUTION : This function is expected to be called when initializing or
 *       when re-configuring. After re-configuration its recomended to reset the
 *       core and wrapper. To stabilize the clocks, it recomended to wait for a
 *       period of time.
 */
static int configurePll(HdmiObject_t *instHandle)
{
#if 0
	instHandle->wpRegOvrLay->PWR_CTRL = 0x95;

    int                   rtnValue    =   VPS_SOK;
    CSL_Hdmi_PllCtrlRegs    *pllRegs    =   NULL;
    Vps_HdmiPllCtrlParams   *pllCfg     =   NULL;
    volatile u32         temp;

    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>configurePll");
    HDMI_GT_assert(hdmiTrace, (instHandle != NULL));
    HDMI_GT_assert(hdmiTrace, (instHandle->pllRegOvrLay != NULL));

    /* Steps
     * 1. For the given mode, get the defaults
     * 2. Configure the controller input clock configurations
     * 3. Configure mutipliers and dividers
     * 4. Configure sigma delta divider for ADPLLLJ
     * 5. Re-Lock with new configurations
     *
     * Note that until GO bit is set, non of the changes has any effect
     *
     * Policies - Allow user to control this TBD
     * 1. Reference source - should be selectable - hard coded to 1 REF1
     * 2. Clock Drift - Checks not enabled - enable it and re-caliber if
     *      required.
     * 3. Use of sysClk - hard coded not use SYSCLK as input - should be
     *    user configureable
     */
    /* Step 1 */
    if (instHandle->config.displayMode >= VPS_HDMI_MAX_MODE)
    {
        rtnValue = VPS_EUNSUPPORTED_CMD;
        HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "The display format is not supported");
    }
    else
    {
        pllCfg  =   (Vps_HdmiPllCtrlParams *)
                    &(hdmiPllCtrlCfg[instHandle->config.displayMode]);
    }
    /* NTSC and PAL is not supported for now */
    if (pllCfg->nDivider == 0x0)
    {
        rtnValue = VPS_EUNSUPPORTED_CMD;
        HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "The display format is not supported");
    }
    while((pllCfg != NULL) && (rtnValue == VPS_SOK))
    {
        pllRegs = instHandle->pllRegOvrLay;
        /* Step 2 controllers input clock configurations */
        temp = pllRegs->CONFIGURATION2;
        /* Clear out the bits that we are going to update */
        temp &= (~( CSL_HDMI_PLLCTRL_CONFIGURATION2_PHY_CLKINEN_MASK        |
                    CSL_HDMI_PLLCTRL_CONFIGURATION2_PLL_REFEN_MASK          |
                    CSL_HDMI_PLLCTRL_CONFIGURATION2_PLL_HIGHFREQ_MASK       |
                    CSL_HDMI_PLLCTRL_CONFIGURATION2_PLL_CLKSEL_MASK         |
                    CSL_HDMI_PLLCTRL_CONFIGURATION2_PLL_DRIFTGUARDEN_MASK   |
                    CSL_HDMI_PLLCTRL_CONFIGURATION2_PLL_SELFFREQDCO_MASK));

        /* Clock input PHY - enable it */
        temp |= CSL_HDMI_PLLCTRL_CONFIGURATION2_PHY_CLKINEN_MASK;
        /* Reference clock input to PLL - enable it */
        temp |= CSL_HDMI_PLLCTRL_CONFIGURATION2_PLL_REFEN_MASK;
        /*
         * High clock is reset - as system clock is under the tolerable range of
         *      2 to 60 MHz
         * Clock select it reset - sys clock for PLL is sourced from the
         *      system clock - input clock to silicon.
         * Drift Guard is disabled - Enable it TBD
         */
        temp |=     (CSL_HDMI_PLLCTRL_CONFIGURATION2_PLL_SELFFREQDCO_MASK &
                    (pllCfg->selFreqDco <<
                    CSL_HDMI_PLLCTRL_CONFIGURATION2_PLL_SELFFREQDCO_SHIFT));

        pllRegs->CONFIGURATION2 = temp;

        /* Step 3. Configure M and N dividers */
        temp = pllRegs->CONFIGURATION1;
        temp &= (~(CSL_HDMI_PLLCTRL_CONFIGURATION1_PLL_REGM_MASK    |
                   CSL_HDMI_PLLCTRL_CONFIGURATION1_PLL_REGN_MASK));

        temp |=     (CSL_HDMI_PLLCTRL_CONFIGURATION1_PLL_REGM_MASK &
                    (pllCfg->mDivider <<
                    CSL_HDMI_PLLCTRL_CONFIGURATION1_PLL_REGM_SHIFT));

        temp |=     (CSL_HDMI_PLLCTRL_CONFIGURATION1_PLL_REGN_MASK &
                    (pllCfg->nDivider <<
                    CSL_HDMI_PLLCTRL_CONFIGURATION1_PLL_REGN_SHIFT));

        pllRegs->CONFIGURATION1 = temp;


        temp = pllRegs->CONFIGURATION4;

        temp &= (~(CSL_HDMI_PLLCTRL_CONFIGURATION4_PLL_REGM2_MASK   |
                   CSL_HDMI_PLLCTRL_CONFIGURATION4_PLL_REGM_F_MASK));

        temp |=     (CSL_HDMI_PLLCTRL_CONFIGURATION4_PLL_REGM2_MASK &
                    (pllCfg->m2Divider <<
                    CSL_HDMI_PLLCTRL_CONFIGURATION4_PLL_REGM2_SHIFT));

        temp |=     (CSL_HDMI_PLLCTRL_CONFIGURATION4_PLL_REGM_F_MASK &
                    (pllCfg->mDividerFraction <<
                    CSL_HDMI_PLLCTRL_CONFIGURATION4_PLL_REGM_F_SHIFT));

        pllRegs->CONFIGURATION4 = temp;

        /* Step 4 - Configure sigma delta for ADPLLLJ */
        temp = pllRegs->CONFIGURATION3;

        temp &= (~(CSL_HDMI_PLLCTRL_CONFIGURATION3_PLL_SD_MASK));

        temp |=     (CSL_HDMI_PLLCTRL_CONFIGURATION3_PLL_SD_MASK &
                    (pllCfg->sigmaDeltaValue <<
                    CSL_HDMI_PLLCTRL_CONFIGURATION3_PLL_SD_SHIFT));

        pllRegs->CONFIGURATION3 = temp;

        /* Re-lock with new configurations */
        pllRegs->GO |= CSL_HDMI_PLLCTRL_PLLGO_GO_MASK;
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "configurePll<<<<");
    return (rtnValue);
#endif
    return 0x0;
}


/**
 *  configurePhy
 *  \brief  This function configures the PHY. Programs the TCLK to core
 *          Enables RXSens to core, Enables source termination of the TMDS lines
 *          and deep color modes.
 *
 *  \par CAUTION : This function is expected to be called when initializing or
 *       when re-configuring. After re-configuration its recomended to reset the
 *       core and wrapper. To stabilize the clocks, it recomended to wait for a
 *       period of time.
 */
static int configurePhy(HdmiObject_t *instHandle)
{
    int               rtnValue    =   VPS_SOK;
    CSL_Hdmi_PhyRegs    *phyRegs    =   NULL;
    volatile u32     temp;

    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>configurePhy");
    HDMI_GT_assert(hdmiTrace, (instHandle != NULL));
    HDMI_GT_assert(hdmiTrace, (instHandle->phyRegOvrLay != NULL));
    while(TRUE)
    {
        phyRegs = instHandle->phyRegOvrLay;

        /* Steps
         * 0. Power up if powered down
         * 1. Determine the TCLK based in Deep color mode (Dither mode is used
         *    to depth of the color) and Pixel repeatation (Use the local
         *    function to determine). Turn OFF BIST pattern generator
         * 2. Turn OFF BIST and DVI Encoder
         * 3. Configure the source termination determination - we would require
         *    when the sink terminates the source - recomended by HDMI Spec 1.3A
         *    when operating higer frequencies
         * 4. Enable the PHY
         * 5. Wait for the clocks to stabilize - how? TBD
         */
        /* Step 0 - Power up - if not powered up already */
        if ((phyRegs->TMDS_CNTL3 & CSL_HDMI_PHY_TMDS_CNTL3_PDB_MASK) !=
                                   CSL_HDMI_PHY_TMDS_CNTL3_PDB_MASK)
        {
            phyRegs->TMDS_CNTL3 |= CSL_HDMI_PHY_TMDS_CNTL3_PDB_MASK;
        }

        /* Clear out the bits that were are going to update.
         * BIST Pattern generator is disabled - leave it at that
         */
        temp = phyRegs->TMDS_CNTL3;
        temp &= (~((CSL_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_MASK) |
                   (CSL_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_MASK) |
                   (CSL_HDMI_PHY_TMDS_CNTL3_BIST_SEL_MASK)));

        /* Step 1.1 - Output width of the dither module in core, determines
         *            deep color or not
         */

        if (instHandle->config.corePathConfig.outputWidth ==
                                                VPS_HAL_HDMI_10_BITS_CHAN_WIDTH)
        {
            temp |= (CSL_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_10BITCHANNEL <<
                     CSL_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_SHIFT);
            /* TBD - We would require to set bit 7 of TMDS Control 9 when in
                     deep color mode - cannot justify at this point */
            phyRegs->TMDS_CNTL3 |= CSL_HDMI_PHY_TMDS_CNTL9_TEN_BIT_BYPASS;
        }
        else if (instHandle->config.corePathConfig.outputWidth ==
                                                VPS_HAL_HDMI_8_BITS_CHAN_WIDTH)
        {
            temp |= (CSL_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_NO <<
                     CSL_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_SHIFT);
            /* TBD - We would require to set bit 7 of TMDS Control 9 when in
                     deep color mode - cannot justify at this point */
            phyRegs->TMDS_CNTL3 |= CSL_HDMI_PHY_TMDS_CNTL9_TEN_BIT_BYPASS;
        }
        else
        {
            temp |= (CSL_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_12BITCHANNEL <<
                     CSL_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_SHIFT);
            /* TBD - We would require to set bit 7 of TMDS Control 9 when in
                     deep color mode - cannot justify at this point */
            phyRegs->TMDS_CNTL3 |= CSL_HDMI_PHY_TMDS_CNTL9_TEN_BIT_BYPASS;
        }
        /* Step 1.2 - Pixel repeataion */
        rtnValue = determinePixelRepeatation(instHandle);
        if (rtnValue == HDMI_PIXEL_REPLECATED_ONCE)
        {
            /* Repeat once */
            temp |= (CSL_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_2_0X <<
                     CSL_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_SHIFT);
			//printk("\n PHY Prog - Pixel repeat 2\n");
        }
        else if (rtnValue == HDMI_PIXEL_REPLECATED_FOUR_TIMES)
        {
            /* Repeated four times */
            temp |= (CSL_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_4_0X <<
                     CSL_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_SHIFT);
			//printk("\n PHY Prog - Pixel repeat 4\n");
        }
        else if (rtnValue == VPS_SOK)
        {
            /* No repeatation */
            temp |= (CSL_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_1_0X  <<
                     CSL_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_SHIFT);
			//printk("\n PHY Prog - Pixel repeat 1\n");
        }
        else
        {
            /* Error let the caller know */
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not determine pixel ");
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "that would be required.");
            break;
        }
		//printk("\n TMDS_CNT3 = %x\n", temp);
        phyRegs->TMDS_CNTL3 = temp;

        /* Step 2 - Turn OFF BIST and DVI Encoder */
        phyRegs->BIST_CNTL &= ~CSL_HDMI_PHY_BIST_CNTL_BIST_EN_MASK;

        phyRegs->BIST_CNTL |= CSL_HDMI_PHY_BIST_CNTL_ENC_BYP_MASK;

        phyRegs->TMDS_CNTL9 |= 0xE0;
#if 0
#ifdef USE_SILICON_RECOM_2306
/************************Silicon Image Recomndations***************************/
        phyRegs->BIST_CNTL &= (~(CSL_HDMI_PHY_BIST_CNTL_BIST_EN_MASK |
                                CSL_HDMI_PHY_BIST_CNTL_BIST_ON_MASK  |
                                CSL_HDMI_PHY_BIST_CNTL_ENC_BYP_MASK));
/************************Silicon Image Recomndations Ends**********************/
#endif /* USE_SILICON_RECOM_2306 */
#endif

#if 0
/************************Silicon Image DVI Mode********************************/

        phyRegs->BIST_PATTERN   =   0x00;
        /*  Reg 4 - BIST Pattern Register  = 8'h00*/
        phyRegs->BIST_INST0     =   0x81;
        /* Reg 5 - BIST Instruction 0 Register = 8'h81 */
        phyRegs->BIST_INST1     =   0x20;
        /* Reg 6 - BIST Instruction 1 Register = 8'h20 */
        /* Reg 7 - BIST Configuration Register = No Change */
/************************Silicon Image DVI Mode Ends***************************/
#endif

/************************Silicon Image PHY BIST Test***************************/
#if 0
        phyRegs->BIST_CNTL      =   0x40;
        /* Reg_addrees 3, BIST CNTL  = 8’h40  // Disable the bist  */
        phyRegs->TMDS_CNTL3     =   0xE9;
        /* Reg_address 2, TMDS_CTL3 = 8’hE9  // Select the Tx BIST */
        phyRegs->BIST_PATTERN   =   0x0;
        /* Program the instruction, pattern, configuration registers */
        /* Reg_address 4, BIST_PATTERN_REGISTER = 8’h00  // pattern[7:0] */
        phyRegs->BIST_INST0     =   0x81;
        /* Reg_address 5, BIST Instruction 0 register  = 8’h81 //instruction[7:0] */
        phyRegs->BIST_CONF0     =   0x00;
        /* Reg_address 7, BIST configuration register = 8’h00 // configuration[7:0] */
        phyRegs->BIST_INST1     =   0x20;
        /* Reg_address6, BIST instruction 1 register  = 8’h20 // {instruction[10:8], 1’b0, configuration[9:8], pattern[9:8]} */
        temp = 0xFF;
        while(temp)
        {
            temp--;
        }
        /* Wait for few clocks (say 20 TMDS clocks) */
        phyRegs->BIST_CNTL      =   0x41;
        /* Start the BIST data generation
        Reg_address 3, BIST CNTL = 8’h41 // Enable the bist to run*/

#endif
/************************Silicon Image PHY BIST Test***************************/

        /* Step 3 and 4 */
        phyRegs->TMDS_CNTL2 |= (CSL_HDMI_PHY_TMDS_CNTL2_TERM_EN_MASK |
                                CSL_HDMI_PHY_TMDS_CNTL2_OE_MASK);
        /* phyRegs->TMDS_CNTL2 |= (CSL_HDMI_PHY_TMDS_CNTL2_OE_MASK);*/
        break;
    }

    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "configurePhy<<<<");
    return (rtnValue);
}


/**
 *  configureWrapper
 *  \brief  Configure the wrapper with debouce data packing modes, timming
 *          parameters if operating as a clock/timming generator.
 *
 *  \par CAUTION : Assumes that the wrapper is held in reset state before
 *                 calling this function.
 */
static int configureWrapper(HdmiObject_t *instHandle)
{
    int               rtnValue    =   VPS_SOK;
    CSL_Hdmi_WpRegs     *wpRegs     =   NULL;
    volatile u32     temp;

    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>configureWrapper");
    HDMI_GT_assert(hdmiTrace, (instHandle != NULL));
    HDMI_GT_assert(hdmiTrace, (instHandle->wpRegOvrLay != NULL));
    while(TRUE)
    {
        wpRegs  =   instHandle->wpRegOvrLay;

        printk("\n Wrapper revision ID is %x", wpRegs->REVISION);
        /* Steps
         * 0. Debounce remain at reset value - tweak if required
         * 0.0 CEC Divider requires to be configured - important
         * 1. Enable the interrupts that would be handled
         * 2. In Master mode - Configure video timming parameters
         * 3. In Master mode - Configure video size
         * 4. In Master mode - polarity of the syncs
         * 5. Data Packing modes
         * 6. As either MASTER or SLAVE
         */
        /* Step 0 - Tweak if required */
        temp =  ((instHandle->config.wpConfig.debounceRecvDetect <<
                      CSL_HDMI_WP_DEBOUNCE_RXDET_SHIFT) &
                    CSL_HDMI_WP_DEBOUNCE_RXDET_MASK);

        temp |= ((instHandle->config.wpConfig.debounceRecvSens <<
                      CSL_HDMI_WP_DEBOUNCE_LINE5VSHORT_SHIFT) &
                    CSL_HDMI_WP_DEBOUNCE_LINE5VSHORT_MASK);

        wpRegs->DEBOUNCE  = temp;

        /* require to format this... TBD Dividing the 48MHz clock to 2 MHz for
		   CEC and OCP different dividor */
		temp = wpRegs->CLK;
		temp |= 0x00000218u;
        wpRegs->CLK = temp;


        /* Step 1
         * Interrupts of interest
         * A. PLL Re-Caliber - clock degradation on the link TBD
         * B. Link disconnected
         * C. Link connected
         * D. VSYNC done - not now - on HDCP, would require to count 5 frames
         *                 before pumping out audio
         * E. Audio - not now - disabled for now
         * G. Core
         */
#if INCLUDE_BIOS_INT_HANDLER
        wpRegs->IRQENABLE_SET = (CSL_HDMI_WP_IRQENABLE_PHY_LINK_CONNECT_MASK |
                                 CSL_HDMI_WP_IRQENABLE_CORE_INTR_MASK        |
                                 CSL_HDMI_WP_IRQENABLE_PHY_LINK_DISCONNECT_MASK
                                 );
#endif /* INCLUDE_BIOS_INT_HANDLER */

        /* Following steps only applicable for a master generating the timmings
           signal to core */
        if (instHandle->config.wpConfig.isSlaveMode == 0x0)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "Configuring as Master mode");
            /* Step 2 - Configure vido timings parameters */
            temp =  ((instHandle->config.wpConfig.hbp <<
                      CSL_HDMI_WP_VIDEO_TIMING_H_HBP_SHIFT) &
                    CSL_HDMI_WP_VIDEO_TIMING_H_HBP_MASK);
            temp |= ((instHandle->config.wpConfig.hfp <<
                      CSL_HDMI_WP_VIDEO_TIMING_H_HFP_SHIFT) &
                    CSL_HDMI_WP_VIDEO_TIMING_H_HFP_MASK);
            temp |= ((instHandle->config.wpConfig.hsw <<
                      CSL_HDMI_WP_VIDEO_TIMING_H_HSW_SHIFT) &
                    CSL_HDMI_WP_VIDEO_TIMING_H_HSW_MASK);

            wpRegs->VIDEO_TIMING_H  = temp;

            temp =  ((instHandle->config.wpConfig.vbp <<
                      CSL_HDMI_WP_VIDEO_TIMING_V_VBP_SHIFT) &
                    CSL_HDMI_WP_VIDEO_TIMING_V_VBP_MASK);
            temp |= ((instHandle->config.wpConfig.vfp <<
                      CSL_HDMI_WP_VIDEO_TIMING_V_VFP_SHIFT) &
                    CSL_HDMI_WP_VIDEO_TIMING_V_VFP_MASK);
            temp |= ((instHandle->config.wpConfig.vsw <<
                      CSL_HDMI_WP_VIDEO_TIMING_V_VSW_SHIFT) &
                    CSL_HDMI_WP_VIDEO_TIMING_V_VSW_MASK);

            wpRegs->VIDEO_TIMING_V  = temp;

            /* Step 4 - Configure polarity of the syncs - in case of master */
            if (instHandle->config.wpConfig.vSyncPol != 0x0)
            {
                wpRegs->VIDEO_CFG |= CSL_HDMI_WP_VIDEO_CFG_VSYNC_POL_MASK;
            }
            else
            {
                wpRegs->VIDEO_CFG &= ~(CSL_HDMI_WP_VIDEO_CFG_VSYNC_POL_MASK);
            }
            if (instHandle->config.wpConfig.hSyncPol != 0x0)
            {
                wpRegs->VIDEO_CFG |= CSL_HDMI_WP_VIDEO_CFG_HSYNC_POL_MASK;
            }
            else
            {
                wpRegs->VIDEO_CFG &= ~(CSL_HDMI_WP_VIDEO_CFG_HSYNC_POL_MASK);
            }
        }

        /* Step 4, 5 and 6 */
        /* Packing mode */
        temp = wpRegs->VIDEO_CFG;
        /* Clear out previous packing mode config and re-configure */
        temp &= (~(CSL_HDMI_WP_VIDEO_CFG_PACKING_MODE_MASK));
        temp |= ((instHandle->config.wpConfig.packMode <<
                  CSL_HDMI_WP_VIDEO_CFG_PACKING_MODE_SHIFT) &
                 CSL_HDMI_WP_VIDEO_CFG_PACKING_MODE_MASK);

        /* Invert if required - follows input otherwise */
        if (instHandle->config.wpConfig.isVSyncPolInverted != 0x0)
        {
            temp |= CSL_HDMI_WP_VIDEO_CFG_CORE_VSYNC_INV_MASK;
        }
        else
        {
            temp &= (~(CSL_HDMI_WP_VIDEO_CFG_CORE_VSYNC_INV_MASK));
        }
        if (instHandle->config.wpConfig.isHSyncPolInverted != 0x0)
        {
            temp |= CSL_HDMI_WP_VIDEO_CFG_CORE_HSYNC_INV_MASK;
        }
        else
        {
            temp &= (~(CSL_HDMI_WP_VIDEO_CFG_CORE_HSYNC_INV_MASK));
        }

        if (instHandle->isInterlaced == TRUE)
        {
            temp |= CSL_HDMI_WP_VIDEO_CFG_PROGRESSIVE_INTERLACE_MASK;
        }
        else
        {
            temp &= (~(CSL_HDMI_WP_VIDEO_CFG_PROGRESSIVE_INTERLACE_MASK));
        }

        /*
         * Configure SLAVE/MASTER mode
         * In case of master mode, set the deep color modes also. Note that TCLK
         * would change depending on the deep color 1.25XPCLK 10bits, 1.5*PCLK
         * for 12 bits.
         */
        if (instHandle->config.wpConfig.isSlaveMode == 0x0)
        {
            temp |= (CSL_HDMI_WP_VIDEO_CFG_MODE_MASK);
            temp |= instHandle->config.wpConfig.width;
        }
        else
        {
            temp &= (~(CSL_HDMI_WP_VIDEO_CFG_MODE_MASK));
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "Operating wrapper as slave");
        }
        wpRegs->VIDEO_CFG   =   temp;
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "configureWrapper<<<<");
    return (rtnValue);
}


/**
 *  configureCoreInput
 *  \brief  This function configures the interface between the wrapper and core.
 *          Configure the bus width, sync source and edge for lataching of data/
 *          control.
 *
 *  \par CAUTION : The number of lines/channel between in the core and the
 *                 wrapper is not a configureable option. Recommended to use the
 *                 default
 *
 */
static int configureCoreInput(HdmiObject_t *instHandle)
{
    volatile u32         temp;
    Vps_HdmiCoreInputConfig *cfg    =   NULL;

    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>configureCoreInput");
    HDMI_GT_assert(hdmiTrace, (instHandle != NULL));
    HDMI_GT_assert(hdmiTrace, (instHandle->coreRegOvrLay != NULL));
    while(TRUE)
    {
        cfg = &(instHandle->config.coreIpConfig);
        /*
         * Step 1. Configure the width of the input bus. Update
         *          VID_ACEN.WIDE_BUS with user given & SYS_CTRL1.BSEL to 1
         * Step 2. Configure the sources for sync signals
         *         if VPS_HAL_HDMI_SYNC_EXTRACT - VID_MODE.SYNCEX = 1
         *         if VPS_HAL_HDMI_SYNC_GENERATE - DE_CTRL.DE_GEN = 1 and de_top
         *         de_dly, etc...
         *         if VPS_HAL_HDMI_SYNC_SOURCED - SYS_CTRL1.VEN/HEN = 1
         * Step 3. Configure the edge to latch on.
         */
        /* Step 1 bus width config */
        temp = instHandle->coreRegOvrLay->VID_ACEN;
        temp &= (~(CSL_HDMI_VID_ACEN_WIDE_BUS_MASK));
        temp |= ((cfg->dataBusWidth << CSL_HDMI_VID_ACEN_WIDE_BUS_SHIFT) &
                CSL_HDMI_VID_ACEN_WIDE_BUS_MASK);
        instHandle->coreRegOvrLay->VID_ACEN = temp;

        /* Step 1 and 3 */
        temp = instHandle->coreRegOvrLay->SYS_CTRL1;
        temp &= (~(CSL_HDMI_SYS_CTRL1_BSEL_MASK |CSL_HDMI_SYS_CTRL1_EDGE_MASK));
        if (cfg->edgePol != 0x0)
        {
            temp |= CSL_HDMI_SYS_CTRL1_EDGE_MASK;
        }
        temp |= CSL_HDMI_SYS_CTRL1_BSEL_MASK;
        instHandle->coreRegOvrLay->SYS_CTRL1 = temp;

        /* Step 2 Sync signals configurations */
        if (cfg->syncGenCfg == VPS_HAL_HDMI_SYNC_EXTRACT)
        {
            temp = instHandle->coreRegOvrLay->VID_MODE;
            temp &= (~(CSL_HDMI_VID_MODE_SYNCEX_MASK));
            temp |= CSL_HDMI_VID_MODE_SYNCEX_MASK;
            instHandle->coreRegOvrLay->VID_MODE = temp;
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "Embedded syncs is being used");
        }
        else if (cfg->syncGenCfg == VPS_HAL_HDMI_SYNC_GENERATE)
        {
            temp = instHandle->coreRegOvrLay->DE_CTRL;
            temp &= (~(CSL_HDMI_DE_CTRL_DE_GEN_MASK));
            temp |= CSL_HDMI_DE_CTRL_DE_GEN_MASK;
            instHandle->coreRegOvrLay->DE_CTRL = temp;

            instHandle->coreRegOvrLay->DE_DLY =
                (cfg->deDelayCfg.DE_DLY & CSL_HDMI_DE_DLY_DE_DLY_MASK);
            instHandle->coreRegOvrLay->DE_TOP =
                (cfg->deDelayCfg.DE_TOP & CSL_HDMI_DE_TOP_DE_TOP_MASK);
            instHandle->coreRegOvrLay->DE_CNTL =
                (cfg->deDelayCfg.DE_CNTL & CSL_HDMI_DE_CNTL_DE_CNT_MASK);
            instHandle->coreRegOvrLay->DE_CNTH =
                (cfg->deDelayCfg.DE_CNTH & CSL_HDMI_DE_CNTH_DE_CNT_MASK);
            instHandle->coreRegOvrLay->DE_LINL =
                (cfg->deDelayCfg.DE_LINL & CSL_HDMI_DE_LINL_DE_LIN_MASK);
            instHandle->coreRegOvrLay->DE_LINH =
                (cfg->deDelayCfg.DE_LINH & CSL_HDMI_DE_LINH_DE_LIN_MASK);
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "Syncs being generated");
        }
        else
        {
#if 0
            instHandle->coreRegOvrLay->DE_TOP   =   0x19u;
            instHandle->coreRegOvrLay->DE_CNTL  =   0x0u;
            instHandle->coreRegOvrLay->DE_CNTH  =   0x5u;
            instHandle->coreRegOvrLay->DE_LINL  =   0xD0u;
            instHandle->coreRegOvrLay->DE_LINH  =   0x2u;

            instHandle->coreRegOvrLay->DE_DLY   =   0x4u;

            instHandle->coreRegOvrLay->DE_CTRL  =   0x41u;
#else
            instHandle->coreRegOvrLay->DE_CTRL  =   0x1u;
#endif
            /* Default to be sourced from wrapper/hdvenc */
            temp = instHandle->coreRegOvrLay->SYS_CTRL1;
            /* Clear out the required bits */
            /* temp &= (~(CSL_HDMI_SYS_CTRL1_EDGE_MASK)); Not required as the
				EDGE would still required to be programmed */
            temp |= CSL_HDMI_SYS_CTRL1_VEN_MASK;
            temp |= CSL_HDMI_SYS_CTRL1_HEN_MASK;
            instHandle->coreRegOvrLay->SYS_CTRL1 =   temp;
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "Descrete syncs and being sourced");
        }
#ifdef USE_SILICON_RECOM_2306
        instHandle->coreRegOvrLay->IADJUST  =   0x0;
#endif

        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "configureCoreInput<<<<");
    return (VPS_SOK);
}


/**
 *  configureCoreDataPath
 *  \brief  Enables / configure each of the blocks in the core. Such as Dither
 *          CSC, etc... .
 */
static int configureCoreDataPath(HdmiObject_t *instHandle)
{
    int                           rtnValue    =   VPS_SOK;
    Vps_HdmiCoreDataPathConfig      *pathCfg    =   NULL;
    u32                          tempVidAcen;
    u32                          tempVidMode;
    u32                          tempVidDither;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>configureCoreDataPath");
    HDMI_GT_assert(hdmiTrace, (instHandle != NULL));
    HDMI_GT_assert(hdmiTrace, (instHandle->coreRegOvrLay != NULL));
    while(TRUE)
    {
        tempVidAcen     =   instHandle->coreRegOvrLay->VID_ACEN;
        tempVidMode     =   instHandle->coreRegOvrLay->VID_MODE;

        pathCfg =   &(instHandle->config.corePathConfig);
        /*
         *  If configured to enable a block, enable it.
         *  In an enabled block If configured to override default co-efficients
         *  overrid them
         *  Blocks enabled / checked in the following order
         *  1. 422 to 444 up sampler
         *  2. YCbCr to RGB color space converter
         *  2.1. Configure the standard color space co-efficients to be used.
         *       either BT.709 or BT.601 statnard co-efficients
         *  3. RGB Range expander
         *  4. Dithering module enable
         *  5. Output width
         *  6. RGB to YCbCr color space converter
         *  7. Range compression
         *  8. 444 to 422 Decimation
         */

        /*
         * Lets assume that all the block are desiabled by default. If required
         * we will enable them
         */
        tempVidMode &= (~(CSL_HDMI_VID_MODE_UPSMP_MASK  |
                          CSL_HDMI_VID_MODE_CSC_MASK    |
                          CSL_HDMI_VID_MODE_RANGE_MASK  |
                          CSL_HDMI_VID_MODE_DITHER_MASK));

        tempVidAcen &= (~(CSL_HDMI_VID_ACEN_RGB_2_YCBCR_MASK|
                          CSL_HDMI_VID_ACEN_RANGE_CMPS_MASK |
                          CSL_HDMI_VID_ACEN_DOWN_SMPL_MASK  |
                          CSL_HDMI_VID_ACEN_RANGE_CLIP_MASK |
                          CSL_HDMI_VID_ACEN_CLIP_CS_ID_MASK));
        /* 1. 422 to 444 up sampler */
        if (pathCfg->upSamplerEnable != 0x0)
        {
            tempVidMode |=  CSL_HDMI_VID_MODE_UPSMP_MASK;
        }

        /* 2. YCbCr to RGB color space converter  */
        if (pathCfg->cscYCbCr2RGBEnable != 0x0)
        {
            tempVidMode |=  CSL_HDMI_VID_MODE_CSC_MASK;
            /* Check for CSC configurations */
            rtnValue    =   configureCscYCbCr2RGB(instHandle);
            if (rtnValue != VPS_SOK)
            {
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not conf CSC YCbCr to RGB");
                break;
            }
        }
        /*
         * 2.1 Configure the default co-efficents standards to be used either
         * BT.709 or BT.601
         */
        if (pathCfg->csConvertStandard != 0x0)
        {
            instHandle->coreRegOvrLay->VID_CTRL |=
                                                CSL_HDMI_VID_CTRL_CSCSEL_MASK;
        }
        else
        {
            instHandle->coreRegOvrLay->VID_CTRL &=
                                            (~(CSL_HDMI_VID_CTRL_CSCSEL_MASK));
        }
        /* 3. RGB Range expander  */
        if (pathCfg->rangeExpRGBEnable != 0x0)
        {
            tempVidMode |=  CSL_HDMI_VID_MODE_RANGE_MASK;
        }
        /* 4. Dithering module enable */
        if (pathCfg->ditherEnable != 0x0)
        {
            tempVidDither = instHandle->coreRegOvrLay->VID_DITHER;
            tempVidMode |=  CSL_HDMI_VID_MODE_DITHER_MASK;
            tempVidDither &= (~(CSL_HDMI_VID_DITHER_M_D2_MASK       |
                                CSL_HDMI_VID_DITHER_UP2_MASK        |
                                CSL_HDMI_VID_DITHER_STR_422_EN_MASK |
                                CSL_HDMI_VID_DITHER_D_BC_EN_MASK    |
                                CSL_HDMI_VID_DITHER_D_GC_EN_MASK    |
                                CSL_HDMI_VID_DITHER_D_RC_EN_MASK    |
                                CSL_HDMI_VID_DITHER_DRD_MASK));
            /* Configure dithering parameters */
            if (pathCfg->ditherConfig.M_D2 != 0x0)
            {
                tempVidDither |= CSL_HDMI_VID_DITHER_M_D2_MASK;
            }
            if (pathCfg->ditherConfig.UP2 != 0x0)
            {
                tempVidDither |= CSL_HDMI_VID_DITHER_UP2_MASK;
            }
            if (pathCfg->ditherConfig.STR_422_EN != 0x0)
            {
                tempVidDither |= CSL_HDMI_VID_DITHER_STR_422_EN_MASK;
            }
            if (pathCfg->ditherConfig.D_BC_EN != 0x0)
            {
                tempVidDither |= CSL_HDMI_VID_DITHER_D_BC_EN_MASK;
            }
            if (pathCfg->ditherConfig.D_GC_EN != 0x0)
            {
                tempVidDither |= CSL_HDMI_VID_DITHER_D_GC_EN_MASK;
            }
            if (pathCfg->ditherConfig.D_RC_EN != 0x0)
            {
                tempVidDither |= CSL_HDMI_VID_DITHER_D_RC_EN_MASK;
            }
            if (pathCfg->ditherConfig.DRD != 0x0)
            {
                tempVidDither |= CSL_HDMI_VID_DITHER_DRD_MASK;
            }
            instHandle->coreRegOvrLay->VID_DITHER = tempVidDither;
        }

        /* 5. Output width */
        /*
         * Set the output width, note that when dither is enabled, the video
         * is dithered to the width specified here.
         * Otherwise, the viode data is trucated to the specified width
         */
        tempVidMode |=
            ((pathCfg->outputWidth << CSL_HDMI_VID_MODE_DITHER_MODE_SHIFT) &
              CSL_HDMI_VID_MODE_DITHER_MODE_MASK);

        instHandle->coreRegOvrLay->VID_MODE = tempVidMode;

        /* 6. RGB to YCbCr color space converter */
        if(pathCfg->cscRGB2YCbCrEnable != 0x0)
        {
            tempVidAcen |= CSL_HDMI_VID_ACEN_RGB_2_YCBCR_MASK;
            /* TBD configure co-efficients for CSC RGB to YCbCr */
        }
        /* 7. Range compression */
        if (pathCfg->rangeCompEnable != 0x0)
        {
            tempVidAcen |= CSL_HDMI_VID_ACEN_RANGE_CMPS_MASK;
        }
        /* 8. Enable 444 to 422 decimation */
        if (pathCfg->downSamplerEnable != 0x0)
        {
            tempVidAcen |= CSL_HDMI_VID_ACEN_DOWN_SMPL_MASK;
        }
        /* 9. Enable Range clip, if enabled would require to specify the
              color space */
        if (pathCfg->rangeClipEnable != 0x0)
        {
            tempVidAcen |= CSL_HDMI_VID_ACEN_RANGE_CLIP_MASK;
            if (pathCfg->clipColorSpace != 0x0)
            {
                tempVidAcen |= CSL_HDMI_VID_ACEN_CLIP_CS_ID_MASK;
            }
        }
        instHandle->coreRegOvrLay->VID_ACEN  =   tempVidAcen;
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "configureCoreDataPath<<<<");
    return (rtnValue);
}


/**
 *  configureCore
 *  \brief  Configures the flow of video from wrapper to different functional
 *          blocks such as CSC, UP Sampler, etc.. Enables the interrupts and
 *          configures HALs policies.
 */
static int configureCore(HdmiObject_t *instHandle)
{
    int   rtnValue    =   VPS_SOK;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>configureCore");
    HDMI_GT_assert(hdmiTrace, (instHandle != NULL));
    HDMI_GT_assert(hdmiTrace, (instHandle->coreRegOvrLay != NULL));
    while(TRUE)
    {
        /*
         * Step 1. Configure the input path
         * Step 2. Configure the data path - if enabled
         * Step 3. Enables interrupts that we are interested in
         * Step 4. Driver Policies
         * Step 5. Powerup and enable core
         * Step 6. Check if a sink is detcted
         */
#ifdef USE_SILICON_RECOM_2306
/************************Silicon Image Recomndations****************************/
        instHandle->coreRegOvrLay->TEST_TXCTRL &= (~(
                                        CSL_HDMI_TEST_TXCTRL_DIV_ENC_BYP_MASK));
#endif /* USE_SILICON_RECOM_2306 */

        /* Step 1 - Configure input path */
        if (instHandle->config.useCoreIpConfig != 0x0)
        {
            rtnValue    =   configureCoreInput(instHandle);
            if (rtnValue != VPS_SOK)
            {
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not configure core input");
                break;
            }
        }
        /* Step 2 - Configure the data path - if enabled */
        if (instHandle->config.useCorePathConfig != 0x0)
        {
            rtnValue    =   configureCoreDataPath(instHandle);
            if (rtnValue != VPS_SOK)
            {
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not configure data path");
                break;
            }
        }
        /* Step 3 - Enable interrupts that we are interested */
        /* Enable HPD and VSYNC, Others disabled for now. */
        /*      VSYNC is used here, as we would require to track the number of
         *      VSYNC before we could enable HDCP.
         *      The wrapper WILL not generate VSYNC interrupts, We would have to
         *      rely on VPSS sub-system to track the VSYNC.
         *      Instead rely HDMI core detecting the vSync.
         */
#if INCLUDE_BIOS_INT_HANDLER
        instHandle->coreRegOvrLay->INT_UNMASK1 |= CSL_HDMI_INT_UNMASK1_HPD_MASK;
        instHandle->coreRegOvrLay->INT_UNMASK2 |=
                                            CSL_HDMI_INT_UNMASK2_VSYNC_REC_MASK;
#endif /* INCLUDE_BIOS_INT_HANDLER */

        /* Step 4. Driver Policies  */
        rtnValue = configureHdmiPolicies(instHandle);
        if (rtnValue != VPS_SOK)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not configure HAL Policies");
            break;
        }

        /* Disable audio NCTS Packets */
        instHandle->coreRegOvrLay->ACR_CTRL =   0x0u;
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "configureCore<<<<");
    return (rtnValue);
}


/**
 *  configureHdmiPolicies
 *  \brief  Configure the rules of HDMI Hal, like pixel repeatations, packetizer
 *          among others. Most of these rules are not directly controlled by
 *          the applications.
 */
static int configureHdmiPolicies(HdmiObject_t *instHandle)
{
    int    rtnValue   =   VPS_SOK;
    volatile u32 temp;
    volatile u32 ditherModeValue;

    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>configureHdmiPolicies");
    HDMI_GT_assert(hdmiTrace, (instHandle != NULL));
    HDMI_GT_assert(hdmiTrace, (instHandle->coreRegOvrLay != NULL));
    while(TRUE)
    {
        /* Steps
         * 1. Determine pixel repeatation requirements.
         * 3. Determine if deep color packets requires to sent out
         * 4. Configure the packetizer, the width of the channels/bits-pixels
         * 5. Mutiply input clock if required - support for higher sampling
         *    rates in audio
         */

        temp = instHandle->coreRegOvrLay->VID_CTRL;

        /* Step 1 -   Determine Pixel repeatation - if required - the idea is
                      HDMI requires 25 MPixels/sec, for a given resolution and
                      frame rate if the pixel rate falls below 25 MPixels/sec
                      Pixels requries to be replicated.
                      When audio is supported this might required to be updated
                      in cases of support for multi-channel/higher sample rate
                      audio streams, if required. */

        /* No pixel repeatation by default */
        temp &= (~(CSL_HDMI_VID_CTRL_ICLK_MASK));

        rtnValue = determinePixelRepeatation(instHandle);
        if (rtnValue == HDMI_PIXEL_REPLECATED_ONCE)
        {
            /* Repeat once */
            temp |= (((0x01u) << CSL_HDMI_VID_CTRL_ICLK_SHIFT) &
                                    CSL_HDMI_VID_CTRL_ICLK_MASK);
        }
        else if (rtnValue == HDMI_PIXEL_REPLECATED_FOUR_TIMES)
        {
            temp |= CSL_HDMI_VID_CTRL_ICLK_MASK;
        }
        else if (rtnValue == VPS_SOK)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "No Pixel repeatation required");
        }
        else
        {
            /* Error let the caller know */
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not determine pixel ");
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "that would be required.");
            break;
        }
        instHandle->coreRegOvrLay->VID_CTRL = temp;

        /* Step 3 */
        temp = instHandle->coreRegOvrLay->HDMI_CTRL;
        temp &= (~(CSL_HDMI_HDMI_CTRL_DC_EN_MASK        |
                   CSL_HDMI_HDMI_CTRL_PACKET_MODE_MASK));
        /* Deep color packets are to be sent when operating in deep color mode.
           ASSUMING that bits/channel == 3 * bits/pixel
           i.e. 8 bits/chan = 24 bits/pixel
           Bits/channel is finally controlled by dithering module. At this point
           dither module should have been configured, read and determine if
           deep color mode is selected.
           Note that irrespective of the state of dither module(enabled/disable)
           the output is dithred/truncated to width specified in DITHER_MODE */
        ditherModeValue = ((instHandle->coreRegOvrLay->VID_MODE &
                            CSL_HDMI_VID_MODE_DITHER_MODE_MASK ) >>
                            CSL_HDMI_VID_MODE_DITHER_MODE_SHIFT);
        /* Deep color mode packets are to sent or not */
        if (ditherModeValue != HDMI_VID_MODE_DITHER_TO_24_BITS_MODE)
        {
            /* Deep color mode - as bits/pixel >= 10 bits */
            temp |= CSL_HDMI_HDMI_CTRL_DC_EN_MASK;
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "Deep color mode selected");
        }
        /* Step 3 - Specify the width of the pixels being sent out to
                        packetizer -default at 24bits/pixel*/
        temp |= ((HDMI_CTRL_PACKET_MODE_24BITS_PIXEL) <<
                  CSL_HDMI_HDMI_CTRL_PACKET_MODE_SHIFT);
        if (ditherModeValue == HDMI_VID_MODE_DITHER_TO_30_BITS_MODE)
        {
            temp |= ((HDMI_CTRL_PACKET_MODE_30BITS_PIXEL) <<
                      CSL_HDMI_HDMI_CTRL_PACKET_MODE_SHIFT);
        }
        if (ditherModeValue == HDMI_VID_MODE_DITHER_TO_36_BITS_MODE)
        {
            temp |= ((HDMI_CTRL_PACKET_MODE_36BITS_PIXEL) <<
                      CSL_HDMI_HDMI_CTRL_PACKET_MODE_SHIFT);
        }
        /* Use HDMI mode */
        /* DVI mode could be used in debug mode / should this configureable
           to applications? */
        temp |= CSL_HDMI_HDMI_CTRL_HDMI_MODE_MASK;

        instHandle->coreRegOvrLay->HDMI_CTRL = temp;

        /*
         * To support higher sampling frequency/channels of audio, we would
         * require to increase the input clock frequency to offer sufficient
         * bandwidth required to pump out audio. The input clock could be
         * increased by specifying the multipler. Right now, hard coding to
         * the multiplier to 0x01 increase when needed - audio
         */
        temp = instHandle->coreRegOvrLay->TMDS_CTRL;
        temp |= (CSL_HDMI_TMDS_CTRL_TCLKSEL_MASK &
                    (HDMI_TMDS_CTRL_IP_CLOCK_MULTIPLIER_AUDIO <<
                    CSL_HDMI_TMDS_CTRL_TCLKSEL_SHIFT));
        instHandle->coreRegOvrLay->TMDS_CTRL = temp;
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "configureHdmiPolicies<<<<");
    return (rtnValue);
}

static int    configureControlPacket  (HdmiObject_t   *instHandle)
{
    int   rtnValue    =   VPS_SOK;

    /*
     * Steps
     * 1. Program DC header with 0x3 - Default value - reprogram to ensure
     *    This value indicates generic control packet header
     * 2. Set / Clear AV Mute flags - On HDCP this requires to be updated.
     */

    instHandle->coreRegOvrLay->DC_HEADER = 0x3u;

    /* Go over the CP_BYTE1 Description */
    instHandle->coreRegOvrLay->CP_BYTE1 = 0x10u;
    return (rtnValue);
}

/**
 *  configureAVIInfoFrame
 *  \brief  Initializes the AVI Info packet with details defined in the instance
 *          AIV configuration.
 *  \par CAUTION : Before this function is called, the pixel repeatation should
 *                 have been determined and configured in VID_CTRL.ICLK
 *                 If AVI Info frames are being transmitted - will be stopped -
 *                 re-started
 */
static int configureAVIInfoFrame(HdmiObject_t *instHandle, Bool transmitOnce)
{
    int   rtnValue    =   VPS_SOK;
    UInt8   checkSum    =   0x0;
    UInt8   dataByteIndex = 0x0;
    volatile UInt8          tempDataByte =  0x0;
    Vps_HdmiAviInfoFrameCfg *infoPkt     =  NULL;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>configureAVIInfoFrame");
    HDMI_GT_assert(hdmiTrace, (instHandle != NULL));
    HDMI_GT_assert(hdmiTrace, (instHandle->coreRegOvrLay != NULL));
    while(TRUE)
    {
        infoPkt =   &(instHandle->config.infoFrameConfig.aviData);
        /*
         * 1. Specify the packet type, version and length. All standard for AVI
         *    Info packet
         * 2. Program the PACKET
         * 3. Checksum - HDMI version 1.3a requires that BYTE sum of all the
         *    valid bytes in the packet + checksum itself be == 0x0
         *    Checksum will computed while updating the info packet and updated
         *    end of this function.
         *    checksum = 0x100 - tempChecksum;
         *    so that byte sum of checksum + tempChecksum == 0x0
         * 4. Pixel repeatation
         *    A. Load the pixel repeatation value in VID_CTRL.ICLK into packet
         *    B. Check the if channels are being doubled i.e. demuxed (i.e.
         *       VID_MODE.DEMUX) if enabled the effective pixel repeatation
         *       should be increased by one
         */

        /* Step 1 - Configure type, version and length */
        instHandle->coreRegOvrLay->AVI_TYPE = (CSL_HDMI_AVI_TYPE_AVI_TYPE_MASK
                                                & HDMI_AVI_INFOFRAME_PKT_TYPE);
        checkSum    =   HDMI_AVI_INFOFRAME_PKT_TYPE;
        instHandle->coreRegOvrLay->AVI_VERS = (CSL_HDMI_AVI_VERS_AVI_VERS_MASK
                                                & HDMI_AVI_INFOFRAME_PKT_VER);
        checkSum    +=  HDMI_AVI_INFOFRAME_PKT_VER;
        instHandle->coreRegOvrLay->AVI_LEN  = (CSL_HDMI_AVI_LEN_AVI_LEN_MASK
                                                & HDMI_AVI_INFOFRAME_PKT_LEN);
        checkSum    +=  HDMI_AVI_INFOFRAME_PKT_LEN;
        /* Step 2 - Program the packet - not using a structure with bit feilds
                    as they would not be portable */
        /* RGB or YCbCr - Y0 and Y1 */
        tempDataByte =(((UInt8)infoPkt->outputCs << HDMI_SHIFT_BY_5) &
                        HDMI_AVI_INFOFRAME_Y0_Y1_MASK);
        /* Active information A0, if so require to update R0-R3 byte 2 */
        if (infoPkt->useActiveAspectRatio == TRUE)
        {
            tempDataByte |= HDMI_AVI_INFOFRAME_A0_MASK;
        }
        /* Bar information B0 and B1 - if so require to update byte 6-13 */
        if (infoPkt->barInfo.barInfoValid != 0x0)
        {
            tempDataByte |= ((((UInt8)infoPkt->barInfo.barInfoValid) <<
                                HDMI_SHIFT_BY_2) &
                                HDMI_AVI_INFOFRAME_B0_B1_MASK);
        }
        /* Scan information S1 and S0 */
        tempDataByte |= (((UInt8)infoPkt->scanInfo) &
                        HDMI_AVI_INFOFRAME_S0_S1_MASK);
        /* First data byte of the packet */
        instHandle->coreRegOvrLay->AVI_DBYTE[dataByteIndex++]  =   tempDataByte;
        checkSum  +=   tempDataByte;

        /* Colorimetry C1 C0 */
        tempDataByte = (((UInt8)infoPkt->colorimetryInfo << HDMI_SHIFT_BY_6) &
                        HDMI_AVI_INFOFRAME_C0_C1_MASK);

        /* Aspect ratio M1 M0 */
        tempDataByte |= (((UInt8)infoPkt->aspectRatio << HDMI_SHIFT_BY_4) &
                        HDMI_AVI_INFOFRAME_M0_M1_MASK);
        /* R3 to R1 */
        if (infoPkt->useActiveAspectRatio == TRUE)
        {
            tempDataByte |= (((UInt8)infoPkt->activeAspectRatio) &
                             HDMI_AVI_INFOFRAME_R0_R3_MASK);
        }
        /* Second data byte of the packet */
        instHandle->coreRegOvrLay->AVI_DBYTE[dataByteIndex++]  =   tempDataByte;
        checkSum  +=   tempDataByte;

        /* It Content present ITC */
        tempDataByte    =   0x0;
        if (infoPkt->itContentPresent != 0x0)
        {
            tempDataByte = HDMI_AVI_INFOFRAME_ITC_MASK;
        }
        /* Extended colorimetry range EC3 to EC0 */
        tempDataByte |= (((UInt8)infoPkt->extColorimetry << HDMI_SHIFT_BY_4) &
                        HDMI_AVI_INFOFRAME_EC2_EC0_MASK);
        /* Quantization range range Q1 to Q0 */
        tempDataByte |= (((UInt8)infoPkt->quantizationRange << HDMI_SHIFT_BY_2)
                         & HDMI_AVI_INFOFRAME_Q1_Q0_MASK);
        /* Non-Uniform scaling S0 and S1 */
        tempDataByte |= ((UInt8)infoPkt->nonUniformSc &
                        HDMI_AVI_INFOFRAME_SC1_SC0_MASK);
        /* Third data byte of the packet */
        instHandle->coreRegOvrLay->AVI_DBYTE[dataByteIndex++]  =   tempDataByte;
        checkSum  +=   tempDataByte;
        /* Fourth data byte of the packet */
        switch(instHandle->config.displayMode)
        {
            case VPS_HDMI_MODE_720P_60:
            {
                infoPkt->formatIdentier = 4u;
                break;
            }
            case VPS_HDMI_MODE_1080P_30:
            {
                infoPkt->formatIdentier = 34u;
                break;
            }
            case VPS_HDMI_MODE_1080I_60:
            {
                infoPkt->formatIdentier = 5u;
                break;
            }
            case VPS_HDMI_MODE_1080P_60:
            {
                infoPkt->formatIdentier = 16u;
                break;
            }
            default :
            {
                rtnValue = VPS_EUNSUPPORTED_CMD;
                break;
            }
        }
        if (rtnValue != VPS_SOK)
        {
            break;
        }
        tempDataByte = (UInt8)infoPkt->formatIdentier;

        instHandle->coreRegOvrLay->AVI_DBYTE[dataByteIndex++]  =
            ((UInt8) tempDataByte + HDMI_AVI_INFOFRAME_VIC6_VIC0_MASK);

        checkSum  +=   tempDataByte;

        /* Pixel Repeatation */
        tempDataByte = (UInt8) (CSL_HDMI_VID_CTRL_ICLK_MASK &
                                instHandle->coreRegOvrLay->VID_CTRL);

        /* TODO - Why do we require to up the pixel repeatation when demux is
           is used. */
        if ((instHandle->coreRegOvrLay->VID_MODE & CSL_HDMI_VID_MODE_DEMUX_MASK)
            == CSL_HDMI_VID_MODE_DEMUX_MASK)
        {
            /* Do not worry about exceeding the upper limit.
               Pixel could be repeated a maximum of 4 times (value 0x03).
               The pixel repeatation has 4 bit space in info packet which could
               be a maximum of 0x0F, but limited to 0x09 */
            tempDataByte++;
        }
        instHandle->coreRegOvrLay->AVI_DBYTE[dataByteIndex++] =
            HDMI_AVI_INFOFRAME_PR3_PR0_MASK & tempDataByte;
        checkSum  +=   tempDataByte;

        /* Bar information */
        if (infoPkt->barInfo.barInfoValid != 0x0)
        {
            /* Top Bar */
            tempDataByte = (UInt8)(infoPkt->barInfo.topBar & 0xFF);
            instHandle->coreRegOvrLay->AVI_DBYTE[dataByteIndex++] =tempDataByte;
            checkSum  +=   tempDataByte;

            tempDataByte =
                 (UInt8)((infoPkt->barInfo.topBar >> HDMI_SHIFT_BY_8)  & 0xFF);
            instHandle->coreRegOvrLay->AVI_DBYTE[dataByteIndex++] =tempDataByte;
            checkSum  +=   tempDataByte;

            /* Bottom Bar */
            tempDataByte = (UInt8)(infoPkt->barInfo.bottomBar & 0xFF);
            instHandle->coreRegOvrLay->AVI_DBYTE[dataByteIndex++] =tempDataByte;
            checkSum  +=   tempDataByte;

            tempDataByte =
                (UInt8)((infoPkt->barInfo.bottomBar >> HDMI_SHIFT_BY_8) & 0xFF);
            instHandle->coreRegOvrLay->AVI_DBYTE[dataByteIndex++] =tempDataByte;
            checkSum  +=   tempDataByte;

            /* Left Bar */
            tempDataByte = (UInt8)(infoPkt->barInfo.leftBar & 0xFF);
            instHandle->coreRegOvrLay->AVI_DBYTE[dataByteIndex++] =tempDataByte;
            checkSum  +=   tempDataByte;

            tempDataByte =
                (UInt8)((infoPkt->barInfo.leftBar >> HDMI_SHIFT_BY_8)  & 0xFF);
            instHandle->coreRegOvrLay->AVI_DBYTE[dataByteIndex++] =tempDataByte;
            checkSum  +=   tempDataByte;

            /* Right Bar */
            tempDataByte = (UInt8)(infoPkt->barInfo.rightBar & 0xFF);
            instHandle->coreRegOvrLay->AVI_DBYTE[dataByteIndex++] =tempDataByte;
            checkSum  +=   tempDataByte;

            tempDataByte =
                (UInt8)((infoPkt->barInfo.rightBar >> HDMI_SHIFT_BY_8) & 0xFF);
            instHandle->coreRegOvrLay->AVI_DBYTE[dataByteIndex++] =tempDataByte;
            checkSum  +=   tempDataByte;
        }

        /* Checksum */
        /* Casting explictly to avoid MISRAC errors */
        instHandle->coreRegOvrLay->AVI_CHSUM =
            (UInt8) (HDMI_AVI_INFOFRAME_CONST_0x100 - (UInt16) checkSum);
        HDMI_GT_1trace(hdmiTrace, HDMI_GT_INFO, "AVI - Computed check sum %d", checkSum);
        HDMI_GT_1trace(hdmiTrace, HDMI_GT_INFO, "Check sum sent %d",
                                    instHandle->coreRegOvrLay->AVI_CHSUM);
        HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "Sent check sum + all bytes should 0x0");
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "configureAVIInfoFrame<<<<");
    return(rtnValue);
}


/**
 *  configureCscYCbCr2RGB
 *  \brief  Configure YCbCr to RGB Color Space converter
 */

static int configureCscYCbCr2RGB (HdmiObject_t *instHandle)
{
    Vps_HdmiCSC_YCbCr2RGB_Ctrl  *ctrl       =   NULL;
    u32                      tempxvYCC2RGBCtl;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>configureCscYCbCr2RGB");
    HDMI_GT_assert(hdmiTrace, (instHandle != NULL));
    HDMI_GT_assert(hdmiTrace, (instHandle->coreRegOvrLay != NULL));
    while(TRUE)
    {
        ctrl = &(instHandle->config.corePathConfig.cscYCbCr2RGBConfig);
        tempxvYCC2RGBCtl = instHandle->coreRegOvrLay->XVYCC2RGB_CTL;
        HDMI_GT_assert(hdmiTrace, (ctrl != NULL));
        /*
         * Note - Ignoring BYP_ALL as this module would not be enabled when
         *        VID_MODE.CSC is reset
         */
        /* Reset all that we are going to configure */
        tempxvYCC2RGBCtl &= (~(CSL_HDMI_XVYCC2RGB_CTL_EXP_ONLY_MASK     |
                               CSL_HDMI_XVYCC2RGB_CTL_BYP_ALL_MASK      |
                               CSL_HDMI_XVYCC2RGB_CTL_SW_OVR_MASK       |
                               CSL_HDMI_XVYCC2RGB_CTL_FULLRANGE_MASK    |
                               CSL_HDMI_XVYCC2RGB_CTL_XVYCCSEL_MASK));
        if (ctrl->enableRngExp != 0x0)
        {
            tempxvYCC2RGBCtl |= CSL_HDMI_XVYCC2RGB_CTL_EXP_ONLY_MASK;
        }

        if (ctrl->enableFullRngExp != 0x0)
        {
            tempxvYCC2RGBCtl |= CSL_HDMI_XVYCC2RGB_CTL_FULLRANGE_MASK;
        }
        if (ctrl->srcCsSel != 0x0)
        {
            tempxvYCC2RGBCtl |= CSL_HDMI_XVYCC2RGB_CTL_XVYCCSEL_MASK;
        }
        if (ctrl->customCoEff != 0x0)
        {
            /* Load the custom coefficitents - using memcopy to load as the
               structures maps to register */
            VpsUtils_memcpy((HDMI_Ptr)(&(instHandle->coreRegOvrLay->Y2R_COEFF_LOW)),
                            ((const void *) &(ctrl->coEff)),
                            sizeof(Vps_HdmiCSC_YCbCr2RGB_coEff));
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "Using custom co-effs");
        }
        instHandle->coreRegOvrLay->XVYCC2RGB_CTL = tempxvYCC2RGBCtl;
        break;
    }

    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "configureCscYCbCr2RGB<<<<");
    return (VPS_SOK);
}


/**
 *  validateInfoFrameCfg
 *  \brief  Validates the Information frame configurations that would be sent.
 *          Validates for range violation for now.
 */
static int validateInfoFrameCfg(Vps_HdmiInfoFrameCfg *config)
{
    int   rtnValue                     =   VPS_EBADARGS;
    Vps_HdmiAviInfoFrameCfg *aviData     =   NULL;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>validateInfoFrameCfg");
    while(TRUE)
    {
        if (config == NULL)
        {
            break;
        }
        aviData =   &(config->aviData);
        if (aviData->outputCs >= VPS_HAL_HDMI_AVIINFO_OP_CS_MAX)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "In correct color space");
            break;
        }
        if ((aviData->useActiveAspectRatio !=
                                VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATION_NO) &&
            (aviData->useActiveAspectRatio !=
                                VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATION_YES))
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Wrong aspect ratio");
            break;
        }
        if (aviData->scanInfo >= VPS_HAL_HDMI_AVIINFO_SCAN_INFO_MAX)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "In correct scan information");
            break;
        }
        if (aviData->colorimetryInfo >= VPS_HAL_HDMI_AVIINFO_COLORIMETRY_MAX)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "In correct colorimetry information");
            break;
        }
        if (aviData->aspectRatio >= VPS_HAL_HDMI_AVIINFO_ASPECTRATIO_MAX)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "In correct aspect ratio information");
            break;
        }
        if ((aviData->activeAspectRatio <
                            VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATIO_SAME) &&
            (aviData->activeAspectRatio >=VPS_HAL_HDMI_AVIINFO_ASPECTRATIO_MAX))
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "In correct active aspect ratio info");
            break;
        }
        if (aviData->nonUniformSc >=VPS_HAL_HDMI_AVIINFO_NONUNIFORM_SCALING_MAX)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "In correct non-uniform scaling info");
            break;
        }
        rtnValue = VPS_SOK;
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "validateInfoFrameCfg<<<<");
    return (rtnValue);
}


/**
 *  validateHdmiCoreIpConfig
 *  \brief  Validates the configureable parameters of the core.
 */
static int validateHdmiCoreIpConfig(Vps_HdmiCoreInputConfig *config)
{
    int   rtnValue    =   VPS_EBADARGS;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>validateHdmiCoreIpConfig");
    HDMI_GT_assert(hdmiTrace, (config != NULL));
    while(TRUE)
    {
        if (config->dataBusWidth > VPS_HAL_HDMI_10_BITS_CHAN_WIDTH)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Bus width should be <=30 bits/pixel");
            break;
        }

        if (config->syncGenCfg >= VPS_HAL_HDMI_SYNC_MAX)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Incorrect meathods used ");
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "for synchronization");
            break;
        }
        rtnValue = VPS_SOK;
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "validateHdmiCoreIpConfig<<<<");
    return (rtnValue);
}


/**
 *  validateHdmiWpConfig
 *  \brief  Validates the configureable parameters of the wrapper.
 */
static int validateHdmiWpConfig(Vps_HdmiWpConfig *config)
{
    int   rtnValue    =   VPS_EBADARGS;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>validateHdmiWpConfig");
    HDMI_GT_assert(hdmiTrace, (config != NULL));
    while(TRUE)
    {
        /* Conditions
           1. Debounce Receiver detect and receiver sense should be between 0x14
              0x01
           2. Only SLAVE mode supported
           3. Width should not be > 10 per channel i.e. 30 bits/pixel
           4. Check data packing mode */
        /* Check 1 */
        if ((config->debounceRecvDetect < 0x01) ||
            (config->debounceRecvDetect > 0x14))
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Debounce receiver detect incorrect");
            break;
        }
        if ((config->debounceRecvSens < 0x01) ||
            (config->debounceRecvSens > 0x14))
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Debounce receiver sens incorrect");
            break;
        }
        /* Check 2 */
        if (config->isSlaveMode == 0x0)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Warpper is not in SLAVE mode ");
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, " - Master mode cannot be supported");
            break;
        }
        /* Check 3 */
        if (config->width >= VPS_HAL_HDMI_12_BITS_CHAN_WIDTH)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Bus width should be < 36 bits/pixel");
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, " - 8 & 10 bits/channel is valid");
            break;
        }
        if (config->packMode >= VPS_HAL_HDMI_WP_NO_PACK)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Incorrect data packing mode");
            break;
        }
        rtnValue    =   VPS_SOK;
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "validateHdmiWpConfig<<<<");
    return (rtnValue);
}


/**
 *  validateHdmiPathConfig
 *  \brief  Validates the data path configuration of the core.
 */
static int validateHdmiPathConfig(Vps_HdmiCoreDataPathConfig *config)
{
    int   rtnValue    =   VPS_SOK;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>validateHdmiPathConfig");
    HDMI_GT_assert(hdmiTrace, (config != NULL));
    /*
     * 1. Check the channel width should be >= 8 and < 12
     * 2. If xvYCC is enabled - check for coefficients overide
     */
    while(TRUE)
    {
        if (config->outputWidth >= VPS_HAL_HDMI_MAX_BITS_CHAN_WIDTH)
        {
            HDMI_GT_1trace(hdmiTrace, HDMI_GT_ERR, "In valid output channel width",
                        config->outputWidth);
            rtnValue    =   VPS_EBADARGS;
            break;
        }
#ifdef Vps_HdmiCSC_xvYCC_Ctrl
        if (config->xvYCCCtrl.RGB2xvYCC_XV_CO_OV != 0x0)
        {
            if (config->xvYCCCtrl.RGB2xvYCC_Coefs == NULL)
            {
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Turned on RGB to xvYCC CSC");
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "With custom config");
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Without specifying coefficients");
                rtnValue    =   VPS_EBADARGS;
                break;
            }
        }
#endif /* if defined Vps_HdmiCSC_xvYCC_Ctrl */

        /* No issues - go through */
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "validateHdmiPathConfig<<<<");
    return (rtnValue);
}


/**
 *  checkAndCopyCfg
 *  \brief  Validates HDMI configurations, if configurations could be applied,
 *          the default configuration is update with the configuration supplied
 *          by the caller.
 */
static int checkAndCopyCfg(   HdmiObject_t            *instPtr,
                                Vps_HdmiConfigParams    *config)
{
    int   rtnValue    =   VPS_SOK;
    u32  cookie;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>checkAndCopyCfg");
    HDMI_GT_assert(hdmiTrace, (instPtr != NULL));
    HDMI_GT_assert(hdmiTrace, (config != NULL));
    while (TRUE)
    {
        if (config->useDcDisplayMode != 0x0)
        {
            /* Custom mode not supported for now TBD */
            if (config->displayMode >= VPS_HDMI_MAX_MODE)
            {
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR,
                    "Custom mode timmings not supported");
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "OR incorrect DC mode id");
                rtnValue    =   VPS_EUNSUPPORTED_CMD;
                break;
            }
            instPtr->config.displayMode = config->displayMode;
        }
        if (config->useWpConfig != 0x0)
        {
            rtnValue    =   validateHdmiWpConfig(&(config->wpConfig));
            if (rtnValue != VPS_SOK)
            {
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Wrapper config incorrect");
                break;
            }
            VpsUtils_memcpy((HDMI_Ptr)(&(instPtr->config.wpConfig)),
                            ((const void *)&(config->wpConfig)),
                            sizeof(Vps_HdmiWpConfig));
        }
        if (config->useCoreIpConfig != 0x0)
        {
            rtnValue    =   validateHdmiCoreIpConfig(&(config->coreIpConfig));
            if (rtnValue != VPS_SOK)
            {
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Core config incorrect");
                break;
            }
            VpsUtils_memcpy((HDMI_Ptr)(&(instPtr->config.coreIpConfig)),
                            ((const void *)&(config->coreIpConfig)),
                            sizeof(Vps_HdmiCoreInputConfig));
        }
        if (config->useCorePathConfig != 0x0)
        {
            rtnValue    =   validateHdmiPathConfig(&(config->corePathConfig));
            if (rtnValue != VPS_SOK)
            {
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Core data path config incorrect");
                break;
            }
            VpsUtils_memcpy((HDMI_Ptr)(&(instPtr->config.corePathConfig)),
                            ((const void *)&(config->corePathConfig)),
                            sizeof(Vps_HdmiCoreDataPathConfig));
        }
        if (config->useInfoFrameConfig != 0x0)
        {
            if (config->infoFrameConfig.useAviInfoData != 0x0)
            {
                /* Valildate info frame configurations - Just ensure that enums
                   values are not exceeded */
                rtnValue    =  validateInfoFrameCfg(&(config->infoFrameConfig));
                if (rtnValue != VPS_SOK)
                {
                    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Bad AVI Info frame data");
                    break;
                }
                VpsUtils_memcpy((HDMI_Ptr)(&(instPtr->config.infoFrameConfig)),
                                ((const void *)&(config->infoFrameConfig)),
                                sizeof(Vps_HdmiInfoFrameCfg));
            }
        }
        /* Get the supplied callback */
        if (config->cbOnHPD != NULL)
        {
            cookie = disableInterrupts
            instPtr->config.cbArgs  =   config->cbArgs;
            instPtr->config.cbOnHPD =   config->cbOnHPD;
            enableInterrupts
        }
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "checkAndCopyCfg<<<<");

    return(rtnValue);
}

/**
 *  waitForTCLKtoStabalize
 *  \brief  This functions checks if pixel repeatation is required or not.
 *          Returns HDMI_PIXEL_REPLECATED_ONCE is requires to be repeated once
 *          or HDMI_PIXEL_REPLECATED_FOUR_TIMES if required to replicate four
 *          times OR suiteable error code.
 */
static int waitForTCLKtoStabalize( HdmiObject_t    *instHandle)
{
    int           rtnValue    =   VPS_SOK;
    volatile u32 timeout     =   VPS_HAL_HDMI_PHY_2_WP_PLL_LOCK_TIMEOUT;

    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>waitForTCLKtoStabalize");
    HDMI_GT_assert(hdmiTrace, (instHandle != NULL));
#if 0 /* On Simulator HDMI is not completely functional */
    while(TRUE)
    {
        while ((instHandle->wpRegOvrLay->IRQSTATUS_RAW &
                CSL_HDMI_WP_IRQSTATUS_PLL_LOCK_PEND_MASK) !=
                CSL_HDMI_WP_IRQSTATUS_PLL_LOCK_PEND_MASK)
        {
            timeout--;
            if (timeout == 0x0)
            {
                break;
            }
        }
        if ((instHandle->wpRegOvrLay->IRQSTATUS_RAW &
             CSL_HDMI_WP_IRQSTATUS_PLL_LOCK_PEND_MASK) !=
             CSL_HDMI_WP_IRQSTATUS_PLL_LOCK_PEND_MASK)
        {
            HDMI_GT_0trace(hdmiTrace,HDMI_GT_ERR,"PHY did not provide stabel TCLK to WP");
            rtnValue    =   VPS_ETIMEOUT;
            break;
        }
        break;
    }
#endif /* PLATFORM_EVM_SI */
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "waitForTCLKtoStabalize<<<<");
    return (rtnValue);
}


/**
 *  determinePixelRepeatation
 *  \brief  This functions checks if pixel repeatation is required or not.
 *          Returns HDMI_PIXEL_REPLECATED_ONCE is requires to be repeated once
 *          or HDMI_PIXEL_REPLECATED_FOUR_TIMES if required to replicate four
 *          times OR suiteable error code.
 */
static int determinePixelRepeatation( HdmiObject_t    *instHandle)
{
    int           rtnValue        =   VPS_SOK;
    u32          mPixelPerSec    =   0x0;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>determinePixelRepeatation");
    HDMI_GT_assert(hdmiTrace, (instHandle != NULL));
    while(TRUE)
    {
        /* Notes
         * Pixel rate should be > 25 Mpixels/Sec
         * Pixel rate = Actual no of coloums * actual no of rows * frames / sec
         * In case of custom config, use the pixel clock directly.
         */
        switch(instHandle->config.displayMode)
        {
            case VPS_HDMI_MODE_NTSC:
            {
                mPixelPerSec = HDMI_VIDEO_STAND_NTSC;
                instHandle->isInterlaced = TRUE;
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "NTSC Display Standard");
                break;
            }
            case VPS_HDMI_MODE_PAL:
            {
                mPixelPerSec = HDMI_VIDEO_STAND_PAL;
                instHandle->isInterlaced = TRUE;
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "PAL Display Standard");
                break;
            }
            case VPS_HDMI_MODE_720P_60:
            {
                mPixelPerSec = HDMI_VIDEO_STAND_720P60;
                instHandle->isInterlaced = FALSE;
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "720 60 P Display format");
                break;
            }
            case VPS_HDMI_MODE_1080P_60:
            {
                mPixelPerSec = HDMI_VIDEO_STAND_1080P60;
                instHandle->isInterlaced = FALSE;
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "1080 60 P Display format");
                break;
            }
            case VPS_HDMI_MODE_1080P_30:
            {
                mPixelPerSec = HDMI_VIDEO_STAND_1080P30;
                instHandle->isInterlaced = FALSE;
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "1080 30 P Display format");
                break;
            }
            case VPS_HDMI_MODE_1080I_60:
            {
                mPixelPerSec = HDMI_VIDEO_STAND_1080I60;
                instHandle->isInterlaced = FALSE;
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "720 60 I Display format");
                break;
            }
            default:
            {
                /* This should not happen */
                rtnValue = VPS_EFAIL;
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "The display format is not ");
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, " supported by HDMI");
                break;
            }
        }
        if (rtnValue == VPS_SOK)
        {
            if (mPixelPerSec < HDMI_MINIMUM_PIXELS_SEC)
            {
                /* Pixels requires to be replicated */
                /* Figure out how many times it requires to be replicated */
                if ((mPixelPerSec * HDMI_PIXEL_REPLECATED_ONCE) <
                    HDMI_MINIMUM_PIXELS_SEC)
                {
                    if ((mPixelPerSec * HDMI_PIXEL_REPLECATED_FOUR_TIMES) <
                        HDMI_MINIMUM_PIXELS_SEC)
                    {
                        /* We could not still meet the HDMI needs - let the
                           caller know */
                        rtnValue = VPS_EOUT_OF_RANGE;
                        HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Resolution too low");
                        HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not reach 25 M");
                        break;
                    }
                    else
                    {
                        /* Repeat four times */
                        rtnValue = HDMI_PIXEL_REPLECATED_FOUR_TIMES;
                        HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "Pixel Repeating 4 time");
                        break;
                    }
                }
                else
                {
                    rtnValue = HDMI_PIXEL_REPLECATED_ONCE;
                    HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "Pixel Repeating 1 time");
                    break;
                }
            }
            /* Otherwise the default is no repeatation - the bits are cleared
               when entered - no action required here */
            /* TBD delete this check - required to catch modes that VENC could
               support but not HDMI */
            #if (VPS_HDMI_MAX_MODE > 0x05)
            #error ("Unsupported mode in HDMI - update me please");
            #endif
        }
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, ">>>>determinePixelRepeatation");
    return(rtnValue);
}

/* ========================================================================== */
/*                            Global Functions                                */
/* ========================================================================== */
/**
 *  VpsHal_hdmiInit
 *  \brief  Initializes HDMI HAL, copies the default configurations, acquires
 *          semaphores, etc...
 */
int VpsHal_hdmiInit(Vps_HdmiInitParams *initParams)
{
    int               rtnValue    =   VPS_SOK;
/*    Semaphore_Params    semParams;*/
    u32              instIndex   =   0x0;

    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>VpsHal_hdmiInit");
    while (TRUE)
    {
        if (HdmiHalInitState != HDMI_HAL_INST_UN_INITIALIZED)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Initialized earlier");
            rtnValue    =   VPS_EDEVICE_INUSE;
            break;
        }
        if (initParams == NULL)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "NULL init params");
            rtnValue = VPS_EBADARGS;
            break;
        }
        /*
         * Considering single instance
         * 1. Create semaphores
         * 2. Init other members
         * 3. Copy the default config
         */
        while(instIndex < HDMI_PER_CNT)
        {
/*            Semaphore_Params_init(&semParams);
            HdmiObjectsPool[instIndex].guard  =
                                        Semaphore_create(1u, &semParams, NULL);
            if (HdmiObjectsPool[instIndex].guard == NULL)
            {
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not acquire semaphore");
                rtnValue    =   VPS_EALLOC;
                break;
            }
            Semaphore_Params_init(&semParams);
            HdmiObjectsPool[instIndex].configGuard  =
                                        Semaphore_create(1u, &semParams, NULL);
            if (HdmiObjectsPool[instIndex].configGuard == NULL)
            {
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not acquire config guard");
                rtnValue    =   VPS_EALLOC;
                break;
            }*/
            HdmiObjectsPool[instIndex].isReceiverSensed = FALSE;
            HdmiObjectsPool[instIndex].isStreaming      = FALSE;
            HdmiObjectsPool[instIndex].isSclClocked     = FALSE;
            HdmiObjectsPool[instIndex].isVSyncCountTracked
                                                        = FALSE;
            HdmiObjectsPool[instIndex].vSyncCounter     = 0x0;
            HdmiObjectsPool[instIndex].isInterlaced     = FALSE;

            HdmiObjectsPool[instIndex].coreRegOvrLay    = (CSL_Hdmi_CoreRegs *)
                                                    initParams->coreBaseAddr;
            HdmiObjectsPool[instIndex].wpRegOvrLay      = (CSL_Hdmi_WpRegs *)
                                                    initParams->wpBaseAddr;
            HdmiObjectsPool[instIndex].phyRegOvrLay     = (CSL_Hdmi_PhyRegs *)
                                                    initParams->phyBaseAddr;
            HdmiObjectsPool[instIndex].pllRegOvrLay     =
                        (CSL_Hdmi_PllCtrlRegs *) initParams->pllCtrlBaseAddr;

            /* Multinstance support - requires to be updated */
            HdmiObjectsPool[instIndex].interruptNo = initParams->interruptNo;

            HdmiObjectsPool[instIndex].pairedVencId     = initParams->vencId;
            HdmiObjectsPool[instIndex].encInstEd        = initParams->encoderId;

            VpsUtils_memcpy(((HDMI_Ptr) &(HdmiObjectsPool[instIndex].config)),
                            ((HDMI_Ptr) &defaultConfig),
                            sizeof(Vps_HdmiConfigParams));
            HdmiObjectsPool[instIndex].state = HDMI_HAL_INST_INITIALIZED;
            instIndex++;
        }
        HdmiHalInitState = HDMI_HAL_INST_INITIALIZED;
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "VpsHal_hdmiInit<<<<");
    return(rtnValue);
}


/**
 *  VpsHal_hdmiDeInit
 *  \brief  De Initializes HDMI HAL, releases acquired resources during
 *          initialization
 */
int VpsHal_hdmiDeInit(HDMI_Ptr args)
{
    u32  index       =   0x0u;
    int   rtnValue    =   VPS_SOK;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>VpsHal_hdmiDeInit");
    HDMI_GT_assert(hdmiTrace, (args == NULL));
    while(TRUE)
    {
        if (HdmiHalInitState != HDMI_HAL_INST_INITIALIZED)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Not yet initialized");
            rtnValue    =   VPS_EBADARGS;
            break;
        }
        /* Check if any instances are opetational If so we cannot de-initialize
           this HAL */
        while (index < HDMI_PER_CNT)
        {
            if ((HdmiObjectsPool[index].state != HDMI_HAL_INST_CLOSED) &&
                (HdmiObjectsPool[index].state != HDMI_HAL_INST_INITIALIZED))
            {
                HDMI_GT_1trace(hdmiTrace, HDMI_GT_ERR, "Instance %d operational", index);
                rtnValue    =   VPS_EALLOC;
                break;
            }
            index++;
        }
        if (rtnValue == VPS_SOK)
        {
            index = 0x0;
            while(index < HDMI_PER_CNT)
            {
/*                Semaphore_delete(&(HdmiObjectsPool[index].guard));
                Semaphore_delete(&(HdmiObjectsPool[index].configGuard));*/
                HdmiObjectsPool[index].state = HDMI_HAL_INST_UN_INITIALIZED;
                index++;
            }
        }
        HdmiHalInitState = HDMI_HAL_INST_UN_INITIALIZED;
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "VpsHal_hdmiDeInit<<<<");
    return (rtnValue);
}

/* Open     - Power up the clock for the DDC and keeps it ON.
 *          - Register the int, update HPD if required
 */

VpsHal_Handle VpsHal_hdmiOpen(  u32                  instance,
                                Vps_HdmiConfigParams    *config,
                                int                   *status,
                                HDMI_Ptr                     args)
{
    HdmiObject_t    *instHandle     =   NULL;
    int           rtnValue        =   VPS_SOK;
/*    u32          resRelease      =   NONE_GUARD;
    Hwi_Params      intParams;*/
    volatile u32 resetTimeOut;
    u32          temp;

    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>VpsHal_hdmiOpen");
    HDMI_GT_assert(hdmiTrace, (args == NULL));
    /*
     * Steps
     * 1. Validate State, get instance lock and validate/copy Configurations
     *    supplied
     * 2. Get the configuration lock - exclusive access to configure
     * 3. Configure PHY and PLL Controller
     * 4. configure clock for DDC - enable DDC clock
     * 5. Reset Core, Wrapper
     * 6. Attach interrupt handler and enable it
     * 7. Configure Wrapper - without enabling it - enable interrupts
     * 8. configure the core - Programe core, determine if pixel repeation
     *                          is required program, enable required interrupts
     *                          and enable core
     * 9.  Program AVI Info frames
     * 9.1 Update the state of HAL
     * 10. Future - enable audio - program DMA also - disable audio for now
     * 11. Future - Configure CEC - disable now
     * 12. Future - Configure HDCP - disabled at this point - disable now
     */
    while(TRUE)
    {
        /* Step 1 - Validate state and configurations */
        if (instance >= HDMI_PER_CNT)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Wrong instance");
            rtnValue    =   VPS_EBADARGS;
            break;
        }
        if ((HdmiObjectsPool[instance].state == HDMI_HAL_INST_OPENED) ||
            (HdmiObjectsPool[instance].state == HDMI_HAL_INST_UN_INITIALIZED))
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Either not initialized");
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, " OR instance already opened");
            rtnValue    =   VPS_EDRIVER_INUSE;
            break;
        }
        instHandle    =   &(HdmiObjectsPool[instance]);
        HDMI_GT_assert(hdmiTrace, (instHandle->coreRegOvrLay != NULL));
        HDMI_GT_assert(hdmiTrace, (instHandle->wpRegOvrLay != NULL));

        /* At this point no interrupts - protection for multi-tasking */
/*        Semaphore_pend(instHandle->guard, BIOS_WAIT_FOREVER);*/
        /* If configuration supplied is valid copy the same */
        if (config != NULL)
        {
            rtnValue = checkAndCopyCfg(&(HdmiObjectsPool[instance]),config);
            if (rtnValue != VPS_SOK)
            {
		printk("\n check and copy config fails ");
/*                resRelease  =   INST_GUARD; */
                break;
            }
        }
#if ((hdmiTrace & (HDMI_GT_DEBUG | GT_TraceState_Enable)) ==   \
                  (HDMI_GT_DEBUG | GT_TraceState_Enable))
        initializeDebugInfo(instHandle);
#endif

        /* Step 2 - Get exclusive access to configure */
/*        Semaphore_pend(instHandle->configGuard, BIOS_WAIT_FOREVER);*/


#ifdef PLATFORM_EVM_SI /* On Simulator HDMI is not completely functional */
        /*
         * Step 5    Reset Wrapper and core
         *           Reset the wrapper first
         */
        resetTimeOut = VPS_HAL_HDMI_WP_RESET_TIMEOUT;
        /* Not inverting TCLK phase
        instHandle->coreRegOvrLay->DPD &= (0xFFFFFFF7);*/

        instHandle->wpRegOvrLay->SYSCONFIG |=
                                           CSL_HDMI_WP_SYSCONFIG_SOFTRESET_MASK;
        /* Wait until soft reset sequence completes */
        while ((instHandle->wpRegOvrLay->SYSCONFIG &
                CSL_HDMI_WP_SYSCONFIG_SOFTRESET_MASK) ==
               CSL_HDMI_WP_SYSCONFIG_SOFTRESET_MASK)
        {
            resetTimeOut--;
            if (resetTimeOut == 0x0)
            {
		printk("\n Could not reset wrapper");
                break;
            }
        }
        if ((instHandle->wpRegOvrLay->SYSCONFIG &
             CSL_HDMI_WP_SYSCONFIG_SOFTRESET_MASK) ==
               CSL_HDMI_WP_SYSCONFIG_SOFTRESET_MASK)
        {
            /* Could not get out of reset */
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not reset wrapper ");
/*            resRelease  =   BOTH_GUARDS;*/
            rtnValue    =   VPS_EFAIL;
            break;
        }
#endif /* PLATFORM_EVM_SI */


        /* Step 3 - Configure the PLL Controller */
        rtnValue = configurePll(instHandle);
        if (rtnValue != VPS_SOK)
        {
/*            resRelease  =   BOTH_GUARDS;*/
            break;
        }
        /* Step 3 - Configure PHY */
        rtnValue = configurePhy(instHandle);
        if (rtnValue != VPS_SOK)
        {
/*            resRelease  =   BOTH_GUARDS;*/
            break;
        }
        /*
         * Wait for the TCLK to stabilize - this clock would be required for
         * configuring wrapper/core
         */
        rtnValue = waitForTCLKtoStabalize(instHandle);
        if (rtnValue != VPS_SOK)
        {
/*            resRelease  =   BOTH_GUARDS;*/
            break;
        }
        /*
         * Step 4 - Get the clock for DDC Master going.
		 *			Configure TCLK Phase
         *          IDCK normal operation
         *          Internal oscilator normal operation
         *          Power down - normal operation
         */
/*        instHandle->coreRegOvrLay->DPD |= ( CSL_HDMI_DPD_TCLKPHZ_MASK	|
											CSL_HDMI_DPD_PDIDCK_MASK    |
                                            CSL_HDMI_DPD_PDOSC_MASK     |
                                            CSL_HDMI_DPD_PDTOT_MASK);
        instHandle->coreRegOvrLay->DPD |= ( CSL_HDMI_DPD_PDIDCK_MASK    |
                                            CSL_HDMI_DPD_PDOSC_MASK     |
                                            CSL_HDMI_DPD_PDTOT_MASK);*/
        /*
         * After configuring the DDC clock - we would require some delay before
         * we could use DDC - Assumption is that the dealy in executing this
         * function and application using DDC (EDID Read) should be long enough
         */
        /* Ensure to set the by-pass allfunctions of internal video path
        instHandle->coreRegOvrLay->XVYCC2RGB_CTL   |= 0x8;*/

#if INCLUDE_BIOS_INT_HANDLER
        /* Step 6 - Attach interrupt handlers and enable the interrupt */
        Hwi_Params_init(&intParams);
        intParams.arg       = (u32)instHandle;
        intParams.enableInt = TRUE;
        instHandle->hwiHandle =   Hwi_create(   CSL_INTC_EVENTID_HDMI,
                                                hdmiMainIsr,
                                                &intParams, NULL);
        if (instHandle->hwiHandle == NULL)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not attach interrupts handler");
/*            resRelease  =   BOTH_GUARDS;*/
            rtnValue    =   VPS_EFAIL;
            break;
        }
#endif

        /*
         * Step 7 - Configure wrapper - without enabling it.
         *          Interlaced / Progressive should be specified to wrapper.
         *          While configuring the core we determine the scan type.
         *          After configuring the core, the wrapper is updated with the
         *          scan type of the in-coming video stream
         */
        rtnValue    =   configureWrapper(instHandle);
        if (rtnValue != VPS_SOK)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not configure wrapper");
/*            resRelease  =   BOTH_GUARDS;*/
            rtnValue    =   VPS_EFAIL;
            break;
        }

        /* Step 7/9 - Disable audio - Would be programmed / contrled by other
                      entity */
        instHandle->wpRegOvrLay->AUDIO_CTRL &=
                                        (~(CSL_HDMI_WP_AUDIO_CTRL_DISABLE_MASK));
        instHandle->wpRegOvrLay->AUDIO_CFG   =   0x0;

        /*
         * Check If HDMI sink is attached and no callback was generated,
         * Could happen if the sink was attached before enbling of the
         * interrupts in above function.
         * if so update the status to say sink dected and call the callback.
         * Applications could READ out edid after open completes
         */
        if (instHandle->isReceiverSensed == FALSE)
        {
            /*
             * Looking at RAW Status, as interrupts are disabled at this point
             * When sink was dected by hardware, RAW status would be set
             * but NOT the IRQSTATUS
             *
             * \par CAUTION : Using the bit feilds of the IRQSTATUS reg,
             *                as the bit positions are same between RAW and
             *                STATUS.
             *          Interrupts requires to be disabled, as at this point we
             *          do not want two entities to modify the HPD status.
             *          If the HPD int happens just before no harm done.
             *          If HPD int happend right after this - no issues
             *          If sink detach happens after this point - no issues
             *          If sink detach happens before this point - RAW / int
             *              would take care of this.
             */
			if ((instHandle->wpRegOvrLay->IRQSTATUS_RAW & CSL_HDMI_WP_IRQSTATUS_CORE_INTR_PEND_MASK) ==
				CSL_HDMI_WP_IRQSTATUS_CORE_INTR_PEND_MASK)
			{
				/* Right now, we are handling HPD and RSEN interrupts from the core
				   when required to handle more interrupts update this section */
				if ((instHandle->coreRegOvrLay->INTR1 & CSL_HDMI_INTR1_HPD_MASK) ==
						CSL_HDMI_INTR1_HPD_MASK)
				{
					if (instHandle->isReceiverSensed != TRUE)
					{
						/* Why was this not sensed by wrappers interrupt ? */
						instHandle->isReceiverSensed = TRUE;
						//printk("Detected the TV\n");
						HDMI_GT_0trace(hdmiTrace, HDMI_GT_DEBUG, "Dected a sink");
					}
				}
				else
				{
					/* Check for un-plug */
					if (instHandle->isReceiverSensed == TRUE)
					{
						/* Why was this not sensed by wrappers interrupt ? */
						instHandle->isReceiverSensed = FALSE;
						//printk("TV Removed \n");
						HDMI_GT_0trace(hdmiTrace, HDMI_GT_DEBUG, "Attached sink is removed");
					}
				}
			}
			if ((instHandle->isReceiverSensed == TRUE) &&
				(instHandle->config.cbOnHPD != NULL))
			{
				instHandle->config.cbOnHPD( TRUE,
											instHandle->config.cbArgs);
			}
        }
#ifdef IGNORE_TV
		else
		{
			instHandle->isReceiverSensed = TRUE;
		}
#endif


        /* Reset & power down core */
        instHandle->coreRegOvrLay->SRST       |=  CSL_HDMI_SRST_SWRST_MASK;
        instHandle->coreRegOvrLay->SYS_CTRL1  |=  CSL_HDMI_SYS_CTRL1_PD_MASK;


        /* Step 8 - Configure core */
        rtnValue = configureCore(instHandle);
        if (rtnValue != VPS_SOK)
        {
/*            resRelease  =   BOTH_GUARDS;*/
            break;
        }
#ifdef USE_SILICON_RECOM_2306
        /* Ensure to disable the audio */
        instHandle->coreRegOvrLay->AUD_MODE =   0x0;
#endif
        /* Power up core and bring it out of reset. */
        instHandle->coreRegOvrLay->SRST       &=  (~(CSL_HDMI_SRST_SWRST_MASK));

        /*
         * Core updates function would have updated the global member to specify
         * the scan type update the wrapper with same info
         */
        if (instHandle->isInterlaced == TRUE)
        {
            instHandle->wpRegOvrLay->VIDEO_CFG |=
                            CSL_HDMI_WP_VIDEO_CFG_PROGRESSIVE_INTERLACE_MASK;
        }
        else
        {
            instHandle->wpRegOvrLay->VIDEO_CFG &=
                        (~(CSL_HDMI_WP_VIDEO_CFG_PROGRESSIVE_INTERLACE_MASK));
        }

        /* Step 9 - Configure AVI Info frame and enable them to be transmitted
                    every frame */
        rtnValue = configureAVIInfoFrame(instHandle, FALSE);
        if (rtnValue != VPS_SOK)
        {
/*            resRelease  =   BOTH_GUARDS;*/
            break;
        }

        rtnValue = configureControlPacket(instHandle);
        if (rtnValue != VPS_SOK)
        {
/*            resRelease  =   BOTH_GUARDS;*/
            break;
        }

        /*
         * Policy
         * 1. Enabling continious transmission of AVI Information packets
         */
        /*
         * Note that enbling transmission of AVI info packet and repeat mode
         * should be simultaneous
         *
         * Not trying to stop, as the reset of IP should have stopped
         * transmission of packets. In close its ensured that device is reset.
         */

        instHandle->coreRegOvrLay->PB_CTRL1 =
                                        (CSL_HDMI_PB_CTRL1_AVI_EN_MASK |
                                        CSL_HDMI_PB_CTRL1_AVI_RPT_MASK);

        temp = ((instHandle->coreRegOvrLay->VID_MODE &
                            CSL_HDMI_VID_MODE_DITHER_MODE_MASK ) >>
                            CSL_HDMI_VID_MODE_DITHER_MODE_SHIFT);
        /* General control packets are required only when in deep color mode,
           as packing phase would require to be indicated, else bypass this */
        if (temp != HDMI_VID_MODE_DITHER_TO_24_BITS_MODE)
        {
            instHandle->coreRegOvrLay->PB_CTRL2 =
                                            (CSL_HDMI_PB_CTRL2_GEN_EN_MASK  |
                                             CSL_HDMI_PB_CTRL2_GEN_RPT_MASK);
        }
        else
        {
            instHandle->coreRegOvrLay->PB_CTRL2 =
                                            (CSL_HDMI_PB_CTRL2_CP_EN_MASK   |
                                             CSL_HDMI_PB_CTRL2_CP_RPT_MASK  |
                                             CSL_HDMI_PB_CTRL2_GEN_EN_MASK  |
                                             CSL_HDMI_PB_CTRL2_GEN_RPT_MASK);
        }

        HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "Pulled core out of reset");

        /* 9.1 */
        instHandle->state = HDMI_HAL_INST_OPENED;

/*        resRelease  =   BOTH_GUARDS;*/
        break;
    }

/*    if ((resRelease & BOTH_GUARDS) == BOTH_GUARDS)
    {
        Semaphore_post(instHandle->configGuard);
        Semaphore_post(instHandle->guard);
    }
    if ((resRelease & INST_GUARD) == INST_GUARD)
    {
        Semaphore_post(instHandle->guard);
    }*/

    /* Let the caller know the reason for failure - if any */
    if (status != NULL)
    {
        *status =   rtnValue;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "VpsHal_hdmiOpen<<<<");
    return ((VpsHal_Handle) instHandle);
}


/**
 *  VpsHal_hdmiClose
 *  \brief  Closes the instance handle and would reset the core.
 *          Wrapper should have been stopped/disabled.
 */
int VpsHal_hdmiClose( VpsHal_Handle handle, HDMI_Ptr args)
{
    int           rtnValue    =   VPS_SOK;
    HdmiObject_t    *hdmiObj    =   NULL;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>VpsHal_hdmiClose");
    HDMI_GT_assert(hdmiTrace, (args == NULL));
    while(TRUE)
    {
        /*
         * 1. Validate arguments, Check if opened
         * 2. Ensure streaming is stopped
         * 3. Power down and reset the core
         * 4. Un-Register interrupts and dis-own the instance
         */
        /* Step 1 */
        if (handle == NULL)
        {
            rtnValue = VPS_EBADARGS;
            break;
        }
        hdmiObj = (HdmiObject_t *)handle;
        /* Step 2 */
        if (hdmiObj->state != HDMI_HAL_INST_OPENED)
        {
            rtnValue = VPS_EUNSUPPORTED_OPS;
            break;
        }
        /* Step 3 Check if for no active streams */
        if (hdmiObj->isStreaming == FALSE)
        {
            /* Reset the core - the wrapper would have been reset when stopped
             * streaming
             */
            hdmiObj->coreRegOvrLay->SRST       |=  CSL_HDMI_SRST_SWRST_MASK;
            hdmiObj->coreRegOvrLay->SYS_CTRL1  &=
                                            (~(CSL_HDMI_SYS_CTRL1_PD_MASK));
#if INCLUDE_BIOS_INT_HANDLER
            /* We do not want an interrupt while disabling the same */
            Hwi_disableInterrupt(CSL_INTC_EVENTID_HDMI);
            Hwi_delete(&(hdmiObj->hwiHandle));
#endif
            hdmiObj->state = HDMI_HAL_INST_CLOSED;
        }
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "VpsHal_hdmiClose<<<<");
    return(rtnValue);
}


/**
 *  VpsHal_hdmiGetMode
 *  \brief  Get the current mode of the HDMI instance
 */
int VpsHal_hdmiGetMode(VpsHal_Handle handle, Vps_HdmiModeId *modeInfo)
{
    int           rtnValue    =   VPS_SOK;
    HdmiObject_t    *hdmiObj    =   NULL;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>VpsHal_hdmiGetMode");
    while(TRUE)
    {
        /*
         * 1. Validate arguments
         * 2. Check if are opened and obtain exclusive access
         * 3. Copy the mode information
         */
        /* Step 1 */
        if ((handle == NULL) || (modeInfo == NULL))
        {
            rtnValue = VPS_EBADARGS;
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Invalid handle/modeInfo pointer");
            break;
        }
        hdmiObj = (HdmiObject_t *)handle;
        /* Step 2 */
        if (hdmiObj->state != HDMI_HAL_INST_OPENED)
        {
            rtnValue = VPS_EUNSUPPORTED_OPS;
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Instance not yet opened");
            break;
        }
/*        Semaphore_pend(hdmiObj->guard, BIOS_WAIT_FOREVER);*/
        /* Copy the configurations */
        *modeInfo   = hdmiObj->config.displayMode;
/*        Semaphore_post(hdmiObj->guard);*/
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "VpsHal_hdmiGetMode<<<<");
    return (rtnValue);
}


/**
 *  VpsHal_hdmiSetMode
 *  \brief  Update the current mode to new mode of operation.
 *          mode configuration is used to computations of pixel repeatation and
 *          pixel repeatation in AVI info frames
 */
int VpsHal_hdmiSetMode(VpsHal_Handle handle, Vps_HdmiModeId *modeInfo)
{
    int           rtnValue    =   VPS_SOK;
    HdmiObject_t    *hdmiObj    =   NULL;
    static Vps_HdmiConfigParams  currentConfig;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>VpsHal_hdmiSetMode");
    while(TRUE)
    {
        /*
         * 1. Validate arguments
         * 2. Check if are opened and obtain exclusive access
         * 3. Copy the current HDMI config into local config struct
         * 4. Update the local config with user supplied mode info
         * 5. Call the set config API
         */
        /* Step 1 */
        if ((handle == NULL) || (modeInfo == NULL))
        {
            rtnValue = VPS_EBADARGS;
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Invalid handle/modeInfo pointer");
            break;
        }
        hdmiObj = (HdmiObject_t *)handle;
        /* Step 2 */
        if (hdmiObj->state != HDMI_HAL_INST_OPENED)
        {
            rtnValue = VPS_EUNSUPPORTED_OPS;
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Instance not yet opened");
            break;
        }
        /* Custom mode not supported for now TBD */
        if (*modeInfo >= VPS_HDMI_MAX_MODE)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, " Incorrect mode id");
            rtnValue    =   VPS_EUNSUPPORTED_CMD;
            break;
        }
        /* Copy the complete current configurations */
        VpsUtils_memcpy((HDMI_Ptr)(&currentConfig),
                            ((const void *)&(hdmiObj->config)),
                            sizeof(Vps_HdmiConfigParams));
        /* Overwirte with updated mode info and apply the same */
        currentConfig.displayMode        = *modeInfo;
        /* Turn off all update flags */
        currentConfig.useWpConfig           =   0x0;
        currentConfig.useCoreIpConfig       =   0x0;
        currentConfig.useInfoFrameConfig    =   0x0;
        /* We let the path config as TRUE - mode info is used while
           updating the path config */

        rtnValue =  VpsHal_hdmiSetConfig( hdmiObj, &currentConfig, NULL);
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "VpsHal_hdmiSetMode<<<<");
    return (rtnValue);
}


/**
 *  VpsHal_hdmiGetConfig
 *  \brief  Get the current configuration of the HDMI instance
 */
static int VpsHal_hdmiGetConfig(  VpsHal_Handle handle,
                                    Vps_HdmiConfigParams *config,
                                    HDMI_Ptr     args)
{
    int           rtnValue    =   VPS_SOK;
    HdmiObject_t    *hdmiObj    =   NULL;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>VpsHal_hdmiGetConfig");
    HDMI_GT_assert(hdmiTrace, (args == NULL));
    while(TRUE)
    {
        /*
         * 1. Validate arguments
         * 2. Check if are opened and obtain exclusive access
         * 3. Copy the config and set the update flags to FALSE
         */
        /* Step 1 */
        if ((handle == NULL) || (config == NULL))
        {
            rtnValue = VPS_EBADARGS;
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Invalid handle/config pointer");
            break;
        }
        hdmiObj = (HdmiObject_t *)handle;
        /* Step 2 */
        if (hdmiObj->state != HDMI_HAL_INST_OPENED)
        {
            rtnValue = VPS_EUNSUPPORTED_OPS;
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Instance not yet opened");
            break;
        }
        /* Copy the configurations */
        VpsUtils_memcpy((HDMI_Ptr)config,
                            ((const void *)&(hdmiObj->config)),
                            sizeof(Vps_HdmiConfigParams));
        /* Turn OFF the config update flags */
        config->useDcDisplayMode    = 0x0;
        config->useWpConfig         = 0x0;
        config->useCoreIpConfig     = 0x0;
        config->useCorePathConfig   = 0x0;
        config->useInfoFrameConfig  = 0x0;
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "VpsHal_hdmiGetConfig<<<<");
    return (rtnValue);
}


/**
 *  VpsHal_hdmiAttachHpdCb
 *  \brief  Attach a user supplied function that would be called when an sink
 *          is dected. If an sink is already attached, call the callback ASAP
 */
static int VpsHal_hdmiAttachHpdCb(    VpsHal_Handle handle,
                                        Vps_Hdmi_CbFxn cbFxn,
                                        HDMI_Ptr cbArgs,
                                        HDMI_Ptr args)
{
    int           rtnValue    =   VPS_SOK;
    HdmiObject_t    *hdmiObj    =   NULL;
    u32          cookie;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>VpsHal_hdmiAttachHpdCb");
    HDMI_GT_assert(hdmiTrace, (args == NULL));
    while(TRUE)
    {
        /*
         * 1. Validate arguments
         * 2. Check if are opened and obtain exclusive access
         * 3. Copy the config and set the update flags to FALSE
         * 4. Check if an SINK is already attached, if so call the CB now.
         *    No need to check the physical regs for the status, check the flag
         *    if not set, no sink attached.
         */
        /* Step 1 */
        if ((handle == NULL) || (cbFxn == NULL))
        {
            rtnValue = VPS_EBADARGS;
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Invalid handle/cb func pointer");
            break;
        }
        hdmiObj = (HdmiObject_t *)handle;
        /* Step 2 */
        if (hdmiObj->state != HDMI_HAL_INST_OPENED)
        {
            rtnValue = VPS_EUNSUPPORTED_OPS;
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Instance not yet opened");
            break;
        }
/*        Semaphore_pend(hdmiObj->guard, BIOS_WAIT_FOREVER);*/
        /* Requires interrupts to be disabled - as we do not want ISR to look
           at cbOnHPD while we are updating it */
        cookie = disableInterrupts
        hdmiObj->config.cbOnHPD =   cbFxn;
        hdmiObj->config.cbArgs  =   cbArgs;

        /* Require to be under interrupt protection. Cause, at this point, if
           sink was detached, there could be 2 callbacks. It could result in
           nesting of user supplied cb.
           i.e. task->VpsHal_hdmiAttachHpdCb->Cb - while in progress
                detach happens. Cb would be called int contect, while Cb
                execution was in progress */
        if (hdmiObj->isReceiverSensed == TRUE)
        {
            /* cbFxn is already checked for NULL - no need to check again */
            cbFxn(  TRUE,
                    hdmiObj->config.cbArgs);
        }
        enableInterrupts
/*        Semaphore_post(hdmiObj->guard);*/

        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "VpsHal_hdmiAttachHpdCb<<<<");
    return (rtnValue);
}


/**
 *  VpsHal_hdmiSetConfig
 *  \brief  Apply the configurations specified and enabled. Should not be
 *          streaming at this point.
 *  \para Note We would not require to disable the interrupts, as we are only
 *        tracking the presence of SINK in the interrupts for now.
 *        While configuring/enalbing, the interrupt should not cause anu issues
 */
static int VpsHal_hdmiSetConfig(  VpsHal_Handle handle,
                                    Vps_HdmiConfigParams *config,
                                    HDMI_Ptr     args)
{
    HdmiObject_t    *instHandle     =   NULL;
    int           rtnValue        =   VPS_SOK;
/*    u32          resRelease      =   NONE_GUARD;*/
    u32          reConfPll       =   FALSE;
    u32          cookie;

    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>VpsHal_hdmiSetConfig");
    HDMI_GT_assert(hdmiTrace, (args == NULL));
    /*
     * Steps
     * 1. Validate State, get instance lock and validate/copy Configurations
     *    supplied
     * 1.1 Re-Cofigure PLL Ctrl if required
     * 1.1 Re-Configure PHY - always
     * 2. If required to configure Wrapper - configure without enabling it
     * 3. If core input or data path requires to be configured,
     *      Reset the core and reconfigure the core and enable it.
     *    else
     *       Since PHY has been re-configured. core for pixel repeatation
     * 4. Update the wrapper for scan type.
     * 5. If required re-program AVI Info frames
     * 6. Reconfigure PLL, Pixel repeatation would require higher clocks
     *
     */
    while(TRUE)
    {
        /* Step 1 - Validate state and configurations */
        instHandle    =   (HdmiObject_t *)handle;
        HDMI_GT_assert(hdmiTrace, (instHandle->coreRegOvrLay != NULL));
        HDMI_GT_assert(hdmiTrace, (instHandle->wpRegOvrLay != NULL));
        if (instHandle->state != HDMI_HAL_INST_OPENED)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Not yet opened");
            rtnValue    =   VPS_EDRIVER_INUSE;
            break;
        }
        if (instHandle->isStreaming == TRUE)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Streaming - cannot re-configure");
            rtnValue    =   VPS_EUNSUPPORTED_CMD;
            break;
        }
        if (config == NULL)
        {
            rtnValue = VPS_EBADARGS;
            break;
        }
        /* Protection for multi-tasking */
/*        Semaphore_pend(instHandle->guard, BIOS_WAIT_FOREVER);*/
        if (config->useDcDisplayMode == TRUE)
        {
            if (config->displayMode != instHandle->config.displayMode)
            {
                /* Basic resolution has changed - reconfigure PLL */
                reConfPll   =   TRUE;
            }
        }
        /* If configuration supplied is valid copy the same */
        rtnValue    =   checkAndCopyCfg(instHandle, config);
        if (rtnValue != VPS_SOK)
        {
/*            resRelease  =   INST_GUARD;*/
            break;
        }
        /* Get exclusive access to configure */
/*        Semaphore_pend(instHandle->configGuard, BIOS_WAIT_FOREVER);*/
        /* Step 1.1 - Reconfigure PLL Controller if required */
        if (reConfPll == TRUE)
        {
            rtnValue = configurePll(instHandle);
            if (rtnValue != VPS_SOK)
            {
/*                resRelease  =   BOTH_GUARDS;*/
                break;
            }
        }
        /* Step 1.1 - ReConfigure PHY */
        rtnValue = configurePhy(instHandle);
        if (rtnValue != VPS_SOK)
        {
/*            resRelease  =   BOTH_GUARDS;*/
            break;
        }
        /*
         * Wait for the TCLK to stabilize - this clock would be required for
         * configuring wrapper/core
         */
        rtnValue = waitForTCLKtoStabalize(instHandle);
        if (rtnValue != VPS_SOK)
        {
/*            resRelease  =   BOTH_GUARDS;*/
            break;
        }

        /* Step 2 - If required, configure wrapper - without enabling it */
        if (config->useWpConfig != 0x0)
        {
            rtnValue    =   configureWrapper(instHandle);
            if (rtnValue != VPS_SOK)
            {
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not configure wrapper");
/*                resRelease  =   BOTH_GUARDS;*/
                rtnValue    =   VPS_EFAIL;
                break;
            }
        }

        /* Step 3 - Reset the core, if no configuration has changed, the PHY
                    pixel repeatation could have changed.
                    If any of the core configurations have changed,
                    re-program the core */
        /* Reset core */
        instHandle->coreRegOvrLay->SRST       |=  CSL_HDMI_SRST_SWRST_MASK;
        instHandle->coreRegOvrLay->SYS_CTRL1  &=
                                            (~(CSL_HDMI_SYS_CTRL1_PD_MASK));

        if ((config->useCoreIpConfig != 0x0) ||
            (config->useCorePathConfig != 0x0))
        {
            /* This function would configure core and brings it out of reset */
            rtnValue = configureCore(instHandle);
            if (rtnValue != VPS_SOK)
            {
/*                resRelease  =   BOTH_GUARDS;*/
                break;
            }
        }
        else
        {
            rtnValue = determinePixelRepeatation(instHandle);
            /* No pixel repetation - by default*/
            instHandle->coreRegOvrLay->VID_CTRL &=
                                            (~(CSL_HDMI_VID_CTRL_ICLK_MASK));
            if (rtnValue == HDMI_PIXEL_REPLECATED_ONCE)
            {
                /* Repeat once */
                instHandle->coreRegOvrLay->VID_CTRL |=
                                (((0x01u) << CSL_HDMI_VID_CTRL_ICLK_SHIFT) &
                                             CSL_HDMI_VID_CTRL_ICLK_MASK);
            }
            else if (rtnValue == HDMI_PIXEL_REPLECATED_FOUR_TIMES)
            {
                instHandle->coreRegOvrLay->VID_CTRL |=
                                                CSL_HDMI_VID_CTRL_ICLK_MASK;
            }
            else if (rtnValue == VPS_SOK)
            {
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "No Pixel repeatation required");
            }
            else
            {
                /* Error let the caller know */
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not determine pixel ");
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, " rate that would be required.");
                break;
            }
            /* Power up core and bring it out of reset. */
            instHandle->coreRegOvrLay->SYS_CTRL1  |= CSL_HDMI_SYS_CTRL1_PD_MASK;
            instHandle->coreRegOvrLay->SRST &= (~(CSL_HDMI_SRST_SWRST_MASK));
        }
        /*
         * Step 4
         * Re-configure the wrapper with the scan type. It might have changed.
         */
        if (instHandle->isInterlaced == TRUE)
        {
            instHandle->wpRegOvrLay->VIDEO_CFG |=
                        CSL_HDMI_WP_VIDEO_CFG_PROGRESSIVE_INTERLACE_MASK;
        }
        else
        {
            instHandle->wpRegOvrLay->VIDEO_CFG &=
                    (~(CSL_HDMI_WP_VIDEO_CFG_PROGRESSIVE_INTERLACE_MASK));
        }

        /* Step 4 - Configure AVI Info frame and enable them to be transmitted
                    every frame */
        if (config->useInfoFrameConfig != 0x0)
        {
            rtnValue = configureAVIInfoFrame(instHandle, FALSE);
            if (rtnValue != VPS_SOK)
            {
/*                resRelease  =   BOTH_GUARDS;*/
                break;
            }
            /*
             * Policy
             * 1. Enabling continious transmission of AVI Information packets
             */
            instHandle->coreRegOvrLay->PB_CTRL1 |=
               (CSL_HDMI_PB_CTRL1_AVI_EN_MASK | CSL_HDMI_PB_CTRL1_AVI_RPT_MASK);
        }

        /* Copy the CB and its arguments - If present */
        if (config->cbOnHPD != NULL)
        {
            cookie = disableInterrupts
            instHandle->config.cbOnHPD  = config->cbOnHPD;
            instHandle->config.cbArgs   = config->cbArgs;
            enableInterrupts
        }
        if (rtnValue != VPS_SOK)
        {
/*            resRelease  =   BOTH_GUARDS;*/
            break;
        }
/*        resRelease  =   BOTH_GUARDS;*/
        break;
    }

/*    if ((resRelease & BOTH_GUARDS) == BOTH_GUARDS)
    {
        Semaphore_post(instHandle->configGuard);
        Semaphore_post(instHandle->guard);
    }
    if ((resRelease & INST_GUARD) == INST_GUARD)
    {
        Semaphore_post(instHandle->guard);
    }*/

    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "VpsHal_hdmiSetConfig<<<<");

    return (rtnValue);
}


/**
 *  VpsHal_hdmiStart
 *  \brief  Starts streaming on HDMI
 */
int VpsHal_hdmiStart( VpsHal_Handle handle, HDMI_Ptr args)
{
    int           rtnValue    =   VPS_SOK;
    HdmiObject_t    *hdmiObj    =   NULL;
    u32          cookie;

    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>VpsHal_hdmiStart");
    HDMI_GT_assert(hdmiTrace, (args == NULL));

    while(TRUE)
    {
        /*
         * 1. Validate arguments
         * 2. Check if are opened
         * 3. Check if a SINK is attached
         * 4. If we are already streaming - return OK
         * 5. Enable the wrapper to stream
         * Note - All control packets should be enabled after the core/packets
         *        are programmed and core is powered up. Enabling CP and AVI
         *        Info before start.
         */
        /* Step 1 */
        if (handle == NULL)
        {
            rtnValue = VPS_EBADARGS;
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Invalid handle/config pointer");
            break;
        }
        hdmiObj = (HdmiObject_t *)handle;
        /* Step 2 */
        if (hdmiObj->state != HDMI_HAL_INST_OPENED)
        {
            rtnValue = VPS_EUNSUPPORTED_OPS;
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Instance not yet opened");
            break;
        }
        /* Step 3 Check if sink is attached */
        if (hdmiObj->isStreaming == FALSE)
        {
#ifdef PLATFORM_EVM_SI /* On Simulator HDMI is not completely functional */
            if (hdmiObj->isReceiverSensed == TRUE)
            {
#endif /* PLATFORM_EVM_SI */
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "Trying to start the port");

#ifdef STARTED_AFTER_CONFIG
                /*
                 * Enable all control packets that requires to be transmitted
                 * 1. Enabling AVI info frames tranmission and repetation
                 * 2. Enabling CP and repetation
                 */
                hdmiObj->coreRegOvrLay->PB_CTRL1 =
                                                (CSL_HDMI_PB_CTRL1_AVI_EN_MASK |
                                                CSL_HDMI_PB_CTRL1_AVI_RPT_MASK);
                hdmiObj->coreRegOvrLay->PB_CTRL2 =
                                                (CSL_HDMI_PB_CTRL2_CP_EN_MASK  |
                                                 CSL_HDMI_PB_CTRL2_CP_RPT_MASK);
#endif

                /* Step 5 Enable the wrapper */
                hdmiObj->wpRegOvrLay->VIDEO_CFG |=
                                            CSL_HDMI_WP_VIDEO_CFG_ENABLE_MASK;
                /* Update the local structure to record the status */
                cookie = disableInterrupts
                hdmiObj->isStreaming = TRUE;
                enableInterrupts
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "Started the port");
#ifdef PLATFORM_EVM_SI /* On Simulator HDMI is not completely functional */
            }
            else
            {
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "No Sinks dected - not starting");
            }
#endif /* PLATFORM_EVM_SI */
        }
        else
        {
            /* Step 4 */
            break;
        }
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "VpsHal_hdmiStart<<<<");
    return(rtnValue);
}


/**
 *  VpsHal_hdmiStop
 *  \brief  Stops streaming on HDMI
 */
int VpsHal_hdmiStop( VpsHal_Handle handle, HDMI_Ptr args)
{
    int           rtnValue    =   VPS_SOK;
    HdmiObject_t    *hdmiObj    =   NULL;
    u32          cookie;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>VpsHal_hdmiStop");
    HDMI_GT_assert(hdmiTrace, (args == NULL));
    while(TRUE)
    {
        /*
         * 1. Validate arguments
         * 2. Check if opened
         * 3. if streaming disable the wrapper
         */
        /* Step 1 */
        if (handle == NULL)
        {
            rtnValue = VPS_EBADARGS;
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Invalid handle/config pointer");
            break;
        }
        hdmiObj = (HdmiObject_t *)handle;
        /* Step 2 */
        if (hdmiObj->state != HDMI_HAL_INST_OPENED)
        {
            rtnValue = VPS_EUNSUPPORTED_OPS;
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Instance not yet opened");
            break;
        }
        /* Step 3 if streaming disable the wrapper */
        if (hdmiObj->isStreaming == TRUE)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "Trying to stop the port");
            hdmiObj->wpRegOvrLay->VIDEO_CFG &=
                                        (~(CSL_HDMI_WP_VIDEO_CFG_ENABLE_MASK));
            /* Update the local structure to record the status */
            cookie = disableInterrupts
            hdmiObj->isStreaming = FALSE;
            enableInterrupts
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_INFO, "Stopped the port");
        }
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "VpsHal_hdmiStop<<<<");
    return(rtnValue);
}


/**
 *  VpsHal_hdmiControl
 *  \brief  Control HDMI Peripheral
 */
int VpsHal_hdmiControl (  VpsHal_Handle  handle,
                            u32         cmd,
                            HDMI_Ptr            cmdArgs,
                            HDMI_Ptr            additionalArgs)
{
    int                   rtnValue    =   VPS_SOK;
    HdmiObject_t            *hdmiObj    =   NULL;

    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>VpsHal_hdmiControl");
    while(TRUE)
    {
        /* Validate the handle and execute the command. */
        if ((handle == NULL) || (cmdArgs == NULL))
        {
            rtnValue = VPS_EBADARGS;
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Invalid handle/cmdArgs pointer");
            break;
        }
        hdmiObj = (HdmiObject_t *)handle;
        if (hdmiObj->state != HDMI_HAL_INST_OPENED)
        {
            rtnValue = VPS_EUNSUPPORTED_OPS;
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Instance not yet opened");
            break;
        }
        switch(cmd)
        {
            case IOCTL_HDMI_START:
            {
                rtnValue = VpsHal_hdmiStart (handle, NULL);
                break;
            }
            case IOCTL_HDMI_STOP:
            {
                rtnValue = VpsHal_hdmiStop (handle, NULL);
                break;
            }
            case IOCTL_HDMI_ATTACH_HPD_CB:
            {
                rtnValue = VpsHal_hdmiAttachHpdCb (
                                       handle,
                                       (Vps_Hdmi_CbFxn) cmdArgs,
                                       additionalArgs,
                                       NULL);
                break;
            }
            case IOCTL_HDMI_GET_STATUS:
            {
                rtnValue = VPS_EBADARGS;
                if (cmdArgs != NULL)
                {
                    (* (u32 *) cmdArgs) = hdmiObj->isStreaming;
                    rtnValue = VPS_SOK;
                }
                break;
            }
            case IOCTL_HDMI_READ_EDID:
            {
#ifndef EDID_USE_LEGACY_READ
                rtnValue = VpsHal_hdmiEdidRead (
                                    handle,
                                    (Vps_HdmiEdidReadParams *)cmdArgs,
                                    NULL);
#else
                rtnValue = VpsHal_hdmiEdidRead_legacy (
                                    handle,
                                    (Vps_HdmiEdidReadParams *)cmdArgs,
                                    NULL);
#endif
                break;
            }
            case IOCTL_HDMI_GET_CONFIG:
            {
                rtnValue = VpsHal_hdmiGetConfig (
                                    handle,
                                    (Vps_HdmiConfigParams *) cmdArgs,
                                    NULL);
                break;
            }
            case IOCTL_HDMI_SET_CONFIG:
            {
                rtnValue = VpsHal_hdmiSetConfig (
                                    handle,
                                    (Vps_HdmiConfigParams *) cmdArgs,
                                    NULL);
                break;
            }
            default :
            {
                rtnValue = VPS_EBADARGS;
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Un-recoganized command");
                break;
            }
        }
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "VpsHal_hdmiControl<<<<");
    return(rtnValue);
}

#ifdef EDID_USE_LEGACY_READ
/**
 *  VpsHal_hdmiEdidRead_legacy
 *  \brief  Read an specified block - legacy as it follows the steps done in
 *          ompa4 edid read
 */
static int VpsHal_hdmiEdidRead_legacy(    VpsHal_Handle           handle,
                                            Vps_HdmiEdidReadParams  *rParams,
                                            HDMI_Ptr                     args)
{
    int           rtnValue    =   VPS_SOK;
    u32          rByteCnt    =   0x0;
    volatile u32 timeOut;
    volatile u32 cmdStatusReg;
    UInt8           *bufPtr     =   NULL;
    HdmiObject_t    *hdmiObj    =   NULL;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>VpsHal_hdmiEdidRead_legacy");
    HDMI_GT_assert(hdmiTrace, (args == NULL));
    /*
     * Steps
     * 1. Validate params - including HPD
     * 2. Check for RI_STARTED - In case of enabled HDCP and Ri in progress
     *    this bit would be set. When HDCP is supported stop the Ri check
     *    and proceed to READ EDID, if requested.
     * 3. Clock SCL, required as we would not know that status of SCL. As this
     *    API could be called by apps at any point. Clear out FIFOS
     * 4. Set up slave address & ofsets, Send the command to perform EDDC read,
     *    wait for cmdinproce to complete
     * 5. Go ahead and read the segment
     */
    while(TRUE)
    {
        /* Step 1 */
        if ((handle == NULL) || (rParams == NULL))
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Invalid params ");
            rtnValue    =   VPS_EBADARGS;
            break;
        }
        hdmiObj =   handle;
        bufPtr  =   (UInt8 *)rParams->bufferPtr;
        if (bufPtr == NULL)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Invalid buffer pointer");
            rtnValue    =   VPS_EBADARGS;
            break;
        }
        /* 10 bits to hold the count - which would be 3FF*/
        if ((rParams->noOfBytes == 0x0) || (rParams->noOfBytes > 0x3FF))
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Invalid byte count");
            rtnValue    =   VPS_EBADARGS;
            break;
        }
        rParams->noOfBytesRead  =   0x0;
        if ((hdmiObj->state != HDMI_HAL_INST_OPENED) ||
            (hdmiObj->isReceiverSensed != TRUE ))
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "HPD not detected - HAL not opened");
            rtnValue    =   VPS_EUNSUPPORTED_OPS;
            break;
        }
        /* Step 2 */
        if ((hdmiObj->coreRegOvrLay->RI_STAT & CSL_HDMI_RI_STAT_RI_STARTED_MASK)
                == CSL_HDMI_RI_STAT_RI_STARTED_MASK)
        {
            /* When HDCP is supported - disable the RI check - wait for this bit
               to clear, do an read and enable RI. This should be configureable
               for every request */
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "RI Check enabled - DDC bus busy");
            rtnValue    =   VPS_EDEVICE_INUSE;
            break;
        }
        /* Step 3 - Clock SCL and clear FIFOs */
        hdmiObj->coreRegOvrLay->DDC_CMD |=  CSL_HDMI_DDC_CMD_CLOCK_SCL;
        timeOut =   VPS_HAL_HDMI_DDC_CMD_TIMEOUT;
        while((hdmiObj->coreRegOvrLay->DDC_STATUS &
               CSL_HDMI_DDC_STATUS_IN_PROG_MASK)
              == CSL_HDMI_DDC_STATUS_IN_PROG_MASK)
        {
            timeOut--;
        }
        if (timeOut == 0x0)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not clock SCL before read");
            rtnValue    =   VPS_ETIMEOUT;
            break;
        }
        hdmiObj->coreRegOvrLay->DDC_CMD |=  CSL_HDMI_DDC_CMD_CLEAR_FIFO;
        timeOut =   VPS_HAL_HDMI_DDC_CMD_TIMEOUT;
        while((hdmiObj->coreRegOvrLay->DDC_STATUS &
               CSL_HDMI_DDC_STATUS_IN_PROG_MASK)
              == CSL_HDMI_DDC_STATUS_IN_PROG_MASK)
        {
            timeOut--;
        }
        if (timeOut == 0x0)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not clear FIFOs");
            rtnValue    =   VPS_ETIMEOUT;
            break;
        }
        /* Step 4 - Load slave address, segment, offsets and byte count */
        hdmiObj->coreRegOvrLay->DDC_ADDR    =
                    (CSL_HDMI_DDC_ADDR_DDC_ADDR_MASK & rParams->slaveAddress);
        hdmiObj->coreRegOvrLay->DDC_SEGM    =
                    (CSL_HDMI_DDC_SEGM_DDC_SEGM_MASK & rParams->segmentPtr);
        hdmiObj->coreRegOvrLay->DDC_OFFSET  =
                    (CSL_HDMI_DDC_OFFSET_DDC_OFFSET_MASK & rParams->offset);
        hdmiObj->coreRegOvrLay->DDC_COUNT1  =
                    (CSL_HDMI_DDC_COUNT1_DDC_COUNT_MASK & rParams->noOfBytes);
        hdmiObj->coreRegOvrLay->DDC_COUNT2  =
                    (CSL_HDMI_DDC_COUNT2_DDC_COUNT_MASK &
                     (rParams->noOfBytes >> 0x08));
        hdmiObj->coreRegOvrLay->DDC_CMD     |=
                     CSL_HDMI_DDC_CMD_SEQ_R_NO_ACK_ON_LAST_BYTE;
        /* Inserting a couple of MicroSecs between command and action */
        timeOut =   0xFF;
        while(timeOut)
        {
            timeOut--;
        }
        /* Check for no-devices and other errors */
        cmdStatusReg    =   hdmiObj->coreRegOvrLay->DDC_STATUS;

        if ((cmdStatusReg & CSL_HDMI_DDC_STATUS_BUS_LOW_MASK) ==
                            CSL_HDMI_DDC_STATUS_BUS_LOW_MASK)
        {
            /* Bus is being held by the slave / others... Slow slaves? */
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Bus being held low");
            rtnValue    =   VPS_ETIMEOUT;
            break;
        }
        if ((cmdStatusReg & CSL_HDMI_DDC_STATUS_NO_ACK_MASK) ==
                            CSL_HDMI_DDC_STATUS_NO_ACK_MASK)
        {
            /* Bus is being held by the slave / others... Slow slaves? */
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "No ACK from the device");
            rtnValue    =   VPS_ETIMEOUT;
            break;
        }

        timeOut =   100u;
        while(timeOut)
        {
            volatile u32  tempDelay  =   0xFF;
            cmdStatusReg    =   hdmiObj->coreRegOvrLay->DDC_STATUS;
            if ((CSL_HDMI_DDC_STATUS_IN_PROG_MASK & cmdStatusReg) ==
                 CSL_HDMI_DDC_STATUS_IN_PROG_MASK)
            {
                hdmiObj->coreRegOvrLay->DDC_ADDR    =
                    (CSL_HDMI_DDC_ADDR_DDC_ADDR_MASK & rParams->slaveAddress);
                hdmiObj->coreRegOvrLay->DDC_CMD     |=
                     CSL_HDMI_DDC_CMD_SEQ_R_NO_ACK_ON_LAST_BYTE;
            }
            while(tempDelay)
            {
                tempDelay--;
            }
        }
        /* Step 5 */
        while(((hdmiObj->coreRegOvrLay->DDC_STATUS &
                CSL_HDMI_DDC_STATUS_IN_PROG_MASK)
              == CSL_HDMI_DDC_STATUS_IN_PROG_MASK) ||
              (hdmiObj->coreRegOvrLay->DDC_STATUS &
                CSL_HDMI_DDC_STATUS_FIFO_EMP_MASK
              != CSL_HDMI_DDC_STATUS_FIFO_EMP_MASK))
        {
            if (hdmiObj->coreRegOvrLay->DDC_STATUS &
                CSL_HDMI_DDC_STATUS_FIFO_EMP_MASK
                    != CSL_HDMI_DDC_STATUS_FIFO_EMP_MASK)
            {
                *bufPtr = (UInt8)(hdmiObj->coreRegOvrLay->DDC_DATA &
                                        CSL_HDMI_DDC_DATA_DDC_DATA_MASK);
                bufPtr++;
                rByteCnt++;
            }
        }
        rParams->noOfBytesRead  =   rByteCnt;
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "VpsHal_hdmiEdidRead_legacy<<<<");
    return (rtnValue);
}

#endif /* EDID_USE_LEGACY_READ */

/**
 *  VpsHal_hdmiEdidRead
 *  \brief  Reads sinks EDID content. Employs Enhanced DDC read.
 *
 *          Reads the specified block of EDID structures of an attched sink.
 *          Controlled by segment address andoffset within the segment.
 *          Takes into account of secnario where sink is un-plugged when read
 *          is in progress.
 *          Requires CLOCK to DDC be enabled and DDC bus clocked at 100KHz,
 *          the SINK required to be attached and detected by this HAL.
 */
static int VpsHal_hdmiEdidRead(   VpsHal_Handle           handle,
                                    Vps_HdmiEdidReadParams  *rParams,
                                    HDMI_Ptr                     args)
{
    int           rtnValue        =   VPS_SOK;
    u32          rByteCnt        =   0x0;
    volatile u32 timeOut;
    volatile u32 cmdStatusReg;
    UInt8               *bufPtr         =   NULL;
    HdmiObject_t        *hdmiObj        =   NULL;
    CSL_Hdmi_CoreRegs   *regOvrLayPtr   =   NULL;
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_ENTER, ">>>>VpsHal_hdmiEdidRead");
    HDMI_GT_assert(hdmiTrace, (args == NULL));
    /*
     * Steps
     * 1. Validate params - including HPD
     * 2. Check for RI_STARTED - In case of enabled HDCP and Ri in progress
     *    this bit would be set. When HDCP is supported stop the Ri check
     *    and proceed to READ EDID, if requested.
     * 3. Clock SCL, required as we would not know that status of SCL. As this
     *    point API could be called by apps at any point. Would require to
     *    reset the slaves and clear internal FIFOs
     * 4. Set up slave address & ofsets, Send the command to perform EDDC read.
     *
     * 5. Go ahead and read the segment, if the command is in progress or
     *    FIFO is not empty.
     */
    while(TRUE)
    {
        /* Step 1 */
        if ((handle == NULL) || (rParams == NULL))
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Invalid params ");
            rtnValue    =   VPS_EBADARGS;
            break;
        }
        hdmiObj =   handle;
        bufPtr  =   (UInt8 *) rParams->bufferPtr;
        if (bufPtr == NULL)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Invalid buffer pointer");
            rtnValue    =   VPS_EBADARGS;
            break;
        }
        /* 10 bits to hold the count - which would be 3FF*/
        if ((rParams->noOfBytes == 0x0) || (rParams->noOfBytes > 0x3FF))
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Invalid byte count");
            rtnValue    =   VPS_EBADARGS;
            break;
        }
        rParams->noOfBytesRead  =   0x0;
        if ((hdmiObj->state != HDMI_HAL_INST_OPENED) ||
            (hdmiObj->isReceiverSensed != TRUE ))
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "HPD not detected - HAL not opened");
            rtnValue    =   VPS_EUNSUPPORTED_OPS;
            break;
        }
        regOvrLayPtr    =   hdmiObj->coreRegOvrLay;
        /* Step 2 */
        if ((regOvrLayPtr->RI_STAT & CSL_HDMI_RI_STAT_RI_STARTED_MASK) ==
            CSL_HDMI_RI_STAT_RI_STARTED_MASK)
        {
            /*
             * When HDCP is supported - disable the RI check - wait for this bit
             * to clear, do an read and enable RI. This should be configureable
             * for every request
             */
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "RI Check enbled - DDC bus busy");
            rtnValue    =   VPS_EDEVICE_INUSE;
            break;
        }

        /* Step 3 - Clock SCL and clear FIFOs */
        if (hdmiObj->isSclClocked == FALSE)
        {
            regOvrLayPtr->DDC_CMD   |=  CSL_HDMI_DDC_CMD_CLOCK_SCL;
            timeOut                 =   VPS_HAL_HDMI_DDC_CMD_TIMEOUT;
            while((regOvrLayPtr->DDC_STATUS & CSL_HDMI_DDC_STATUS_IN_PROG_MASK)
                  == CSL_HDMI_DDC_STATUS_IN_PROG_MASK)
            {
                timeOut--;
            }
            if (timeOut == 0x0)
            {
                HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not clock SCL before read");
                rtnValue    =   VPS_ETIMEOUT;
                break;
            }
            hdmiObj->isSclClocked   =   TRUE;
        }
        regOvrLayPtr->DDC_CMD   |=  CSL_HDMI_DDC_CMD_CLEAR_FIFO;
        timeOut                 =   VPS_HAL_HDMI_DDC_CMD_TIMEOUT;
        while((regOvrLayPtr->DDC_STATUS & CSL_HDMI_DDC_STATUS_IN_PROG_MASK)
              == CSL_HDMI_DDC_STATUS_IN_PROG_MASK)
        {
            timeOut--;
        }
        if (timeOut == 0x0)
        {
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Could not clear FIFOs");
            rtnValue    =   VPS_ETIMEOUT;
            break;
        }

        /* Step 4 - Load slave address, segment, offsets and byte count */
        regOvrLayPtr->DDC_ADDR    =
                    (CSL_HDMI_DDC_ADDR_DDC_ADDR_MASK & rParams->slaveAddress);
        regOvrLayPtr->DDC_SEGM    =
                    (CSL_HDMI_DDC_SEGM_DDC_SEGM_MASK & rParams->segmentPtr);
        regOvrLayPtr->DDC_OFFSET  =
                    (CSL_HDMI_DDC_OFFSET_DDC_OFFSET_MASK & rParams->offset);
        regOvrLayPtr->DDC_COUNT1  =
                    (CSL_HDMI_DDC_COUNT1_DDC_COUNT_MASK & rParams->noOfBytes);
        regOvrLayPtr->DDC_COUNT2  =
                    (CSL_HDMI_DDC_COUNT2_DDC_COUNT_MASK &
                     (rParams->noOfBytes >> 0x08));
        regOvrLayPtr->DDC_CMD     |=
                     CSL_HDMI_DDC_CMD_EDDC_R_NO_ACK_ON_LAST_BYTE;

        /* Inserting a couple of MicroSecs between command and action */
        timeOut =   0xFF;
        while(timeOut)
        {
            timeOut--;
        }
        /* Check for no-devices and other errors */
        cmdStatusReg    =   regOvrLayPtr->DDC_STATUS;

        if ((cmdStatusReg & CSL_HDMI_DDC_STATUS_BUS_LOW_MASK) ==
                            CSL_HDMI_DDC_STATUS_BUS_LOW_MASK)
        {
            /* Bus is being held by the slave / others... Slow slaves? */
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "Bus being held low");
            /* Aborting the READ command */
            regOvrLayPtr->DDC_CMD   |=  CSL_HDMI_DDC_CMD_ABORT;
            timeOut                 =   VPS_HAL_HDMI_DDC_CMD_TIMEOUT;
            while((regOvrLayPtr->DDC_STATUS & CSL_HDMI_DDC_STATUS_IN_PROG_MASK)
                  == CSL_HDMI_DDC_STATUS_IN_PROG_MASK)
            {
                timeOut--;
            }
            HDMI_GT_assert(hdmiTrace, (timeOut == 0x0));
            rtnValue    =   VPS_ETIMEOUT;
            break;
        }
        if ((cmdStatusReg & CSL_HDMI_DDC_STATUS_NO_ACK_MASK) ==
                            CSL_HDMI_DDC_STATUS_NO_ACK_MASK)
        {
            /* Bus is being held by the slave / others... Slow slaves? */
            HDMI_GT_0trace(hdmiTrace, HDMI_GT_ERR, "No ACK from the device");
            /* Aborting the READ command */
            regOvrLayPtr->DDC_CMD   |=  CSL_HDMI_DDC_CMD_ABORT;
            timeOut                 =   VPS_HAL_HDMI_DDC_CMD_TIMEOUT;
            while((regOvrLayPtr->DDC_STATUS & CSL_HDMI_DDC_STATUS_IN_PROG_MASK)
                  == CSL_HDMI_DDC_STATUS_IN_PROG_MASK)
            {
                timeOut--;
            }
            HDMI_GT_assert(hdmiTrace, (timeOut == 0x0));
            rtnValue    =   VPS_ETIMEOUT;
            break;
        }

        /* Step 5 */
        while(rByteCnt < rParams->noOfBytes)
        {
            /* Check if the device was disconnected */
            if (hdmiObj->isReceiverSensed != TRUE)
            {
                /* Device is dis-connected */
                rtnValue    =   VPS_ETIMEOUT;
                break;
            }
            /* If command in progress OR FIFO is not FULL - Read */
            if (((regOvrLayPtr->DDC_STATUS & CSL_HDMI_DDC_STATUS_IN_PROG_MASK)
                    == CSL_HDMI_DDC_STATUS_IN_PROG_MASK) ||
                ((regOvrLayPtr->DDC_STATUS & CSL_HDMI_DDC_STATUS_FIFO_EMP_MASK)
                    != CSL_HDMI_DDC_STATUS_FIFO_EMP_MASK))
            {
                if ((regOvrLayPtr->DDC_STATUS & CSL_HDMI_DDC_STATUS_FIFO_EMP_MASK)
                        != CSL_HDMI_DDC_STATUS_FIFO_EMP_MASK)
                {
                    *bufPtr = (UInt8)(regOvrLayPtr->DDC_DATA &
                                            CSL_HDMI_DDC_DATA_DDC_DATA_MASK);
                    bufPtr++;
                    rByteCnt++;
                }
            }
        }
        /*
         * Aborting the READ command.
         * In case we have completed as expected - no of bytes to read is read
         *  - No issues, aborting on completion is OK
         * If device was unplugged before read could be complete,
         *  - Abort should leave the bus clean
         * If any other error
         *  - Ensure bus is clean
         */
        regOvrLayPtr->DDC_CMD   |=  CSL_HDMI_DDC_CMD_ABORT;
        timeOut                 =   VPS_HAL_HDMI_DDC_CMD_TIMEOUT;
        while((regOvrLayPtr->DDC_STATUS & CSL_HDMI_DDC_STATUS_IN_PROG_MASK)
              == CSL_HDMI_DDC_STATUS_IN_PROG_MASK)
        {
            timeOut--;
        }
        HDMI_GT_assert(hdmiTrace, (timeOut == 0x0));
        rParams->noOfBytesRead  =   rByteCnt;
        break;
    }
    HDMI_GT_0trace(hdmiTrace, HDMI_GT_LEAVE, "VpsHal_hdmiEdidRead<<<<");
    return (rtnValue);
}

#if (0)
/**
 *  initializeDebugInfo
 *  \brief  Used to debug, this function would point the members to registers
 *          that are of intrest while debugging.
 */
static void initializeDebugInfo (HdmiObject_t *instPtr)
{
    debugWpRegs.SYSCONFIG       = &(instPtr->wpRegOvrLay->SYSCONFIG);
    debugWpRegs.IRQSTATUS_RAW   = &(instPtr->wpRegOvrLay->IRQSTATUS_RAW);
    debugWpRegs.IRQSTATUS       = &(instPtr->wpRegOvrLay->IRQSTATUS);
    debugWpRegs.IRQENABLE_SET   = &(instPtr->wpRegOvrLay->IRQENABLE_SET);
    debugWpRegs.DEBOUNCE        = &(instPtr->wpRegOvrLay->DEBOUNCE);
    debugWpRegs.VIDEO_CFG       = &(instPtr->wpRegOvrLay->VIDEO_CFG);
    debugWpRegs.VIDEO_TIMING_H  = &(instPtr->wpRegOvrLay->VIDEO_TIMING_H);
    debugWpRegs.VIDEO_TIMING_H  = &(instPtr->wpRegOvrLay->VIDEO_TIMING_H);

    debugCoreRegs.SRST          = &(instPtr->coreRegOvrLay->SRST);
    debugCoreRegs.SYS_CTRL1     = &(instPtr->coreRegOvrLay->SYS_CTRL1);
    debugCoreRegs.DE_DLY        = &(instPtr->coreRegOvrLay->DE_DLY);
    debugCoreRegs.DE_CTRL       = &(instPtr->coreRegOvrLay->DE_CTRL);
    debugCoreRegs.DE_TOP        = &(instPtr->coreRegOvrLay->DE_TOP);
    debugCoreRegs.DE_CNTL       = &(instPtr->coreRegOvrLay->DE_CNTL);
    debugCoreRegs.DE_CNTH       = &(instPtr->coreRegOvrLay->DE_CNTH);
    debugCoreRegs.DE_LINL       = &(instPtr->coreRegOvrLay->DE_LINL);
    debugCoreRegs.DE_LINH       = &(instPtr->coreRegOvrLay->DE_LINH);
    debugCoreRegs.VID_CTRL      = &(instPtr->coreRegOvrLay->VID_CTRL);
    debugCoreRegs.VID_ACEN      = &(instPtr->coreRegOvrLay->VID_ACEN);
    debugCoreRegs.VID_MODE      = &(instPtr->coreRegOvrLay->VID_MODE);
    debugCoreRegs.VID_DITHER    = &(instPtr->coreRegOvrLay->VID_DITHER);
    debugCoreRegs.INTR1         = &(instPtr->coreRegOvrLay->INTR1);
    debugCoreRegs.INTR4         = &(instPtr->coreRegOvrLay->INTR4);
    debugCoreRegs.INT_UNMASK1   = &(instPtr->coreRegOvrLay->INT_UNMASK1);
    debugCoreRegs.INT_UNMASK2   = &(instPtr->coreRegOvrLay->INT_UNMASK2);
    debugCoreRegs.TMDS_CTRL     = &(instPtr->coreRegOvrLay->TMDS_CTRL);
    debugCoreRegs.HDMI_CTRL     = &(instPtr->coreRegOvrLay->HDMI_CTRL);
    debugCoreRegs.DPD           = &(instPtr->coreRegOvrLay->DPD);

    debugDdcRegs.DDC_SEGM        = &(instPtr->coreRegOvrLay->DDC_SEGM);
    debugDdcRegs.DDC_OFFSET      = &(instPtr->coreRegOvrLay->DDC_OFFSET);
    debugDdcRegs.DDC_COUNT1      = &(instPtr->coreRegOvrLay->DDC_COUNT1);
    debugDdcRegs.DDC_COUNT2      = &(instPtr->coreRegOvrLay->DDC_COUNT2);
    debugDdcRegs.DDC_STATUS      = &(instPtr->coreRegOvrLay->DDC_STATUS);
    debugDdcRegs.DDC_CMD         = &(instPtr->coreRegOvrLay->DDC_CMD);
    debugDdcRegs.DDC_DATA        = &(instPtr->coreRegOvrLay->DDC_DATA);

    debugAviInfoRegs.PB_CTRL1    = &(instPtr->coreRegOvrLay->PB_CTRL1);
    debugAviInfoRegs.AVI_TYPE    = &(instPtr->coreRegOvrLay->AVI_TYPE);
    debugAviInfoRegs.AVI_VERS    = &(instPtr->coreRegOvrLay->AVI_VERS);
    debugAviInfoRegs.AVI_LEN     = &(instPtr->coreRegOvrLay->AVI_LEN);
    debugAviInfoRegs.AVI_CHSUM   = &(instPtr->coreRegOvrLay->AVI_CHSUM);

}
#endif


void HDMI_GT_0trace ( u32    maskType,
                 u32    classtype,
                 char * infoString)
{
#if 1
    printk(infoString, "\n");
#endif
    return;
}


void HDMI_GT_1trace ( u32    maskType,
                 u32    classtype,
                 char * infoString,
                 u32    val1)
{
#if 1
	printk(infoString, "\n", val1);
#endif
    return;
}

void HDMI_GT_assert(u32 a, u32 b)
{
#if 1
    if (b)
	printk("NO Error %d\n", a);
    else
    printk("Error -----------------------\n");
#endif
    return;
}
