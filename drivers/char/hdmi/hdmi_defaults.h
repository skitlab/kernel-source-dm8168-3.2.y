/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2010 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/

/**
 *  \file vpsHalHdmiDefaults.h
 *
 *  \brief Provides default values for configuring various parameters of HDMI
 *         HAL.
 *
 */

/* Un-Resolved
 *
 */
#ifndef _VPSHALHDMIDEFAULTS_H
#define _VPSHALHDMIDEFAULTS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "hdmi_cfg.h"
#include "hdmi.h"


/* ========================================================================== */
/*                             Default Defines                                */
/* ========================================================================== */

/* Stucture Vps_DcModeInfo

    u32              vencId
    u32              isCustomMode
    u32              modeId
    u32              frameWidth
    u32              frameHeight
    u32              scanFormat
    Vps_DcTimingInfo    *tInfo
*/
/**< \brief Defaults of DC display mode used in HDMI initialization- 1080P 60*/
#define VPSHAL_HDMI_DC_DISPLAY_MODE_1080P_60 VPS_HDMI_MODE_1080P_60

/**< \brief Defaults of DC display mode used in HDMI initialization- 1080I 60*/
#define VPSHAL_HDMI_DC_DISPLAY_MODE_1080I_60 VPS_HDMI_MODE_1080I_60
/**< \brief Defaults of DC display mode used in HDMI initialization- 1080P 30*/
#define VPSHAL_HDMI_DC_DISPLAY_MODE_1080P_30 VPS_HDMI_MODE_1080P_30
/**< \brief Defaults of DC display mode used in HDMI initialization- 720P 60 */
#define VPSHAL_HDMI_DC_DISPLAY_MODE_720P_60 VPS_HDMI_MODE_720P_60

/**< \brief Defaults of DC display mode used in HDMI initialization- NTSC */
#define VPSHAL_HDMI_DC_DISPLAY_MODE_NTSC VPS_HDMI_MODE_NTSC

/**< \brief Defaults of DC display mode used in HDMI initialization- PAL */
#define VPSHAL_HDMI_DC_DISPLAY_MODE_PAL VPS_HDMI_MODE_PAL

/* Structure VpsHal_HdmiWpConfig
    u32                      debounceRecvDetect
    u32                      debounceRecvSens
    u32                      isSlaveMode
    VpsHal_HdmiWpPackingMode    packMode
    u32                      isVSyncPolInverted
    u32                      isHSyncPolInverted
    VpsHal_HdmiBitsPerChannel   width              Not Supported
    u32                      vSyncPol           Not Supported
    u32                      hSyncPol           Not Supported
    u32                      hbp                Not Supported
    u32                      hfp                Not Supported
    u32                      hsw                Not Supported
    u32                      vbp                Not Supported
    u32                      vfp                Not Supported
    u32                      vsw                Not Supported
    u32                      cecClockDivisor    Not Supported
*/
/**< \brief Default to initialize HDMI wrapper - except for packMode others
            are recommended not be changed */
#define VPSHAL_HDMIWPCONFIG_10BIT_IF_SALVE {\
                                    0x14, \
                                    0x14, \
                                    TRUE, \
                                    VPS_HAL_HDMI_WP_30BIT_RGB_YUV444,\
                                    FALSE, \
                                    FALSE, \
                                    \
                                    VPS_HAL_HDMI_10_BITS_CHAN_WIDTH, \
                                    TRUE, TRUE, 0x0, 0x0, 0x0, 0x0, 0x0,\
                                     0x0, 0x0}
#define VPSHAL_HDMIWPCONFIG_8BIT_IF_SALVE {\
                                    0x14, \
                                    0x14, \
                                    TRUE, \
                                    VPS_HAL_HDMI_WP_24BIT_RGB_YUV444_YUV422,\
                                    FALSE, \
                                    FALSE, \
                                    \
                                    VPS_HAL_HDMI_8_BITS_CHAN_WIDTH, \
                                    TRUE, TRUE, 0x0, 0x0, 0x0, 0x0, 0x0,\
                                     0x0, 0x0}
/* Structure Vps_HdmiCoreInputConfig
        u32                      dataBusWidth
        u32                      syncGenCfg
    Vps_HdmiCoreDEDelayConfig       deDelayCfg
        u32                      DE_DLY
        u32                      DE_TOP
        u32                      DE_CNTL
        u32                      DE_CNTH
        u32                      DE_LINL
        u32                      DE_LINH
        u32                      edgePol Updated to FALSE on 0206 900PM, back to TRUE
								10:08PM
*/
/**< \brief Default to initialize Core input paramters - Note thst if timming
            paramters are to be changed, ensure to update the HDVENC
            accordingly */
#define VPSHAL_HDMICORE_IPCFG_10BIT_IF_SRCD_SYNC {\
                                       VPS_HAL_HDMI_10_BITS_CHAN_WIDTH,\
                                       VPS_HAL_HDMI_SYNC_SOURCED,\
                                       {0x0, 0x0, 0x0, 0x0, 0x0, 0x0},\
                                       TRUE}
#define VPSHAL_HDMICORE_IPCFG_8BIT_IF_SRCD_SYNC {\
                                       VPS_HAL_HDMI_8_BITS_CHAN_WIDTH,\
                                       VPS_HAL_HDMI_SYNC_SOURCED,\
                                       {0x0, 0x0, 0x0, 0x0, 0x0, 0x0},\
                                       TRUE}

/**< \brief YCbCr to RGB co-efficients */
#define VPSHAL_HDMICSC_YCBCR2RGB_COEFF {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,\
                                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,\
                                        0x0}
/* Structure Vps_HdmiCSC_YCbCr2RGB_Ctrl
 enableRngExp       =   FALSE
 enableFullRngExp   =   FALSE
 customCoEff        =   FALSE
 srcCsSel           =   FALSE
 */
/**< \brief YCbCr to RGB Control - disabled */
#define VPSHAL_HDMICSC_YCBCR2RGBCTRL_DISABLED {FALSE, \
                                              FALSE, \
                                              FALSE, \
                                              FALSE, \
                                              VPSHAL_HDMICSC_YCBCR2RGB_COEFF}

/* Structure Vps_HdmiDitherConfig
    u32  M_D2
    u32  UP2
    u32  STR_422_EN
    u32  D_BC_EN
    u32  D_GC_EN
    u32  D_RC_EN
    u32  DRD
*/
/**< \brief Dithering disabled for now */
#define VPSHAL_HDMIDITHERCONFIG {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}


/* Structure VpsHal_HdmiCoreDataPathConfig
    upSamplerEnable
    YCbCr2RGBCSCEnable
    yCbCr2RGBConfig
    RGBRangeExpEnable
    RGB2YCBCRCSCEnable
    csConvertStandard - SMTPE 170M / BT 709 / ITU-R BT.601 - FALSE
    rangeCompEnable
    downSamplerEnable
    rangeClipEnable
    clipColorSpace
    ditherEnable
    ditherConfig
    outputWidth
*/
/**< \brief Defaults to initialize core - by pass all modules, sets outwidth
            to 10 bits/channel and BT709 for TV */
#define VPSHAL_HDMICOREDATAPATHCONFIG_BYPS_ALL_10BIT_OUTPUT_BT709 {\
                                               FALSE, \
                                               FALSE, \
                                        VPSHAL_HDMICSC_YCBCR2RGBCTRL_DISABLED,\
                                               FALSE, \
                                               FALSE, \
                                               TRUE, \
                                               FALSE, \
                                               FALSE, \
                                               FALSE, \
                                               FALSE, \
                                               FALSE, \
                                               VPSHAL_HDMIDITHERCONFIG, \
                                               VPS_HAL_HDMI_10_BITS_CHAN_WIDTH}


#define VPSHAL_HDMICOREDATAPATHCONFIG_BYPS_ALL_8BIT_OUTPUT_BT709 {\
                                               FALSE, \
                                               FALSE, \
                                        VPSHAL_HDMICSC_YCBCR2RGBCTRL_DISABLED,\
                                               FALSE, \
                                               FALSE, \
                                               TRUE, \
                                               FALSE, \
                                               FALSE, \
                                               FALSE, \
                                               FALSE, \
                                               FALSE, \
                                               VPSHAL_HDMIDITHERCONFIG, \
                                               VPS_HAL_HDMI_8_BITS_CHAN_WIDTH}

/**************************Information Packets Specifies***********************/

/* Structure VpsHal_HdmiAviInfoBarInfo
    barInfoValid       0x0
    topBar             0x0
    bottomBar          0x0
    leftBar            0x0
    rightBar           0x0
*/
/**< \brief Configures BAR information - none for now */
#define VPSHAL_HDMI_AVI_INFOFRAME_BARINFO   {0x0, 0x0, 0x0, 0x0, 0x0}

/* Structure VpsHal_HdmiAviInfoFrameCfg
    outputCs
    barInfo
    useActiveAspectRatio
    scanInfo
    colorimetryInfo        Standard TV ot HD TV (BT709)
    aspectRatio
    activeAspectRatio
    itContentPresent       FALSE
    extColorimetry         0x0
    quantizationRange      0x0
    nonUniformSc
    formatIdentier         0x0
 */


/**< \brief Configures HDMI AVI Info Frame with following configurations
            output color space as RGB, Overscan - for TV, colorometry 709-HD TV
            aspect ration as 16:9 */

#define VPSHAL_HDMI_AVI_INFOFRAME_RGB_OVERSCAN_BT709_169  {\
                            VPS_HAL_HDMI_AVIINFO_OP_CS_RGB, \
                            VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATION_NO, \
                            VPSHAL_HDMI_AVI_INFOFRAME_BARINFO, \
                            VPS_HAL_HDMI_AVIINFO_SCAN_INFO_OVERSCAN, \
                            VPS_HAL_HDMI_AVIINFO_COLORIMETRY_BT709, \
                            VPS_HAL_HDMI_AVIINFO_ASPECTRATIO_16_9, \
                            VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATIO_SAME, \
                            FALSE, \
                            0x0, \
                            0x0, \
                            VPS_HAL_HDMI_AVIINFO_NONUNIFORM_SCALING_NONE, \
                            0x0}

/**< \brief Configures HDMI AVI Info Frame with following configurations
            output color space as RGB, Overscan - for TV, colorometry 709-HD TV
            aspect ration as 4:3 */

#define VPSHAL_HDMI_AVI_INFOFRAME_RGB_OVERSCAN_BT709_43  {\
                            VPS_HAL_HDMI_AVIINFO_OP_CS_RGB, \
                            VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATION_NO, \
                            VPSHAL_HDMI_AVI_INFOFRAME_BARINFO, \
                            VPS_HAL_HDMI_AVIINFO_SCAN_INFO_OVERSCAN, \
                            VPS_HAL_HDMI_AVIINFO_COLORIMETRY_BT709, \
                            VPS_HAL_HDMI_AVIINFO_ASPECTRATIO_4_3, \
                            VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATIO_SAME, \
                            FALSE, \
                            0x0, \
                            0x0, \
                            VPS_HAL_HDMI_AVIINFO_NONUNIFORM_SCALING_NONE, \
                            0x0}
/**< \brief Configures HDMI AVI Info Frame with following configurations
            output color space as RGB, Overscan - for TV, colorometry 601-SD TV
            aspect ration as 4:3 */
#define VPSHAL_HDMI_AVI_INFOFRAME_RGB_OVERSCAN_BT601_43  {\
                            VPS_HAL_HDMI_AVIINFO_OP_CS_RGB, \
                            VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATION_NO, \
                            VPSHAL_HDMI_AVI_INFOFRAME_BARINFO, \
                            VPS_HAL_HDMI_AVIINFO_SCAN_INFO_OVERSCAN, \
                            VPS_HAL_HDMI_AVIINFO_COLORIMETRY_BT601, \
                            VPS_HAL_HDMI_AVIINFO_ASPECTRATIO_4_3, \
                            VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATIO_SAME, \
                            FALSE, \
                            0x0, \
                            0x0, \
                            VPS_HAL_HDMI_AVIINFO_NONUNIFORM_SCALING_NONE, \
                            0x0}

/**< \brief Configures HDMI AVI Info Frame with following configurations
            output color space as RGB, Overscan - for TV, colorometry 601-SD TV
            aspect ration as none */
#define VPSHAL_HDMI_AVI_INFOFRAME_RGB_OVERSCAN_BT601_NO_ASPECT_RATIO  {\
                            VPS_HAL_HDMI_AVIINFO_OP_CS_RGB, \
                            VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATION_NO, \
                            VPSHAL_HDMI_AVI_INFOFRAME_BARINFO, \
                            VPS_HAL_HDMI_AVIINFO_SCAN_INFO_OVERSCAN, \
                            VPS_HAL_HDMI_AVIINFO_COLORIMETRY_BT601, \
                            VPS_HAL_HDMI_AVIINFO_ASPECTRATIO_NONE, \
                            VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATIO_SAME, \
                            FALSE, \
                            0x0, \
                            0x0, \
                            VPS_HAL_HDMI_AVIINFO_NONUNIFORM_SCALING_NONE, \
                            0x0}

/* Structure VpsHal_HdmiInfoFrameCfg
    useAviInfoData   =   TRUE
    aviData
 */
/**< \brief Configures HDMI Packets that would be sent during data island period
            Right now, AVI Info packets are supported. For HD Display */
#define VPSHAL_HDMIINFOFRAME_CFG_RGB_OVERSCAN_BT709_43   {TRUE, \
                                VPSHAL_HDMI_AVI_INFOFRAME_RGB_OVERSCAN_BT709_43}
#define VPSHAL_HDMIINFOFRAME_CFG_RGB_OVERSCAN_BT709_169  {TRUE, \
                                VPSHAL_HDMI_AVI_INFOFRAME_RGB_OVERSCAN_BT709_169}
/**< \brief Configures HDMI Packets that would be sent during data island period
            Right now, AVI Info packets are supported. For NTSC display*/
#define VPSHAL_HDMIINFOFRAME_CFG_RGB_OVERSCAN_BT709_NTSC   {TRUE, \
                VPSHAL_HDMI_AVI_INFOFRAME_RGB_OVERSCAN_BT601_NO_ASPECT_RATIO}
/**< \brief Configures HDMI Packets that would be sent during data island period
            Right now, AVI Info packets are supported. For APL display */
#define VPSHAL_HDMIINFOFRAME_CFG_RGB_OVERSCAN_BT709_PAL   {TRUE, \
                                VPSHAL_HDMI_AVI_INFOFRAME_RGB_OVERSCAN_BT601_43}

/*********************Information Packets Specifies Ends***********************/

/*********************PLL Controller Default Values Starts*********************/
/*
 *      mMultiplier             -   M
 *      mMultiplierFraction     -   M Fraction
 *      m2Multiplier            -   M2
 *      nDivider                -   N   (will be treated as N + 1)
 *      sigmaDeltaValue         -   ceiling[ (M/(N+1)) x 27 (ref_clock) /256]
 *      selFreqDco              -   2 if 500MHz < DCOCLK < 1000MHz
 *                                  4 if 1000MHz < DCOCLK < 2000MHz
 */
#define VPS_HDMI_PLLCTRL_NOT_SUPPORTED  {0u, 0u, 0u, 0u, 0u, 0u}
/**< NOT Supported mode */
#define VPS_HDMI_PLLCTRL_10BIT_IF_1080P_60 {756u, 65536u, 1u, 10u, 8u, 4u }
/**< 1080 P 60 CEA 861 D Configuration -
     selFreqDco = ceiling of ((756 / (10 + 1)) * (27 / 256)) */
#define VPS_HDMI_PLLCTRL_10BIT_IF_1080P_30 {378u, 32768u, 1u, 10u, 4u, 2u }
/**< 1080 P 30 CEA 861 D Configuration -
     selFreqDco = ceiling of ((378 / (10 + 1)) * (27 / 256)) */
#define VPS_HDMI_PLLCTRL_10BIT_IF_1080I_60 {378u, 32768u, 1u, 10u, 4u, 2u }
/**< 1080 I 60 CEA 861 D Configuration -
     selFreqDco = ceiling of ((378 / (10 + 1)) * (27 / 256)) */
#define VPS_HDMI_PLLCTRL_10BIT_IF_720P_60  {378u, 32768u, 1u, 10u, 4u, 2u }
/**< 720 P 60 CEA 861 D Configuration -
     selFreqDco = ceiling of ((378 / (10 + 1)) * (27 / 256)) */
/*********************PLL Controller Default Values Ends***********************/
/* ========================================================================== */
/*                           Debug Info - Used as debug aid                   */
/* ========================================================================== */

/**< \brief Wrapper register that would be primary interest to us */
typedef struct
{
    volatile u32 *SYSCONFIG;
    volatile u32 *IRQSTATUS_RAW;
    volatile u32 *IRQSTATUS;
    volatile u32 *IRQENABLE_SET;
    volatile u32 *DEBOUNCE;
    volatile u32 *VIDEO_CFG;
    volatile u32 *VIDEO_TIMING_H;
    volatile u32 *VIDEO_TIMING_V;
} CSL_WP_Debug_HdmiRegs;

/**< \brief Core register that would be primary interest to us */
typedef struct
{
    volatile u32 *SRST;
    volatile u32 *SYS_CTRL1;
    volatile u32 *DE_DLY;
    volatile u32 *DE_CTRL;
    volatile u32 *DE_TOP;
    volatile u32 *DE_CNTL;
    volatile u32 *DE_CNTH;
    volatile u32 *DE_LINL;
    volatile u32 *DE_LINH;
    volatile u32 *VID_CTRL;
    volatile u32 *VID_ACEN;
    volatile u32 *VID_MODE;
    volatile u32 *VID_DITHER;
    volatile u32 *INTR1;
    volatile u32 *INTR4;
    volatile u32 *INT_UNMASK1;
    volatile u32 *INT_UNMASK2;
    volatile u32 *TMDS_CTRL;
    volatile u32 *HDMI_CTRL;
    volatile u32 *DPD;
} CSL_CORE_Debug_HdmiRegs;

/**< \brief Core DDC register that would be primary interest to us */
typedef struct
{
    volatile u32 *DDC_SEGM;
    volatile u32 *DDC_OFFSET;
    volatile u32 *DDC_COUNT1;
    volatile u32 *DDC_COUNT2;
    volatile u32 *DDC_STATUS;
    volatile u32 *DDC_CMD;
    volatile u32 *DDC_DATA;
}CSL_CORE_Debug_DdcRegs;

/**< \brief Core AVI info frame register that would be primary interest to us */
typedef struct
{
    volatile u32 *PB_CTRL1;
    volatile u32 *AVI_TYPE;
    volatile u32 *AVI_VERS;
    volatile u32 *AVI_LEN;
    volatile u32 *AVI_CHSUM;
}CAL_CORE_Debug_AVIInfoRegs;

#ifdef __cplusplus
}
#endif

#endif /* _VPSHALHDMIDEFAULTS_H */

