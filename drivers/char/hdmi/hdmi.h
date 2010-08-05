/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2010 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/

/**
 *  \file vpshal_hdmi.h
 *
 *  \brief Provides interfaces that could be used to configure HDMI.
 *
 *  Policies
 *      1. AV Info frames would be transmitted at least once between 2 frames.
 *      2. Audio is not yet supported
 *         When supported - AV Info should be sent least once between 2 frames.
 *          and Audio info frames should be sent least once between 2 frames.
 */

#ifndef _VPSHAL_HDMI_H
#define _VPSHAL_HDMI_H

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "hdmi_cfg.h"
#include "hdmi_defaults.h"

/* ========================================================================== */
/*            Defaults that could be used to initialize HDMI HAL              */
/*  Recommended to start with Vps_HdmiWpConfig, if reading for first time     */
/*  To understand the configurations paramters for the HDMI HAL               */
/* ========================================================================== */
/**
 * \brief Defaults that could be used initialize HDMI HAL in
 *        1080 P, 60 FPS, 4:3 Aspect Ratio, for HD TV
 *        Wrapper - confiured as 10 bit interface with HDVENC and SLAVE
 *        Core input configured as 10 bit interface with syncs sourced from
 *          wrapper
 *        Core Data path - All in bypass mode, outwidth set 10 bits/channel
 *          - In case color space converter is enabled - set to BT709.
 */
#define HDMI_HAL_INIT_1080P_60_16_9_HD {\
        TRUE, VPSHAL_HDMI_DC_DISPLAY_MODE_1080P_60,\
        TRUE, VPSHAL_HDMIWPCONFIG_10BIT_IF_SALVE,\
        TRUE, VPSHAL_HDMICORE_IPCFG_10BIT_IF_SRCD_SYNC,\
        TRUE, VPSHAL_HDMICOREDATAPATHCONFIG_BYPS_ALL_10BIT_OUTPUT_BT709,\
        TRUE, VPSHAL_HDMIINFOFRAME_CFG_RGB_OVERSCAN_BT709_169}

#define HDMI_HAL_INIT_8BIT_1080P_60_16_9_HD {\
        TRUE, VPSHAL_HDMI_DC_DISPLAY_MODE_1080P_60,\
        TRUE, VPSHAL_HDMIWPCONFIG_8BIT_IF_SALVE,\
        TRUE, VPSHAL_HDMICORE_IPCFG_8BIT_IF_SRCD_SYNC,\
        TRUE, VPSHAL_HDMICOREDATAPATHCONFIG_BYPS_ALL_8BIT_OUTPUT_BT709,\
        TRUE, VPSHAL_HDMIINFOFRAME_CFG_RGB_OVERSCAN_BT709_169}

/**
 * \brief Defaults that could be used initialize HDMI HAL in
 *        1080 P, 30 FPS, 4:3 Aspect Ratio, for HD TV
 *        Wrapper - confiured as 10 bit interface with HDVENC and SLAVE
 *        Core input configured as 10 bit interface with syncs sourced from
 *          wrapper
 *        Core Data path - All in bypass mode, outwidth set 10 bits/channel
 *          - In case color space converter is enabled - set to BT709.
 */
#define HDMI_HAL_INIT_1080P_30_16_9_HD {\
        TRUE, VPSHAL_HDMI_DC_DISPLAY_MODE_1080P_30,\
        TRUE, VPSHAL_HDMIWPCONFIG_10BIT_IF_SALVE,\
        TRUE, VPSHAL_HDMICORE_IPCFG_10BIT_IF_SRCD_SYNC,\
        TRUE, VPSHAL_HDMICOREDATAPATHCONFIG_BYPS_ALL_10BIT_OUTPUT_BT709,\
        TRUE, VPSHAL_HDMIINFOFRAME_CFG_RGB_OVERSCAN_BT709_169}

#define HDMI_HAL_INIT_8BIT_1080P_30_16_9_HD {\
        TRUE, VPSHAL_HDMI_DC_DISPLAY_MODE_1080P_30,\
        TRUE, VPSHAL_HDMIWPCONFIG_8BIT_IF_SALVE,\
        TRUE, VPSHAL_HDMICORE_IPCFG_8BIT_IF_SRCD_SYNC,\
        TRUE, VPSHAL_HDMICOREDATAPATHCONFIG_BYPS_ALL_8BIT_OUTPUT_BT709,\
        TRUE, VPSHAL_HDMIINFOFRAME_CFG_RGB_OVERSCAN_BT709_169}

/**
 * \brief Defaults that could be used initialize HDMI HAL in
 *        1080I P, 60 FPS, 4:3 Aspect Ratio, for HD TV
 *        Wrapper - confiured as 10 bit interface with HDVENC and SLAVE
 *        Core input configured as 10 bit interface with syncs sourced from
 *          wrapper
 *        Core Data path - All in bypass mode, outwidth set 10 bits/channel
 *          - In case color space converter is enabled - set to BT709.
 */
#define HDMI_HAL_INIT_1080I_60_16_9_HD {\
        TRUE, VPSHAL_HDMI_DC_DISPLAY_MODE_1080I_60,\
        TRUE, VPSHAL_HDMIWPCONFIG_10BIT_IF_SALVE,\
        TRUE, VPSHAL_HDMICORE_IPCFG_10BIT_IF_SRCD_SYNC,\
        TRUE, VPSHAL_HDMICOREDATAPATHCONFIG_BYPS_ALL_10BIT_OUTPUT_BT709,\
        TRUE, VPSHAL_HDMIINFOFRAME_CFG_RGB_OVERSCAN_BT709_169}

#define HDMI_HAL_INIT_8BIT_1080I_60_16_9_HD {\
        TRUE, VPSHAL_HDMI_DC_DISPLAY_MODE_1080I_60,\
        TRUE, VPSHAL_HDMIWPCONFIG_8BIT_IF_SALVE,\
        TRUE, VPSHAL_HDMICORE_IPCFG_8BIT_IF_SRCD_SYNC,\
        TRUE, VPSHAL_HDMICOREDATAPATHCONFIG_BYPS_ALL_8BIT_OUTPUT_BT709,\
        TRUE, VPSHAL_HDMIINFOFRAME_CFG_RGB_OVERSCAN_BT709_169}

/**
 * \brief Defaults that could be used initialize HDMI HAL in
 *        720 P, 60 FPS, 16:9 Aspect Ratio, for HD TV
 *        Wrapper - confiured as 10 bit interface with HDVENC and SLAVE
 *        Core input configured as 10 bit interface with syncs sourced from
 *          wrapper
 *        Core Data path - All in bypass mode, outwidth set 10 bits/channel
 *          - In case color space converter is enabled - set to BT709.
 */
#define HDMI_HAL_INIT_10BIT_720P_60_16_9_HD {\
        TRUE, VPSHAL_HDMI_DC_DISPLAY_MODE_720P_60,\
        TRUE, VPSHAL_HDMIWPCONFIG_10BIT_IF_SALVE,\
        TRUE, VPSHAL_HDMICORE_IPCFG_10BIT_IF_SRCD_SYNC,\
        TRUE, VPSHAL_HDMICOREDATAPATHCONFIG_BYPS_ALL_10BIT_OUTPUT_BT709,\
        TRUE, VPSHAL_HDMIINFOFRAME_CFG_RGB_OVERSCAN_BT709_43}

#define HDMI_HAL_INIT_8BIT_720P_60_16_9_HD {\
        TRUE, VPSHAL_HDMI_DC_DISPLAY_MODE_720P_60,\
        TRUE, VPSHAL_HDMIWPCONFIG_8BIT_IF_SALVE,\
        TRUE, VPSHAL_HDMICORE_IPCFG_8BIT_IF_SRCD_SYNC,\
        TRUE, VPSHAL_HDMICOREDATAPATHCONFIG_BYPS_ALL_8BIT_OUTPUT_BT709,\
        TRUE, VPSHAL_HDMIINFOFRAME_CFG_RGB_OVERSCAN_BT709_43}

/**
 * \brief Defaults that could be used initialize HDMI HAL in
 *        NTSC, No Aspect Ratio specified, for SD TV
 *        Wrapper - confiured as 10 bit interface with HDVENC and SLAVE
 *        Core input configured as 10 bit interface with syncs sourced from
 *          wrapper
 *        Core Data path - All in bypass mode, outwidth set 10 bits/channel
 *          - In case color space converter is enabled - set to BT709.
 */
#define HDMI_HAL_INIT_NTSC_SD {\
        TRUE, VPSHAL_HDMI_DC_DISPLAY_MODE_NTSC,\
        TRUE, VPSHAL_HDMIWPCONFIG_10BIT_IF_SALVE,\
        TRUE, VPSHAL_HDMICORE_IPCFG_10BIT_IF_SRCD_SYNC,\
        TRUE, VPSHAL_HDMICOREDATAPATHCONFIG_BYPS_ALL_10BIT_OUTPUT_BT709,\
        TRUE, VPSHAL_HDMIINFOFRAME_CFG_RGB_OVERSCAN_BT709_NTSC}

/**
 * \brief Defaults that could be used initialize HDMI HAL in
 *        PAL, 4:3 Aspect Ratio, for SD TV
 *        Wrapper - confiured as 10 bit interface with HDVENC and SLAVE
 *        Core input configured as 10 bit interface with syncs sourced from
 *          wrapper
 *        Core Data path - All in bypass mode, outwidth set 10 bits/channel
 *          - In case color space converter is enabled - set to BT709.
 */
#define HDMI_HAL_INIT_PAL_SD {\
        TRUE, VPSHAL_HDMI_DC_DISPLAY_MODE_PAL,\
        TRUE, VPSHAL_HDMIWPCONFIG_10BIT_IF_SALVE,\
        TRUE, VPSHAL_HDMICORE_IPCFG_10BIT_IF_SRCD_SYNC,\
        TRUE, VPSHAL_HDMICOREDATAPATHCONFIG_BYPS_ALL_10BIT_OUTPUT_BT709,\
        TRUE, VPSHAL_HDMIINFOFRAME_CFG_RGB_OVERSCAN_BT709_PAL}

/* ========================================================================== */
/*                                  Types                                     */
/* ========================================================================== */

/**
 *  \brief Instace initialization parameters.
 *
 *      HDMI port instance parameters, that defines base address of the
 *      peripheral, Interrupts to be used among others...
 */
typedef struct
{
    u32      wpBaseAddr;
    /**< Base address of the wrapper */
    u32      coreBaseAddr;
    /**< Base address of the core */
    u32      phyBaseAddr;
    /**< Base address of the PHY */
    u32      pllCtrlBaseAddr;
    /**< Base address of the PLL Controller */
    u32      interruptNo;
    /**< Interrupt number allocated to HDMI */
    u32      vencId;
    /**< Paired venc ID, as defined by Display Controller */
    u32      encoderId;
    /**< Identifer assigned to this instace by Display Controller */
}Vps_HdmiInitParams;


/**
 *  \brief PLL Configuration params
 *
 *      PLL configurations parameters are computed for a given combination of
 *      reference clock (fixed with EVM) and operating mode such as 1080 P 60,
 *      1080 P 30, 720 P 60, etc... The structure below holds the configuration
 *      for all the supported resolutions
 */
typedef struct
{
    u32      mDivider;
    /**< M Dividers Mantissa part */
    u32      mDividerFraction;
    /**< M Dividers fraction part */
    u32      m2Divider;
    /**< M2 Divider */
    u32      nDivider;
    /**< N Divider */
    u32      sigmaDeltaValue;
    /**< Sigma Delta value for ADPLL, based on PLL.
         ceiling[ (M/(N+1)) x reference clock /256] */
    u32      selFreqDco;
    /**< Selected frequency domain
         2 if 500MHz < DCOCLK < 1000MHz
         4 if 1000MHz < DCOCLK < 2000MHz */
}Vps_HdmiPllCtrlParams;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  VpsHal_hdmiInit
 *  \brief Initializes HDMI port objects
 *
 *         Initializes all instances of HDMI, Each instance is updated with
 *         default configuration.
 *
 *  \param initParams   Initialization parameters, such as base address and
 *                      interrupt number.
 */
int VpsHal_hdmiInit(Vps_HdmiInitParams *initParams);


/**
 *  VpsHal_hdmiDeInit
 *  \brief Unwinds the operations done in VpsHal_hdmiInit.
 *
 *  \param arg      Not used currently. Meant for future purpose.
 *                  should be set to NULL
 */
int VpsHal_hdmiDeInit(HDMI_Ptr args);


/**
 *  VpsHal_hdmiOpen
 *  \brief Creates a instance of HDMI Hal.
 *
 *         Initialize the instance of HDMI, loads the provided config. If no
 *         config is provided, the default config would be applied.
 *         After a successful open, Hot Plug could be detected and display
 *         could be started on this instance.
 *         Returns a handle of type VpsHal_Handle, which is expected to be used
 *         to make other HDMI Hal calls.
 *
 *  \param instance [IN]    Instance of hdmi peripheral to be used. 0x0 is the
 *                          first instance and 0x1 is second instance and so on
 *  \param config   [IN]    HDMI configurations, if required to override
 *                          default configurations.
 *                          Please refer VpsHal_HdmiDefaults.h or get the
 *                          current configurations using VpsHal_hdmiGetConfig ()
 *                          after this functions returns a handle.
 *  \param status   [OUT]   Memory for this is provided by the caller. This API
 *                          updates this variable with status of open.
 *  \param args     [IN]    Not used for now. Reserved for future expansion.
 *                          should be NULL for now
 *
 *  \par CAUTION Hot Plug Detect - In cases where the sink is connected prior to
 *           opening this HAL, if HPD callback is attached, it would be called
 *           in the context of this API, before the API returns.
 *           Applications should make subsequent HDMI HAL calls after completion
 *           of this call.
 */
VpsHal_Handle VpsHal_hdmiOpen(  u32 instance,
                                Vps_HdmiConfigParams *config,
                                int   *status,
                                HDMI_Ptr     args);

/**
 *  VpsHal_hdmiClose
 *  \brief Closes an instance of HDMI Hal
 *
 *         De-Register the interrupts and reset the peripheral.
 *
 *  \param handle   [IN]       Handle to instance of HDMI Hal
 *
 */
int VpsHal_hdmiClose( VpsHal_Handle handle, HDMI_Ptr args);


/**
 *  VpsHal_hdmiGetMode
 *  \brief Used to get the current mode of operation.
 *
 *  \param handle   [IN]    Handle to instance of HDMI Hal
 *  \param modeInfo [IN]    Pointer to structure that would hold mode info
 */
int VpsHal_hdmiGetMode(VpsHal_Handle handle, Vps_HdmiModeId *modeInfo);


/**
 *  VpsHal_hdmiSetMode
 *  \brief Used to get the current mode of operation.
 *
 *  \param handle   [IN]    Handle to instance of HDMI Hal
 *  \param modeInfo [IN]    Pointer to structure that would hold new mode info
 */
int VpsHal_hdmiSetMode(VpsHal_Handle handle, Vps_HdmiModeId *modeInfo);


/**
 *  VpsHal_hdmiStart
 *  \brief Starts streaming of video over HDMI.
 *
 *         VpsHal_hdmiStart is expected to called after
 *         1. Detection of a sink (i.e. Application supplied callback
 *            supplied in cbOnHPD is called)
 *         2. Applications have configured HDMI HAL to suit the needs of the
 *            sink. (Applications are expected to read the sinks EDID and
 *            configure HDMI HAL/Video source)
 *         3. The video source - HDVPSS is configured and is streaming video
 *
 *  \param handle   [IN]    Handle to instance of HDMI Hal
 *  \param args     [IN]    Not used for now. Reserved for future expansion.
 *                          should be NULL for now
 */
int VpsHal_hdmiStart( VpsHal_Handle handle, HDMI_Ptr args);


/**
 *  VpsHal_hdmiStop
 *  \brief Stops streaming of video over HDMI.
 *
 *         VpsHal_hdmiStop is expected only when streaming via HDMI
 *
 *  \param handle   [IN]    Handle to instance of HDMI Hal
 *  \param args     [IN]    Not used for now. Reserved for future expansion.
 *                          should be NULL for now
 */
int VpsHal_hdmiStop( VpsHal_Handle handle, HDMI_Ptr args);


/**
 *  VpsHal_hdmiControl
 *  \brief Used to updated HDMI non-generic configurations, notifications, among
 *         others.
 *
 *         The HDMI Peripheral has capabilities to convert the color space being
 *         sent out, dither the output, etc... #Vps_HdmiConfigParams list the
 *         configureable options available.
 *         Could also be used to notify apps on dectection of a sink, read sinks
 *         EDID blocks, etc...
 *
 *  \param handle   [IN]    Handle to instance of HDMI Hal
 *  \param cmd      [IN]    Command - Supported commands are...
 *                          #IOCTL_VPS_DCTRL_HDMI_ATTACH_HPD_CB
 *                          #IOCTL_VPS_DCTRL_HDMI_GET_CONFIG
 *                          #IOCTL_VPS_DCTRL_HDMI_SET_CONFIG
 *                          #IOCTL_VPS_DCTRL_HDMI_READ_EDID
 *  \param cmdArgs  [IN]    Depends on the command. Please refer the command
 *  \param additionalArgs [IN] Depends on the command. Please refer the command
 */
int VpsHal_hdmiControl (  VpsHal_Handle  handle,
                            u32         cmd,
                            HDMI_Ptr            cmdArgs,
                            HDMI_Ptr            additionalArgs);

#ifdef __cplusplus
}
#endif

#endif /* _VPSHAL_HDMI_H */
