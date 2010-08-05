/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2010 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/

/**
 * \ingroup VPS_DRV_FVID2_VPS_COMMON
 * \addtogroup VPS_DRV_FVID2_VPS_COMMON_HDMI HD-VPSS - Display HDMI Config API
 *
 * @{
 */

/**
 *  \file vps_cfgHdmi.h
 *
 *  \brief HD-VPSS - Display HDMI Config API
 *
 *  Provides interfaces that could be used to configure HDMI.
 *  Applications are expected to configure HDMI encoder via Display Controller
 *  and FVID2 Control interface
 *
 *  For example
 *  1. Attach a callback to be notified of sink attach/detach status
 *      1.A. Use #IOCTL IOCTL_VPS_DCTRL_HDMI_ATTACH_HPD_CB
 *
 *  2. Get the current HDMI peripheral configuration
 *      2.A Use #IOCTL_VPS_DCTRL_HDMI_GET_CONFIG
 *
 *  3. Read sinks EDID
 *      3.A Use #IOCTL_VPS_DCTRL_HDMI_READ_EDID
 *
 *  4. Update HDMI configuration
 *      4.A Use #IOCTL_VPS_DCTRL_HDMI_SET_CONFIG
 *
 *  Policies
 *      1. AV Info frames would be transmitted at least once between 2 frames.
 *      2. Audio is not yet supported
 *         When supported - AV Info should be sent least once between 2 frames.
 *          and Audio info frames should be sent least once between 2 frames.
 */

#ifndef _VPS_HDMICFG_H
#define _VPS_HDMICFG_H

#ifdef __cplusplus
extern "C" {
#endif


/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "cslr_hdmi.h"
#include <linux/string.h>
# include <linux/kernel.h>

/* ========================================================================== */
/*                          Linux Types                                       */
/* ========================================================================== */
//typedef unsigned int  u32;
typedef unsigned int  Bool;
typedef unsigned char UInt8;
typedef unsigned short UInt16;

#define Uint32  u32
//#define NULL    0
#define TRUE    1
#define FALSE   0

#define VpsHal_Handle           void *
#define interruptHandle         void *
#define HDMI_Ptr                void *
#define UArg                    void *

#define VPS_SOK                 (0x0u)
#define VPS_EBADARGS            (-1)
#define VPS_EUNSUPPORTED_CMD    (-2)
#define VPS_EFAIL               (-3)
#define VPS_ETIMEOUT            (-4)
#define VPS_EUNSUPPORTED_OPS    (-5)
#define VPS_EALLOC              (-6)
#define VPS_EDRIVER_INUSE       (-7)
#define VPS_EOUT_OF_RANGE       (-8)
#define VPS_EDEVICE_INUSE       (-9)

#define HDMI_GT_ENTER           0
#define HDMI_GT_DEBUG           1
#define HDMI_GT_LEAVE           2
#define HDMI_GT_ERR             3
#define HDMI_GT_INFO            4


#define VpsUtils_memcpy         memcpy
#define VpsHalHdmiTrace         (0x0)

#define disableInterrupts       1;
#define enableInterrupts

#define INCLUDE_BIOS_INT_HANDLER (0x0)

#define IGNORE_TV

/* ========================================================================== */
/*                          HDMI Silicon dependecy                            */
/* ========================================================================== */
#define HDMI_PER_CNT            (1u)
#define HDMI_CORE_0_REGS        (0x46c00400u)
#define HDMI_WP_0_REGS          (0x46c00000u)
#define HDMI_PHY_0_REGS         (0x48122000u)
#define HDMI_PLLCTRL_0_REGS     (0x46c00600u)
#define INTC_EVENTID_HDMI       (7u)
#define PLATFORM_EVM_SI			(1u)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef int (*Vps_Hdmi_CbFxn) (u32 status, HDMI_Ptr appData);


#define IOCTL_HDMI_START      (0)
#define IOCTL_HDMI_STOP       (1)
#define IOCTL_HDMI_GET_STATUS (2)
#define IOCTL_HDMI_READ_EDID  (3)
#define IOCTL_HDMI_GET_CONFIG (4)
#define IOCTL_HDMI_SET_CONFIG (5)
#define IOCTL_HDMI_ATTACH_HPD_CB (6)


/**
 * \brief enum Vps_DcModeId
 *  Enum defining ID of the standard Modes. Standard timinig parameters
 *  will be used if the standard mode id is used for configuring mode
 *  in the venc.
 */
typedef enum
{
    VPS_HDMI_MODE_NTSC = 0,
    /**< Mode Id for NTSC */
    VPS_HDMI_MODE_PAL,
    /**< Mode Id for PAL */
    VPS_HDMI_MODE_1080P_60,
    /**< Mode Id for 1080p at 60fps mode */
    VPS_HDMI_MODE_720P_60,
    /**< Mode Id for 720p at 60fps mode */
    VPS_HDMI_MODE_1080I_60,
    /**< Mode Id for 1080I at 60fps mode */
    VPS_HDMI_MODE_1080P_30,
    /**< Mode Id for 1080P at 30fps mode */
    VPS_HDMI_MAX_MODE
    /**< This should be the last mode id */
} Vps_HdmiModeId;

/**
 *  \brief Defines the number of bits per channel that would be used to transmit
 *         /receive video data to/from HDMIPHY/Wrapper.
 */
typedef enum
{
    VPS_HAL_HDMI_8_BITS_CHAN_WIDTH  =   0x0,
    /**< 8 bits per channel */
    VPS_HAL_HDMI_10_BITS_CHAN_WIDTH,
    /**< 10 bits per channel */
    VPS_HAL_HDMI_12_BITS_CHAN_WIDTH,
    /**< 12 bits per channel */
    VPS_HAL_HDMI_MAX_BITS_CHAN_WIDTH
    /**< Upper limit guard */
}Vps_HdmiBitsPerChannel;


/**
 *  \brief Defines the options available for generation of vSync, hSync and DE
 *         synchronization signals
 */
typedef enum
{
    VPS_HAL_HDMI_SYNC_EXTRACT  =   0x0,
    /**< Use extracted hSync, vSync and DE from input*/
    VPS_HAL_HDMI_SYNC_GENERATE,
    /**< Use incoming (from wrapper/hdvenc) vSync and hSync but generate DE */
    VPS_HAL_HDMI_SYNC_SOURCED,
    /**< Use incoming (from wrapper/hdvenc) vSync, hSync and DE */
    VPS_HAL_HDMI_SYNC_MAX
    /**< Upper limit guard */
}Vps_HdmiCoreSyncGenCfg;


/**
 *  \brief Defines the arrangement of video data in interface between the source
 *         and wrapper.
 */
typedef enum
{
    VPS_HAL_HDMI_WP_30BIT_RGB_YUV444    =   0x0,
    /**< Pack as video_data[35:26], video_data[23:14] and video_data[11:2] */
    VPS_HAL_HDMI_WP_24BIT_RGB_YUV444_YUV422,
    /**< Pack as  video_data[35:28], video_data[23:16], video_data[11:4] */
    VPS_HAL_HDMI_WP_20BIT_YUV422,
    /**< Pack as  video_data[35:28], video_data[23:16], video_data[11:10],
         video_data[7:6]*/
    VPS_HAL_HDMI_WP_16BIT_YUV422,
    /**< Pack as  video_data[35:28], video_data[23:16], video_data[11:4] */
    VPS_HAL_HDMI_WP_NO_PACK             =   0x7
    /**< No packing */
}Vps_HdmiWpPackingMode;


/**
 *  \brief YCbCr to RGB CSC coefficients
 *
 *         Defines the place holder for YCbCr to RGB color space converters
 *         coefficients, if applications choose to override the default
 *         configuration.
 *
 *         Note that members names do not follow coding guidelines to enable
 *         easy co-relation between members and register names used in
 *         specifications.
 */
typedef struct
{
    u32  Y2RCOEFF_L;
    /**< Refer peripheral documentation */
    u32  Y2RCOEFF_H;
    /**< Refer peripheral documentation */
    u32  CR2RCOEFF_L;
    /**< Refer peripheral documentation */
    u32  CR2RCOEFF_H;
    /**< Refer peripheral documentation */
    u32  CB2BCOEFF_L;
    /**< Refer peripheral documentation */
    u32  CB2BCOEFF_H;
    /**< Refer peripheral documentation */
    u32  CR2GCOEFF_L;
    /**< Refer peripheral documentation */
    u32  CR2GCOEFF_H;
    /**< Refer peripheral documentation */
    u32  YOFFS1_L;
    /**< Refer peripheral documentation */
    u32  YOFFS1_U;
    /**< Refer peripheral documentation */
    u32  OFFS1_L;
    /**< Refer peripheral documentation */
    u32  OFFS1_M;
    /**< Refer peripheral documentation */
    u32  OFFS1_H;
    /**< Refer peripheral documentation */
    u32  OFFS2_L;
    /**< Refer peripheral documentation */
    u32  OFFS2_H;
    /**< Refer peripheral documentation */
    u32  DC_LEV_L;
    /**< Refer peripheral documentation */
    u32  DC_LEV_H;
    /**< Refer peripheral documentation */
}Vps_HdmiCSC_YCbCr2RGB_coEff;



/**
 *  \brief RGB to xvYCC CSC coefficients
 *
 *         Defines the place holder for coefficients, if applications choose
 *         to override the default configuration.
 *         Note that members names do not follow coding guidelines to enable
 *         easy co-relation between members and register names used in
 *         specifications.
 */
typedef struct
{
    u32  R2Y_COEFF_LOW;
    /**< Refer peripheral documentation */
    u32  R2Y_COEFF_UP;
    /**< Refer peripheral documentation */
    u32  G2Y_COEFF_LOW;
    /**< Refer peripheral documentation */
    u32  G2Y_COEFF_UP;
    /**< Refer peripheral documentation */
    u32  B2Y_COEFF_LOW;
    /**< Refer peripheral documentation */
    u32  B2Y_COEFF_UP;
    /**< Refer peripheral documentation */
    u32  R2CB_COEFF_LOW;
    /**< Refer peripheral documentation */
    u32  R2CB_COEFF_UP;
    /**< Refer peripheral documentation */
    u32  G2CB_COEFF_LOW;
    /**< Refer peripheral documentation */
    u32  G2CB_COEFF_UP;
    /**< Refer peripheral documentation */
    u32  B2CB_COEFF_LOW;
    /**< Refer peripheral documentation */
    u32  B2CB_COEFF_UP;
    /**< Refer peripheral documentation */
    u32  R2CR_COEFF_LOW;
    /**< Refer peripheral documentation */
    u32  G2CR_COEFF_LOW;
    /**< Refer peripheral documentation */
    u32  G2CR_COEFF_UP;
    /**< Refer peripheral documentation */
    u32  B2CR_COEFF_LOW;
    /**< Refer peripheral documentation */
    u32  B2CR_COEFF_UP;
    /**< Refer peripheral documentation */
    u32  RGB_OFFSET_LOW;
    /**< Refer peripheral documentation */
    u32  RGB_OFFSET_UP;
    /**< Refer peripheral documentation */
    u32  Y_OFFSET_LOW;
    /**< Refer peripheral documentation */
    u32  Y_OFFSET_UP;
    /**< Refer peripheral documentation */
    u32  CBCR_OFFSET_LOW;
    /**< Refer peripheral documentation */
    u32  CBCR_OFFSET_UP;
    /**< Refer peripheral documentation */
}Vps_Hdmi_RGB2xvYCC_coEffs;


/**
 *  \brief  xvYCC to RGB/YCbCr CSC coefficients
 *
 *         Defines the place holder for coefficients, if applications choose
 *         to override the default configuration.
 *         Note that members names do not follow coding guidelines to enable
 *         easy co-relation between members and register names used in
 *         specifications.
 */
typedef struct
{
    u32  Y2R_COEFF_LOW;
    /**< Refer peripheral documentation */
    /* TBD spec says this is valid when RGB_2_XVYCC.XV_CO_OV = 1 which is not consistent */
    u32  Y2R_COEFF_UP;
    /**< Refer peripheral documentation */
    u32  CR2R_COEFF_LOW;
    /**< Refer peripheral documentation */
    u32  CR2R_COEFF_UP;
    /**< Refer peripheral documentation */
    u32  CB2B_COEFF_LOW;
    /**< Refer peripheral documentation */
    u32  CB2B_COEFF_UP;
    /**< Refer peripheral documentation */
    u32  CR2G_COEFF_LOW;
    /**< Refer peripheral documentation */
    u32  CR2G_COEFF_UP;
    /**< Refer peripheral documentation */
    u32  CB2G_COEFF_LOW;
    /**< Refer peripheral documentation */
    u32  CB2G_COEFF_UP;
    /**< Refer peripheral documentation */
    u32  YOFFSET1_LOW;
    /**< Refer peripheral documentation */
    u32  YOFFSET1_UP;
    /**< Refer peripheral documentation */
    u32  OFFSET1_LOW;
    /**< Refer peripheral documentation */
    u32  OFFSET1_MID;
    /**< Refer peripheral documentation */
    u32  OFFSET1_UP;
    /**< Refer peripheral documentation */
    u32  OFFSET2_LOW;
    /**< Refer peripheral documentation */
    u32  OFFSET2_UP;
    /**< Refer peripheral documentation */
    u32  DCLEVEL_LOW;
    /**< Refer peripheral documentation */
    u32  DC_LEVEL_UP;
    /**< Refer peripheral documentation */
}Vps_Hdmi_xvYCC2RGB_YCbCr_coEffs;


/**
 *  \brief Configurations to enable RGB to xvYCC color space conversion
 */
typedef struct
{
    u32  RGB2xvYCC_XV_EN;
    /**<  Enable xvYCC color space. Non Zero enables */
    u32  RGB2xvYCC_XV_CO_OV;
    /**<  TRUE overrides default coefficients with application supplied
              coefficients. Non Zero enables */
    u32  RGB2xvYCC_XV_FUS;
    /**<  Configure xvYCC mode, non zero sets mode to full scale, full scale is
              disabled otherwise. Applicable only when enablexvYCC is TRUE */
    Vps_Hdmi_RGB2xvYCC_coEffs     *RGB2xvYCC_Coefs;
    /**<  Specify xvYCC coefficients */
    Vps_Hdmi_RGB2xvYCC_coEffs     space4RGB2xvYCCCoefs;
    /**<  Place holder for xvYCC coefficients */

    u32      xvYCC2RGB_YCbCr_BYP_ALL;
    /**<  Bound to change - not used for now */
    u32      xvYCC2RGB_YCbCr_EXP_ONLY;
    /**<  Bound to change - not used for now */
    u32      xvYCC2RGB_YCbCr_FULLRANGE;
    /**<  Bound to change - not used for now */
    u32      xvYCC2RGB_YCbCr_SW_OVR;
    /**<  Bound to change - not used for now */
    u32      xvYCC2RGB_YCbCr_XVYCCSEL;
    /**<  Bound to change - not used for now */
    Vps_Hdmi_xvYCC2RGB_YCbCr_coEffs  *xvYCC2RGB_YCbCr_Coefs;
    /**<  Bound to change - not used for now */
    Vps_Hdmi_xvYCC2RGB_YCbCr_coEffs  space4xvYCC2RGB_YCbCr_Coefs;
    /**<  Bound to change - not used for now */
}Vps_HdmiCSC_xvYCC_Ctrl;


/**
 *  struct Vps_HdmiCSC_YCbCr2RGB_Ctrl
 *  \brief Defines the configurable parameters color space converter. YCbCr to
 *         RGB. Corresponds to XVYCC2RGB_CTL register
 */
typedef struct
{
    u32  enableRngExp;
    /**<  A non-zero value configures the CSC module in expansion only mode. */
    u32  enableFullRngExp;
    /**<  A non-zero enables xvYCC full range expansion. */
    u32  customCoEff;
    /**<  A non-zero value configure to use custom coefficients -
              Contents of structure Vps_HdmiCSC_YCbCr2RGB_coEff will be used */
    u32  srcCsSel;
    /**<  Select source color space - xvYCC if non-zero, YCbCr otherwise */
    Vps_HdmiCSC_YCbCr2RGB_coEff  coEff;
    /**<  Custom coefficients- will be used only when customCoEff is non-zero */
}Vps_HdmiCSC_YCbCr2RGB_Ctrl;


/**
 *  struct Vps_HdmiDitherConfig
 *  \brief Defines the configurable parameters dithering module.
 *         Note that members names do not follow coding guidelines to enable
 *         easy co-relation between members register names used in
 *         specifications
 */
typedef struct
{
    u32  M_D2;
    /**< Dither round option, non-zero value enables it, disabled otherwise.
         refer peripheral documentation for details */
    u32  UP2;
    /**< Dither round option, non-zero value  enables it, disabled otherwise.
         refer peripheral documentation for details */
    u32  STR_422_EN;
    /**< Color space 422 for dithering, non-zero value enables it, disabled
         otherwise. refer peripheral documentation for details */
    u32  D_BC_EN;
    /**< Addition of random numbers on BLUE channel, non-zero value enables it,
         disabled otherwise. refer peripheral documentation for details */
    u32  D_GC_EN;
    /**< Addition of random numbers on GREEN channel, non-zero value enables it,
         disabled otherwise. refer peripheral documentation for details */
    u32  D_RC_EN;
    /**< Addition of random numbers on RED channel, non-zero value enables it,
         disabled otherwise. refer peripheral documentation for details */
    u32  DRD;
    /**< Addition of random numbers and trucate LSb 2 bits, non-zero value
         enables it,  disabled otherwise.
         refer peripheral documentation for details */
}Vps_HdmiDitherConfig;


/**
 *  \brief Defines the configurable parameters of in data path of HDMI block.
 *         Pleaser refer HDMI peripheral document ion for details.
 *         For enabled sub-block might require additional configurations, unless
 *         documented otherwise
 */
typedef struct
{
    u32                          upSamplerEnable;
    /**<  Enable up sampler - convert 422 to 444 - non-zero value to enable,
              in bypass otherwise */
    u32                          cscYCbCr2RGBEnable;
    /**<  Enable color space converter - Converts YCbCr to RGB
              non-zero value to enable, in bypass otherwise - If enabled,
              required to  supply CSC configuration in cscYCbCr2RGBConfig */
    Vps_HdmiCSC_YCbCr2RGB_Ctrl      cscYCbCr2RGBConfig;
    /**<  Configurations for color space converter */
    u32      rangeExpRGBEnable;
    /**<  Enable range expansion of pixel data from 16-235 to 0-255
              non-zero value enables, disabled otherwise */
    u32      cscRGB2YCbCrEnable;
    /**<  Enable RGB to YCbCr color space converter. A non-zero value enables,
          disabled otherwise.
          If enabled, required to supply CSC configuration in */
    /* TBD coefficients for RGB to YCbCr - supporting source requires to be
       updated */
    u32      csConvertStandard;
    /**<  Specifies the color space standard to be used for color space
              conversions. non-zero value would configure to use BT.709,
              BT.601 otherwise.
              If custom co-efficients are not specified, BT.709 or BT.601
              standard co-efficients would be used - VID_CTRL.CSCSEL */
    u32      rangeCompEnable;
    /**<  A non-zero value enables range compression.
          (for RGB, YCbCr) 0-255 to 16-235 */
    u32      downSamplerEnable;
    /**<  Enable up down sampler/decimation - convert 444 to 422 -
          non-zero value enable, in bypass otherwise */
    u32      rangeClipEnable;
    /**<  Enable range clipping, from 16 to 235 (RGB and Y)/240 (CbCr).
          A non-zero value enables it, disabled otherwise.
          When enabled, clipColorSpace should be configured to select the output
          color space of this block */
    u32      clipColorSpace;
    /**<  Specifies output color space of the clipper, non-zero value for YCbCr,
          RGB otherwise */
    u32      ditherEnable;
    /**<  Configures dithering, non-zero value enables it, bypass otherwise. If
              enabled you would require to configure the dithering options
              via ditherConfig */
    Vps_HdmiDitherConfig            ditherConfig;
    /**<  Specify dithering configurations */

    u32                          outputWidth;
    /**<  Specifies the number of bits per channel that would sent out.
              If dithering is not enabled, the output would be truncated to the
              width specified here.
              Supported values are defined in #Vps_HdmiBitsPerChannel */
    /* Vps_HdmiCSC_xvYCC_Ctrl       xvYCCCtrl;
      Reserved for now Control xvYCC to RGB conversions TBD Not clear as of now
      */
}Vps_HdmiCoreDataPathConfig;


/**
 *  \brief Reserved for future use. Not used
 */
typedef struct
{
    u32      memNotUSed;
    /**< Not supported for now - defined for future use */
}Vps_HdmiCecInitiatorConfig;

/**
 *  \brief Reserved for future use. Not used
 */
typedef struct
{
    u32      memNotUSed;
    /**< Not supported for now - defined for future use */
}Vps_HdmiCecFollowerConfig;


/**
 *  \brief DE Generation configurations
 *
 *         Defines the delays between hSync, vSync and the actual display area.
 *         Note that members names do not follow coding guidelines to enable
 *         easy co-relation between members register names used in specification
 *         As these members have 1 to relation with the register names.
 */
typedef struct
{
    u32      DE_DLY;
    /**<  Delay between rising edge hSync and display of first column */
    u32      DE_TOP;
    /**<  Delay between rising edge vSync and display of first row */
    u32      DE_CNTL;
    /**<  Lower byte of coloumn count per row in active display region
              Only LS byte is used */
    u32      DE_CNTH;
    /**<  Higher nibble of column count per row in active display region
              Only LS nibble is used */
    u32      DE_LINL;
    /**<  Lower byte of row count in active display region
              Only LS byte is used */
    u32      DE_LINH;
    /**<  Higher byte of row count in active display region
              Only LS 3 bits is used */
}Vps_HdmiCoreDEDelayConfig;


/**
 *  \brief Defines configureable parameters of core input signals
 *
 *         Defines the configurable parameters of input video signal to core.
 *         Note that members names do not follow coding guidelines to enable
 *         easy co-relation between members register names used in specification
 */
typedef struct
{
    u32                      dataBusWidth;
    /**<  Specifies the width of the core input bus per channel, minimum would
          8 bit/pixel and maximum would be 10 bits/pixel. i.e. Valid values are
          VPS_HAL_HDMI_8_BITS_CHAN_WIDTH and VPS_HAL_HDMI_10_BITS_CHAN_WIDTH
          Defined in #Vps_HdmiBitsPerChannel */
    u32                      syncGenCfg;
    /**<  Configure vSync, hSync and DE source/generation.
          Supported values are defined in #Vps_HdmiCoreSyncGenCfg */
    Vps_HdmiCoreDEDelayConfig   deDelayCfg;
    /**<  When generating DE, i.e. when syncGenCfg == VPS_HAL_HDMI_SYNC_GENERATE
          DE delay parameter requires to be configured.
          Applications are expected to use this member to configure DE delay
          parameters */
    u32                      edgePol;
    /**<  A non-zero value configure to latch input on rising edge, falling edge
              otherwise - Recommended to use default -corresponds to SYS_CTRL1*/

}Vps_HdmiCoreInputConfig;


/**
 *  \brief Defines the configurable parameters of wrapper.
 */
typedef struct
{
    u32  debounceRecvDetect;
    /**<  Receiver Detect debounce value. A value between 1 to 63 inclusive
               of both end point, determines size of the glitch filter
               - HDMI_WP_DEBOUNCE.RXDET */
    u32  debounceRecvSens;
    /**<  Receiver sense/Line5Short debounce value. A value between 1 to 63
              inclusive of both end point, determines size of glitch filter
              - HDMI_WP_DEBOUNCE.LINE5VSHORT */
    u32  isSlaveMode;
    /**<  Slave / Master mode of operation. Only SLAVE mode supported.
              A non-zero value configure Wrapper in slave mode, MASTER mode
              otherwise.
              In Slave mode, the wrapper would required to be provided with
              Pixel Clock, Data Enable, Synchronization signal and pixel data.
              In Master mode only pixel data is to be provided.
              - HDMI_WP_VIDEO_CFG.MODE*/
    u32  packMode;
    /**<  In case the incoming video data is not packed, could be packed as
              described by the enum - HDMI_WP_VIDEO_CFG.PACKING_MODE .
              Valid values are defined in #Vps_HdmiWpPackingMode */
    u32  isVSyncPolInverted;
    /**<  A non-zero value inverts the polarity of VSync thats is going to the
              core. Polarity remains un-changed otherwise Applicable for both
              MASTER/SLAVE mode HDMI_WP_VIDEO_CFG.CORE_VSYNC_INV */
    u32  isHSyncPolInverted;
    /**<  A non-zero value inverts the polarity of HSync thats is going to the
              core. Polarity remains un-changed otherwise. Applicable for both
              MASTER/SLAVE mode HDMI_WP_VIDEO_CFG.CORE_HSYNC_INV */

    u32  width;
    /**<  Not supported - For future. Used in MASTER mode, Wrappers timing
              generator is used, selects deep color - HDMI_WP_VIDEO_CFG.MODE.
              Supported values are defined in #Vps_HdmiBitsPerChannel */
    u32  vSyncPol;
    /**<  Not Supported - For future. Used in MASTER mode,  TRUE for active
              high, active LOW otherwise */
    u32  hSyncPol;
    /**<  Not Supported - For future. Used in MASTER mode,  TRUE for active
              high, active LOW otherwise */
    u32  hbp;
    /**<  Not Supported - Used in MASTER mode, Horizontal back porch */
    u32  hfp;
    /**<  Not Supported - Used in MASTER mode, Horizontal front porch */
    u32  hsw;
    /**<  Not Supported - Used in MASTER mode, Horizontal sync pulse width */
    u32  vbp;
    /**<  Not Supported - Used in MASTER mode, Vertical back porch */
    u32  vfp;
    /**<  Not Supported - Used in MASTER mode, Vertical front porch */
    u32  vsw;
    /**<  Not Supported - Used in MASTER mode, Vertical sync pulse width */
    u32  cecClockDivisor;
    /**<  Not Supported - For future use, When CEC is supported
              Specifies the divisor that would be applied for clock derived
              from DDC_CLOCK. */
}Vps_HdmiWpConfig;


/*******************************************************************************
*                       Information Packets Specifies                          *
*******************************************************************************/
/**
 *  \brief Defines the transmitted video color space.
 *         Initializing the enumerations explicitly, so that these values could
 *         directly be used to configure register.
 */
typedef enum
{
    VPS_HAL_HDMI_AVIINFO_OP_CS_RGB          = 0x0,
    /**< RGB color space is used */
    VPS_HAL_HDMI_AVIINFO_OP_CS_YCbCr_422    = 0x1,
    /**< YCbCr 422 color space is used */
    VPS_HAL_HDMI_AVIINFO_OP_CS_YCbCr_444    = 0x2,
    /**< YCbCr 444 color space is used */
    VPS_HAL_HDMI_AVIINFO_OP_CS_MAX
    /**< enum guard */
}Vps_HdmiAviInfoOpCs;


/**
 *  \brief Configure if active information is present or not
 */
typedef enum
{
    VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATION_NO    = 0x0,
    /**< Active aspect ratio not specified */
    VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATION_YES   = 0x1
    /**< Active aspect ratio specified */
}Vps_HdmiAviInfoActiveRatio;


/**
 *  \brief Defines the Bar information
 */
typedef struct
{
    u32  barInfoValid;
    /**< Specify if the BAR information is valid or not.
         0x01 - Vertical Bar Info is valid
         0x02 - Horizontal Bar Info is valid
         0x03 - Both Vertical & Horizontal is valid */
    u32  topBar;
    /**< Line number of End of Top Bar - LSb 16 bits are considered */
    u32  bottomBar;
    /**< Line number of End of Bottom Bar - LSb 16 bits are considered  */
    u32  leftBar;
    /**< Line number of End of Left Bar - LSb 16 bits are considered  */
    u32  rightBar;
    /**< Line number of End of Right Bar - LSb 16 bits are considered  */
}Vps_HdmiAviInfoBarInfo;


/**
 *  \brief Configure scan information
 */
typedef enum
{
    VPS_HAL_HDMI_AVIINFO_SCAN_INFO_NONE         = 0x0,
    /**< No scan information */
    VPS_HAL_HDMI_AVIINFO_SCAN_INFO_OVERSCAN     = 0x1,
    /**< Overscan - For Television */
    VPS_HAL_HDMI_AVIINFO_SCAN_INFO_UNDERSCAN    = 0x2,
    /**< Overscan - For computer monitors */
    VPS_HAL_HDMI_AVIINFO_SCAN_INFO_MAX          = 0x3
    /**< MAximum scan info guard */
}Vps_HdmiAviInfoScanInfo;


/**
 *  \brief Colorimetry configurations
 */
typedef enum
{
    VPS_HAL_HDMI_AVIINFO_COLORIMETRY_NONE       = 0x0,
    /**< No Colorimetry information */
    VPS_HAL_HDMI_AVIINFO_COLORIMETRY_BT601      = 0x1,
    /**< BT.601 - For standard television */
    VPS_HAL_HDMI_AVIINFO_COLORIMETRY_BT709      = 0x2,
    /**< BT.709 - For Advanced / High Definition television  */
    VPS_HAL_HDMI_AVIINFO_COLORIMETRY_MAX        = 0x3
    /**< enum guard */
}Vps_HdmiAviInfoColorimetry;


/**
 *  \brief Aspect ratio of the video being sent
 */
typedef enum
{
    VPS_HAL_HDMI_AVIINFO_ASPECTRATIO_NONE       = 0x0,
    /**< No Aspect ration specified */
    VPS_HAL_HDMI_AVIINFO_ASPECTRATIO_4_3        = 0x1,
    /**< 4:3 */
    VPS_HAL_HDMI_AVIINFO_ASPECTRATIO_16_9       = 0x2,
    /**< 16:9 */
    VPS_HAL_HDMI_AVIINFO_ASPECTRATIO_MAX        = 0x3
    /* Enum guard */
}Vps_HdmiAviInfoAspectRatio;


/**
 *  \brief Active Aspect ratio
 */
typedef enum
{
    VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATIO_SAME    = 0x8,
    /**< Same as the aspect ration specified AVI_InfoFrame.M1, M0 */
    VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATIO_4_3     = 0x9,
    /**< 4:3 */
    VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATIO_16_9    = 0xA,
    /**< 16:9 */
    VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATIO_14_9    = 0xB,
    /**< 14:9 */
    VPS_HAL_HDMI_AVIINFO_ACTIVE_ASPECTRATIO_MAX     = 0xC
    /* Enum Guard */
}Vps_HdmiAviInfoActiveAspectRatio;


/**
 *  \brief NON-Uniform scaling on horizontal/vertical plane
 */
typedef enum
{
    VPS_HAL_HDMI_AVIINFO_NONUNIFORM_SCALING_NONE    = 0x0,
    /**< No non-uniform scaling performed AVI_InfoFrame.SC1, SC0 */
    VPS_HAL_HDMI_AVIINFO_NONUNIFORM_SCALING_HORI    = 0x1,
    /**< Non-uniform scaled on horizontal plane */
    VPS_HAL_HDMI_AVIINFO_NONUNIFORM_SCALING_VER     = 0x2,
    /**< Non-uniform scaled on vertical plane */
    VPS_HAL_HDMI_AVIINFO_NONUNIFORM_SCALING_MAX     = 0x3
    /* Enum guard */
}Vps_HdmiAviInfoNonUniformScaling;


/**
 *  \brief AVI Info frame configurations
 *
 *         Using AVI info frame applications could configure sink to decode
 *         video data correctly.
 *         These information would communicated to an attached sink via a AVI
 *         info frame
 */
typedef struct
{
    u32                  outputCs;
    /**< Configure the color space that would be transmitted.
         AVI_InfoFrame.Y1 and Y0
         Valid values are defined by #Vps_HdmiAviInfoOpCs */
    u32                  useActiveAspectRatio;
    /**< Active aspect ratio specified, AVI_InfoFrame.A0
         Valid values are defined by #Vps_HdmiAviInfoActiveRatio */
    Vps_HdmiAviInfoBarInfo  barInfo;
    /**< Configure the width of the frame
         AVI_InfoFrame.B1, B0, Byte 6, 7, 8, 9, 10, 11, 12 and 13 */
    u32                  scanInfo;
    /**< Configure scan information AVI_InfoFrame.S1, S0
         - such OverScan/UnderScan/...
         Valid values are defined by #Vps_HdmiAviInfoScanInfo */
    u32                  colorimetryInfo;
    /**< Configure colorimetry information AVI_InfoFrame.C1, C0
         Valid values are defined by #Vps_HdmiAviInfoColorimetry */
    u32                  aspectRatio;
    /**< Configure the aspect ration AVI_InfoFrame.M1, M0
         Valid values are defined by #Vps_HdmiAviInfoAspectRatio */
    u32                  activeAspectRatio;
    /**< Configure the aspect ration AVI_InfoFrame.R3, R2, R1, R0
         Valid values are defined by #Vps_HdmiAviInfoActiveAspectRatio */
    u32                  itContentPresent;
    /**< IT content present, TRUE marks it FALSE resets it AVI_InfoFrame.ITC */
    u32                  extColorimetry;
    /**< Extended colorimetry AVI_InfoFrame.EC2, EC1, EC0 TBD use enum*/
    u32                  quantizationRange;
    /**< Quantization range  TBD use enum */
    u32                  nonUniformSc;
    /**< If non-uniform scaling done, specify it AVI_InfoFrame.SC1, SC0
         Valid values are defined by #Vps_HdmiAviInfoNonUniformScaling */
    u32                  formatIdentier;
    /**< CEA861 Video format identification code  TBD use enum
         AVI_InfoFrame.VIC6, VIC5, VIC4, VIC3, VIC2, VIC1 and VIC0 */
    /**< Pixel repetition will be determined by the HAL and programmed
         accordingly */
}Vps_HdmiAviInfoFrameCfg;


/**
 *  \brief Info Frame configurations.
 *
 *         Defines the configurable parameters of info frames that would be
 *         sent sink. To be used to instruct sinks on video and audio
 *         configurations
 */
typedef struct
{
    u32                      useAviInfoData;
    /**< A non-zero value configures HAL to pick up the data from aviData to
         frame the AVI info frame which would be sent sink periodically */
    Vps_HdmiAviInfoFrameCfg     aviData;
    /**< Instruct sink on video parameters such as, aspect ratio, non-uniform
         scaling, etc... */
    /* When audio, GAMUT is supported, add audio info packet data, GAMUT packet
       data, SPD packet (Source Product Description ) here */
}Vps_HdmiInfoFrameCfg;


/*******************************************************************************
*                     Information Packets Specifies Ends                       *
*******************************************************************************/


/**
 *  struct Vps_HdmiCoreDataPathConfig
 *  \brief HDMI Peripheral configurations
 *
 *         Defines the configurable parameters of HDMI block.
 *         Pleaser refer HDMI peripheral documentation for details. If a block
 *         such as wrapper (WP), Data Enable (DE), etc... dose not required to
 *         be configured, applications to choose to use the default config
 *         values, or last configured values.
 */
typedef struct
{
    u32                          useDcDisplayMode;
    /**<  A non-zero value specifies to use the display controller mode supplied
          by the below member, otherwise use the last configurations
          (initially it would be the default) */
    Vps_HdmiModeId               displayMode;
    /**<  Display Controllers / venc mode of operations.
          This Will be used to extract information like resolution, scan type
          and frame rate.
          \par CAUTION When custom modes are used, the pixel repetitions
                        requires to be calculated TBD */
    u32                          useWpConfig;
    /**<  A non-zero value specifies to use the wrapper config supplied by the
          below member, otherwise use the last configurations
          (initially it would be the default) */
    Vps_HdmiWpConfig                wpConfig;
    /**<  Wrapper configurations - Supports SLAVE mode only */
    u32                          useCoreIpConfig;
    /**<  A non-zero value specifies to use the input configuration supplied by
              the member below, otherwise use the last configurations
              (initially it would be the default) */
    Vps_HdmiCoreInputConfig         coreIpConfig;
    /**<  Input signal configurations for the core - Recommended to use the
              the default values. As the connections between core and wrapper is
              hardwired. Defined here to enhance portability - Please refer the
              Vps_HdmiCoreInputConfig for details on programmable parameters
              */
    u32                          useCorePathConfig;
    /**<  A non-zero value specifies to use the path configuration supplied by
          the below member, otherwise use the last configurations
          (initially it would be the default) */
    Vps_HdmiCoreDataPathConfig      corePathConfig;
    /**<  Configurable parameters of in data path of HDMI block */
    u32                          useInfoFrameConfig;
    /**< A non-zero value configures the HAL to use the AVI Info frame details
         specified in infoFrameCfg. Defaults are used otherwise.*/
    Vps_HdmiInfoFrameCfg            infoFrameConfig;
    /**<  Configure the packets that would be sent to the sink */
    Vps_Hdmi_CbFxn           cbOnHPD;
    /**<  Application supplied function that would called when an HDMI
              capable sink/receiver is detected.
              Additionally note that - Once a sink (repeater) is detected, this
              callback could be called multiple times to indicate addition/
              deletion of sink attached to the repeater. */
    HDMI_Ptr                         cbArgs;
    /**<  Arguments that would be passed back to application on occurrence
              of hot plug detect */
    Vps_Hdmi_CbFxn           cbOnErr;
    /**<  Reserved for future use. Error callback - will be called on
              occurrence of error defined in cbOnErrors */
    int                           cbOnErrors;
    /**<  Reserved for future use. On following errors the error callback
              would be called */
    u32                          hdcpEnable;
    /**<  Reserved for future use. TRUE would enable HDCP, HDCP would be
              disabled otherwise */
    u32                          cecEnable;
    /**<  Reserved for future use. TRUE would enable CEC, CEC would be
              disabled otherwise */
    u32                          cecInitiator;
    /**<  Reserved for future use. Applicable only when enableCEC is TRUE
              TRUE would configure this port as a initiator, follower otherwise.
              */
    Vps_HdmiCecInitiatorConfig      cecInitiateConfig;
    /**<  Reserved for future use. */
    Vps_HdmiCecFollowerConfig       cecFollowerConfig;
    /**<  Reserved for future use. */
}Vps_HdmiConfigParams;


/* ========================================================================== */
/*                         EDID Specific Structures                           */
/* ========================================================================== */


/**
 *  \brief Configurations required to read EDID.
 *
 *         Defines the parameters required to read EDID from a sink.
 *         Note that this function just READS EDID content. Caller will have to
 *         parse the EDID data for sinks capabilities
 *         Read starting address would be computed based on the formula below
 *              = segmentPtr * 128 + offset;
 *         e.g. for Block 0x0, to read one block starting at address 0x0
 *         slaveAddress = 0xA0
 *         segmentPtr   = 0x0
 *         offset       = 0x0
 *         noOfBytes    = 0x80
 */
typedef struct
{
    u32      slaveAddress;
    /**<  DDC Slave to address. 7 bit slave address, LSByte, ignoring LSbit
              i.e. bits 1-7 is used.
              0xA0 should be passed to READ EDID PROM
              Relates to DDC_ADDR register of core - NO error check performed*/
    u32      segmentPtr;
    /**<  Segment to address of the addressed slave. Segment that you would
              want read. LS byte is used i.e. bit0-7 Relates to DDC_SEGM
              NO error check performed for this param */
    u32      offset;
    /**<  Offset with in the segment that you would want to read.
              LSByte is used i.e. bit0-7, relates to DDC_OFFSET
              NO error check performed for this param */
    u32      noOfBytes;
    /**<  Number of bytes to be read. Should not greater than 1024 bytes
              per request */
    HDMI_Ptr         bufferPtr;
    /**<  Pointer to a buffer that would store the read the data. Should be
              big enough to accommodate the request number of bytes. */
    u32      noOfBytesRead;
    /**< [OUT] Specifies the number of bytes that has been read. */
}Vps_HdmiEdidReadParams;


#ifdef __cplusplus
}
#endif

#endif /* _VPS_HDMICFG_H */

/* @} */
