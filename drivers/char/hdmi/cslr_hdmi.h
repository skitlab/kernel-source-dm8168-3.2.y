/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2010 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/

/*******************************************************************************
 * file: cslr_vps_hdmi.h
 *
 * Brief: This file contains the Register Description for vps_hdmi
 *
 * History
 *   1. Initial version              H, Jayakrishnan     210809
 ******************************************************************************/
#ifndef _CSLR_VPS_HDMI_H_
#define _CSLR_VPS_HDMI_H_


/**************************************************************************\
 * Wrapper Register Overlay Structure
\**************************************************************************/

/* Minimum unit = 4 bytes */
typedef struct  {
    volatile int REVISION;
    volatile int RSVD[3];
    volatile int SYSCONFIG;
    volatile int RSVD1[3];
    volatile int IRQ_EOI;
    volatile int IRQSTATUS_RAW;
    volatile int IRQSTATUS;
    volatile int IRQENABLE_SET;
    volatile int IRQENABLE_CLR;
    volatile int IRQWAKEEN;
    volatile int RSVD2[2];
    volatile int PWR_CTRL;
    volatile int DEBOUNCE;
    volatile int RSVD3[2];
    volatile int VIDEO_CFG;
    volatile int RSVD4[3];
    volatile int VIDEO_SIZE;         /* In spec this reg is deleted */
    volatile int RSVD5[1];
    volatile int VIDEO_TIMING_H;
    volatile int VIDEO_TIMING_V;
    volatile int CLK;
    volatile int RSVD6[3];
    volatile int AUDIO_CFG;
    volatile int AUDIO_CFG2;
    volatile int AUDIO_CTRL;
    volatile int AUDIO_DATA;
    volatile int DEBUG_CFG;
    volatile int DEBUG_DATA;
} CSL_Hdmi_WpRegs;

/**************************************************************************\
 * IP Core Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile int VND_IDL;
    volatile int VND_IDH;
    volatile int DEV_IDL;
    volatile int DEV_IDH;
    volatile int DEV_REV;
    volatile int SRST;
    volatile int RSVD1;
    volatile int RSVD2;
    volatile int SYS_CTRL1;
    volatile int SYS_STAT;
    volatile int SYS_CTRL3;
    volatile int RSVD3;
    volatile int SYS_CTRL4;
    volatile int DCTL;
    volatile int RSVD4;
    volatile int HDCP_CTRL;
    volatile int BKSV_0;
    volatile int BKSV_1;
    volatile int BKSV_2;
    volatile int BKSV_3;
    volatile int BKSV_4;
    volatile int AN__0;
    volatile int AN__1;
    volatile int AN__2;
    volatile int AN__3;
    volatile int AN__4;
    volatile int AN__5;
    volatile int AN__6;
    volatile int AN__7;
    volatile int AKSV__0;
    volatile int AKSV__1;
    volatile int AKSV__2;
    volatile int AKSV__3;
    volatile int AKSV__4;
    volatile int RI1;
    volatile int RI2;
    volatile int RI_128_COMP;
    volatile int I_CNT;
    volatile int RI_STAT;
    volatile int RI_CMD;
    volatile int RI_START;
    volatile int RI_RX_L;
    volatile int RSVD0;
    volatile int RI_DEBUG;
    volatile int RSVD11[6];
    volatile int DE_DLY;
    volatile int DE_CTRL;
    volatile int DE_TOP;
    volatile int RSVD22;
    volatile int DE_CNTL;
    volatile int DE_CNTH;
    volatile int DE_LINL;
    volatile int DE_LINH;
    volatile int HRES_L;
    volatile int HRES_H;
    volatile int VRES_L;
    volatile int VRES_H;
    volatile int IADJUST;
    volatile int POL_DETECT;
    volatile int HBIT_2HSYNC1;
    volatile int HBIT_2HSYNC2;
    volatile int FLD2_HS_OFSTL;
    volatile int FLD2_HS_OFSTH;
    volatile int HWIDTH1;
    volatile int HWIDTH2;
    volatile int VBIT_TO_VSYNC;
    volatile int VWIDTH;
    volatile int VID_CTRL;
    volatile int VID_ACEN;
    volatile int VID_MODE;
    volatile int VID_BLANK1;
    volatile int VID_BLANK2;
    volatile int VID_BLANK3;
    volatile int DC_HEADER;
    volatile int VID_DITHER;
    volatile int RGB2XVYCC_CT;
    volatile int R2Y_COEFF_LOW;
    volatile int R2Y_COEFF_UP;
    volatile int G2Y_COEFF_LOW;
    volatile int G2Y_COEFF_UP;
    volatile int B2Y_COEFF_LOW;
    volatile int B2Y_COEFF_UP;
    volatile int R2CB_COEFF_LOW;
    volatile int R2CB_COEFF_UP;
    volatile int G2CB_COEFF_LOW;
    volatile int G2CB_COEFF_UP;
    volatile int B2CB_COEFF_LOW;
    volatile int B2CB_COEFF_UP;
    volatile int R2CR_COEFF_LOW;
    volatile int RSVD33;
    volatile int G2CR_COEFF_LOW;
    volatile int G2CR_COEFF_UP;
    volatile int B2CR_COEFF_LOW;
    volatile int B2CR_COEFF_UP;
    volatile int RGB_OFFSET_LOW;
    volatile int RGB_OFFSET_UP;
    volatile int Y_OFFSET_LOW;
    volatile int Y_OFFSET_UP;
    volatile int CBCR_OFFSET_LOW;
    volatile int CBCR_OFFSET_UP;
    volatile int RSVD44[7];
    volatile int INTR_STATE;
    volatile int INTR1;
    volatile int INTR2;
    volatile int INTR3;
    volatile int INTR4;
    volatile int INT_UNMASK1;
    volatile int INT_UNMASK2;
    volatile int INT_UNMASK3;
    volatile int INT_UNMASK4;
    volatile int INT_CTRL;
    volatile int RSVD5[6];
    volatile int TMDS_CCTRL;
    volatile int RSVD6;
    volatile int TMDS_CTRL;
    volatile int TMDS_CTRL2;
    volatile int TMDS_CTRL3;
    volatile int TMDS_CTRL4;
    volatile int RSVD7[10];
    volatile int XVYCC2RGB_CTL;
    volatile int Y2R_COEFF_LOW;
    volatile int Y2R_COEFF_UP;
    volatile int CR2R_COEFF_LOW;
    volatile int CR2R_COEFF_UP;
    volatile int CB2B_COEFF_LOW;
    volatile int CB2B_COEFF_UP;
    volatile int CR2G_COEFF_LOW;
    volatile int CR2G_COEFF_UP;
    volatile int CB2G_COEFF_LOW;
    volatile int CB2G_COEFF_UP;
    volatile int YOFFSET1_LOW;
    volatile int YOFFSET1_UP;
    volatile int OFFSET1_LOW;
    volatile int OFFSET1_MID;
    volatile int OFFSET1_UP;
    volatile int OFFSET2_LOW;
    volatile int OFFSET2_UP;
    volatile int DCLEVEL_LOW;
    volatile int DC_LEVEL_UP;
    volatile int RSVD9[40];
    volatile int SHA_CTRL;
    volatile int RSVD10[31];
    volatile int DDC_MAN;
    volatile int DDC_ADDR;
    volatile int DDC_SEGM;
    volatile int DDC_OFFSET;
    volatile int DDC_COUNT1;
    volatile int DDC_COUNT2;
    volatile int DDC_STATUS;
    volatile int DDC_CMD;
    volatile int DDC_DATA;
    volatile int DDC_FIFOCNT;
    volatile int RSVD12[3];
    volatile int EPST;
    volatile int EPCM;
    volatile int RSVD13[4];
    volatile int REG_BANK0__1;
    volatile int GAMUT_HEADER1;
    volatile int GAMUT_HEADER2;
    volatile int GAMUT_HEADER3;
    volatile int GAMUT_DBYTE[28];
    volatile int RSVD15[34];
    volatile int ACR_CTRL;
    volatile int FREQ_SVAL;
    volatile int N_SVAL1;
    volatile int N_SVAL2;
    volatile int N_SVAL3;
    volatile int CTS_SVAL1;
    volatile int CTS_SVAL2;
    volatile int CTS_SVAL3;
    volatile int CTS_HVAL1;
    volatile int CTS_HVAL2;
    volatile int CTS_HVAL3;
    volatile int RSVD16[8];
    volatile int AUD_MODE;
    volatile int SPDIF_CTRL;
    volatile int RSVD17[2];
    volatile int HW_SPDIF_FS;
    volatile int SWAP_I2S;
    volatile int RSVD18;
    volatile int SPDIF_ERTH;
    volatile int I2S_IN_MAP;
    volatile int I2S_IN_CTRL;
    volatile int I2S_CHST0;
    volatile int I2S_CHST1;
    volatile int I2S_CHST2;
    volatile int I2S_CHST4;
    volatile int I2S_CHST5;
    volatile int ASRC;
    volatile int I2S_IN_LEN;
    volatile int RSVD20[10];
    volatile int HDMI_CTRL;
    volatile int AUDIO_TXSTAT;
    volatile int RSVD21[2];
    volatile int AUD_PAR_BUSCLK_1;
    volatile int AUD_PAR_BUSCLK_2;
    volatile int AUD_PAR_BUSCLK_3;
    volatile int RSVD222[6];
    volatile int TEST_TXCTRL;
    volatile int DPD;
    volatile int PB_CTRL1;
    volatile int PB_CTRL2;
    volatile int AVI_TYPE;
    volatile int AVI_VERS;
    volatile int AVI_LEN;
    volatile int AVI_CHSUM;
    volatile int AVI_DBYTE[15];
    volatile int RSVD23[13];
    volatile int SPD_TYPE;
    volatile int SPD_VERS;
    volatile int SPD_LEN;
    volatile int SPD_CHSUM;
    volatile int SPD_DBYTE[27];
    volatile int RSVD24;
    volatile int AUDIO_TYPE;
    volatile int AUDIO_VERS;
    volatile int AUDIO_LEN;
    volatile int AUDIO_CHSUM;
    volatile int AUDIO_DBYTE[27];
    volatile int RSVD25;
    volatile int MPEG_TYPE;
    volatile int MPEG_VERS;
    volatile int MPEG_LEN;
    volatile int MPEG_CHSUM;
    volatile int MPEG_DBYTE[27];
    volatile int RSVD26;
    volatile int GEN_DBYTE[31];
    volatile int CP_BYTE1;
    volatile int GEN2_DBYTE[31];
    volatile int CEC_ADDR_ID;
    volatile int CEC_DEV_ID;
    volatile int CEC_SPEC;
    volatile int CEC_SUFF;
    volatile int CEC_FW;
    volatile int CEC_DBG_0;
    volatile int CEC_DBG_1;
    volatile int CEC_DBG_2;
    volatile int CEC_DBG_3;
    volatile int CEC_TX_INIT;
    volatile int CEC_TX_DEST;
    volatile int RSVD28[4];
    volatile int CEC_SET_UP;
    volatile int CEC_TX_COMMAND;
    volatile int CEC_TX_OPERAND[15];
    volatile int CEC_TRANSMIT_DATA;
    volatile int RSVD29[2];
    volatile int CEC_CA_7_0;
    volatile int CEC_CA_15_8;
    volatile int CEC_INT_ENABLE_0;
    volatile int CEC_INT_ENABLE_1;
    volatile int CEC_INT_STATUS_0;
    volatile int CEC_INT_STATUS_1;
    volatile int RSVD30[4];
    volatile int CEC_RX_CONTROL;
    volatile int CEC_RX_COUNT;
    volatile int CEC_RX_CMD_HEADER;
    volatile int CEC_RX_COMMAND;
    volatile int CEC_RX_OPERAND[15];
} CSL_Hdmi_CoreRegs;


/**************************************************************************\
 * PHY Register Overlay Structure
\**************************************************************************/
typedef struct
{
    volatile const int TMDS_CNTL1;   /* Do not change this registers value */
    volatile int TMDS_CNTL2;
    volatile int TMDS_CNTL3;
    volatile int BIST_CNTL;          /* Disable BIST and disable DVI */
    volatile  int BIST_PATTERN; /* Do not change this registers value */
    volatile  int BIST_INST0;   /* Do not change this registers value */
    volatile  int BIST_INST1;   /* Do not change this registers value */
    volatile  int BIST_CONF0;   /* Do not change this registers value */
    volatile int TMDS_CNTL9;
    volatile  int TMDS_CNTL10;  /* Do not change this registers value */
    volatile int TEST_MUX_CTRL;
    volatile int CHIPID;
}CSL_Hdmi_PhyRegs;


/**************************************************************************\
 * PLL Controller Register Overlay Structure
\**************************************************************************/
typedef struct
{
    volatile int CONTROL;
    volatile int STATUS;
    volatile int GO;
    volatile int CONFIGURATION1;
    volatile int CONFIGURATION2;
    volatile int CONFIGURATION3;
    volatile int SSC_CONFIGURATION1;
    volatile int SSC_CONFIGURATION2;
    volatile int CONFIGURATION4;

}CSL_Hdmi_PllCtrlRegs;


/**************************************************************************\
 * Core Field Definition Macros
\**************************************************************************/

/* VND_IDL */
#define CSL_HDMI_VND_IDL_VND_ID_MASK     (0x000000FFu)
#define CSL_HDMI_VND_IDL_VND_ID_SHIFT    (0x00000000u)
#define CSL_HDMI_VND_IDL_VND_ID_RESETVAL (0x00000001u)

#define CSL_HDMI_VND_IDL_RESETVAL        (0x00000001u)

/* VND_IDH */
#define CSL_HDMI_VND_IDH_VND_ID_MASK     (0x000000FFu)
#define CSL_HDMI_VND_IDH_VND_ID_SHIFT    (0x00000000u)
#define CSL_HDMI_VND_IDH_VND_ID_RESETVAL (0x00000001u)

#define CSL_HDMI_VND_IDH_RESETVAL        (0x00000001u)

/* DEV_IDL */
#define CSL_HDMI_DEV_IDL_DEV_ID_MASK     (0x000000FFu)
#define CSL_HDMI_DEV_IDL_DEV_ID_SHIFT    (0x00000000u)
#define CSL_HDMI_DEV_IDL_DEV_ID_RESETVAL (0x00000036u)

#define CSL_HDMI_DEV_IDL_RESETVAL        (0x00000036u)

/* DEV_IDH */
#define CSL_HDMI_DEV_IDH_DEV_ID_MASK     (0x000000FFu)
#define CSL_HDMI_DEV_IDH_DEV_ID_SHIFT    (0x00000000u)
#define CSL_HDMI_DEV_IDH_DEV_ID_RESETVAL (0x00000097u)

#define CSL_HDMI_DEV_IDH_RESETVAL        (0x00000097u)

/* DEV_REV */
#define CSL_HDMI_DEV_REV_DEV_REV_MASK    (0x000000FFu)
#define CSL_HDMI_DEV_REV_DEV_REV_SHIFT   (0x00000000u)
#define CSL_HDMI_DEV_REV_DEV_REV_RESETVAL (0x00000036u)

#define CSL_HDMI_DEV_REV_RESETVAL        (0x00000036u)

/* SRST */
#define CSL_HDMI_SRST_FIFORST_MASK       (0x00000002u)
#define CSL_HDMI_SRST_FIFORST_SHIFT      (0x00000001u)
#define CSL_HDMI_SRST_FIFORST_RESETVAL   (0x00000000u)

#define CSL_HDMI_SRST_SWRST_MASK         (0x00000001u)
#define CSL_HDMI_SRST_SWRST_SHIFT        (0x00000000u)
#define CSL_HDMI_SRST_SWRST_RESETVAL     (0x00000000u)

#define CSL_HDMI_SRST_RESETVAL           (0x00000000u)

/* RSVD1 */
#define CSL_HDMI_RSVD1_RESETVAL          (0x00000019u)

/* RSVD2 */
#define CSL_HDMI_RSVD2_RESETVAL          (0x00000064u)

/* SYS_CTRL1 */
#define CSL_HDMI_SYS_CTRL1_VSYNC_MASK    (0x00000040u)
#define CSL_HDMI_SYS_CTRL1_VSYNC_SHIFT   (0x00000006u)
#define CSL_HDMI_SYS_CTRL1_VSYNC_RESETVAL (0x00000000u)

#define CSL_HDMI_SYS_CTRL1_VEN_MASK      (0x00000020u)
#define CSL_HDMI_SYS_CTRL1_VEN_SHIFT     (0x00000005u)
#define CSL_HDMI_SYS_CTRL1_VEN_RESETVAL  (0x00000001u)

#define CSL_HDMI_SYS_CTRL1_HEN_MASK      (0x00000010u)
#define CSL_HDMI_SYS_CTRL1_HEN_SHIFT     (0x00000004u)
#define CSL_HDMI_SYS_CTRL1_HEN_RESETVAL  (0x00000001u)

#define CSL_HDMI_SYS_CTRL1_BSEL_MASK     (0x00000004u)
#define CSL_HDMI_SYS_CTRL1_BSEL_SHIFT    (0x00000002u)
#define CSL_HDMI_SYS_CTRL1_BSEL_RESETVAL (0x00000001u)

#define CSL_HDMI_SYS_CTRL1_EDGE_MASK     (0x00000002u)
#define CSL_HDMI_SYS_CTRL1_EDGE_SHIFT    (0x00000001u)
#define CSL_HDMI_SYS_CTRL1_EDGE_RESETVAL (0x00000000u)

#define CSL_HDMI_SYS_CTRL1_PD_MASK       (0x00000001u)
#define CSL_HDMI_SYS_CTRL1_PD_SHIFT      (0x00000000u)
#define CSL_HDMI_SYS_CTRL1_PD_RESETVAL   (0x00000000u)

#define CSL_HDMI_SYS_CTRL1_RESETVAL      (0x00000034u)

/* SYS_STAT */
#define CSL_HDMI_SYS_STAT_VLOW_MASK      (0x00000080u)
#define CSL_HDMI_SYS_STAT_VLOW_SHIFT     (0x00000007u)
#define CSL_HDMI_SYS_STAT_VLOW_RESETVAL  (0x00000001u)

#define CSL_HDMI_SYS_STAT_RSEN_MASK      (0x00000004u)
#define CSL_HDMI_SYS_STAT_RSEN_SHIFT     (0x00000002u)
#define CSL_HDMI_SYS_STAT_RSEN_RESETVAL  (0x00000000u)

#define CSL_HDMI_SYS_STAT_HPD_MASK       (0x00000002u)
#define CSL_HDMI_SYS_STAT_HPD_SHIFT      (0x00000001u)
#define CSL_HDMI_SYS_STAT_HPD_RESETVAL   (0x00000000u)

#define CSL_HDMI_SYS_STAT_P_STABLE_MASK  (0x00000001u)
#define CSL_HDMI_SYS_STAT_P_STABLE_SHIFT (0x00000000u)
#define CSL_HDMI_SYS_STAT_P_STABLE_RESETVAL (0x00000000u)

#define CSL_HDMI_SYS_STAT_RESETVAL       (0x00000080u)

/* SYS_CTRL3 */
#define CSL_HDMI_SYS_CTRL3_CTL_MASK      (0x00000006u)
#define CSL_HDMI_SYS_CTRL3_CTL_SHIFT     (0x00000001u)
#define CSL_HDMI_SYS_CTRL3_CTL_RESETVAL  (0x00000000u)

#define CSL_HDMI_SYS_CTRL3_RESETVAL      (0x00000000u)

/* RSVD3 */
#define CSL_HDMI_RSVD3_RESETVAL          (0x00000097u)

/* SYS_CTRL4 */
#define CSL_HDMI_SYS_CTRL4_PLLF_MASK     (0x00000006u)
#define CSL_HDMI_SYS_CTRL4_PLLF_SHIFT    (0x00000001u)
#define CSL_HDMI_SYS_CTRL4_PLLF_RESETVAL (0x00000000u)

#define CSL_HDMI_SYS_CTRL4_PFEN_MASK     (0x00000001u)
#define CSL_HDMI_SYS_CTRL4_PFEN_SHIFT    (0x00000000u)
#define CSL_HDMI_SYS_CTRL4_PFEN_RESETVAL (0x00000000u)

#define CSL_HDMI_SYS_CTRL4_RESETVAL      (0x00000000u)

/* DCTL */
#define CSL_HDMI_DCTL_VID_BLANK_MASK     (0x00000004u)
#define CSL_HDMI_DCTL_VID_BLANK_SHIFT    (0x00000002u)
#define CSL_HDMI_DCTL_VID_BLANK_RESETVAL (0x00000000u)

#define CSL_HDMI_DCTL_AUD_MUTE_MASK      (0x00000002u)
#define CSL_HDMI_DCTL_AUD_MUTE_SHIFT     (0x00000001u)
#define CSL_HDMI_DCTL_AUD_MUTE_RESETVAL  (0x00000000u)

#define CSL_HDMI_DCTL_HDCP_SEL_MASK      (0x00000001u)
#define CSL_HDMI_DCTL_HDCP_SEL_SHIFT     (0x00000000u)
#define CSL_HDMI_DCTL_HDCP_SEL_RESETVAL  (0x00000000u)

#define CSL_HDMI_DCTL_RESETVAL           (0x00000000u)

/* RSVD4 */
#define CSL_HDMI_RSVD4_RESETVAL          (0x00000097u)

/* HDCP_CTRL */
#define CSL_HDMI_HDCP_CTRL_ENC_ON_MASK   (0x00000040u)
#define CSL_HDMI_HDCP_CTRL_ENC_ON_SHIFT  (0x00000006u)
#define CSL_HDMI_HDCP_CTRL_ENC_ON_RESETVAL (0x00000000u)

#define CSL_HDMI_HDCP_CTRL_BKSV_ERR_MASK (0x00000020u)
#define CSL_HDMI_HDCP_CTRL_BKSV_ERR_SHIFT (0x00000005u)
#define CSL_HDMI_HDCP_CTRL_BKSV_ERR_RESETVAL (0x00000000u)

#define CSL_HDMI_HDCP_CTRL_RX_RPTR_MASK  (0x00000010u)
#define CSL_HDMI_HDCP_CTRL_RX_RPTR_SHIFT (0x00000004u)
#define CSL_HDMI_HDCP_CTRL_RX_RPTR_RESETVAL (0x00000000u)

#define CSL_HDMI_HDCP_CTRL_TX_ANSTOP_MASK (0x00000008u)
#define CSL_HDMI_HDCP_CTRL_TX_ANSTOP_SHIFT (0x00000003u)
#define CSL_HDMI_HDCP_CTRL_TX_ANSTOP_RESETVAL (0x00000000u)

#define CSL_HDMI_HDCP_CTRL_CP_RESTN_MASK (0x00000004u)
#define CSL_HDMI_HDCP_CTRL_CP_RESTN_SHIFT (0x00000002u)
#define CSL_HDMI_HDCP_CTRL_CP_RESTN_RESETVAL (0x00000000u)

#define CSL_HDMI_HDCP_CTRL_RI_RDY_MASK   (0x00000002u)
#define CSL_HDMI_HDCP_CTRL_RI_RDY_SHIFT  (0x00000001u)
#define CSL_HDMI_HDCP_CTRL_RI_RDY_RESETVAL (0x00000000u)

#define CSL_HDMI_HDCP_CTRL_ENC_EN_MASK   (0x00000001u)
#define CSL_HDMI_HDCP_CTRL_ENC_EN_SHIFT  (0x00000000u)
#define CSL_HDMI_HDCP_CTRL_ENC_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_HDCP_CTRL_RESETVAL      (0x00000000u)

/* BKSV_0 */
#define CSL_HDMI_BKSV_0_BKSV_MASK        (0x000000FFu)
#define CSL_HDMI_BKSV_0_BKSV_SHIFT       (0x00000000u)
#define CSL_HDMI_BKSV_0_BKSV_RESETVAL    (0x00000000u)

#define CSL_HDMI_BKSV_0_RESETVAL         (0x00000000u)

/* BKSV_1 */
#define CSL_HDMI_BKSV_1_BKSV_MASK        (0x000000FFu)
#define CSL_HDMI_BKSV_1_BKSV_SHIFT       (0x00000000u)
#define CSL_HDMI_BKSV_1_BKSV_RESETVAL    (0x00000000u)

#define CSL_HDMI_BKSV_1_RESETVAL         (0x00000000u)

/* BKSV_2 */
#define CSL_HDMI_BKSV_2_BKSV_MASK        (0x000000FFu)
#define CSL_HDMI_BKSV_2_BKSV_SHIFT       (0x00000000u)
#define CSL_HDMI_BKSV_2_BKSV_RESETVAL    (0x00000000u)

#define CSL_HDMI_BKSV_2_RESETVAL         (0x00000000u)

/* BKSV_3 */
#define CSL_HDMI_BKSV_3_BKSV_MASK        (0x000000FFu)
#define CSL_HDMI_BKSV_3_BKSV_SHIFT       (0x00000000u)
#define CSL_HDMI_BKSV_3_BKSV_RESETVAL    (0x00000000u)

#define CSL_HDMI_BKSV_3_RESETVAL         (0x00000000u)

/* BKSV_4 */
#define CSL_HDMI_BKSV_4_BKSV_MASK        (0x000000FFu)
#define CSL_HDMI_BKSV_4_BKSV_SHIFT       (0x00000000u)
#define CSL_HDMI_BKSV_4_BKSV_RESETVAL    (0x00000000u)

#define CSL_HDMI_BKSV_4_RESETVAL         (0x00000000u)

/* AN___0 */
#define CSL_HDMI_AN__0_AN_MASK          (0x000000FFu)
#define CSL_HDMI_AN__0_AN_SHIFT         (0x00000000u)
#define CSL_HDMI_AN__0_AN_RESETVAL      (0x00000000u)

#define CSL_HDMI_AN__0_RESETVAL         (0x00000000u)

/* AN___1 */
#define CSL_HDMI_AN__1_AN_MASK          (0x000000FFu)
#define CSL_HDMI_AN__1_AN_SHIFT         (0x00000000u)
#define CSL_HDMI_AN__1_AN_RESETVAL      (0x00000000u)

#define CSL_HDMI_AN__1_RESETVAL         (0x00000000u)

/* AN____2 */
#define CSL_HDMI_AN__2_AN_MASK         (0x000000FFu)
#define CSL_HDMI_AN__2_AN_SHIFT        (0x00000000u)
#define CSL_HDMI_AN__2_AN_RESETVAL     (0x00000000u)

#define CSL_HDMI_AN__2_RESETVAL        (0x00000000u)

/* AN__3 */
#define CSL_HDMI_AN__3_AN_MASK           (0x000000FFu)
#define CSL_HDMI_AN__3_AN_SHIFT          (0x00000000u)
#define CSL_HDMI_AN__3_AN_RESETVAL       (0x00000000u)

#define CSL_HDMI_AN__3_RESETVAL          (0x00000000u)

/* AN__4 */
#define CSL_HDMI_AN__4_AN_MASK           (0x000000FFu)
#define CSL_HDMI_AN__4_AN_SHIFT          (0x00000000u)
#define CSL_HDMI_AN__4_AN_RESETVAL       (0x00000000u)

#define CSL_HDMI_AN__4_RESETVAL          (0x00000000u)

/* AN__5 */
#define CSL_HDMI_AN__5_AN_MASK           (0x000000FFu)
#define CSL_HDMI_AN__5_AN_SHIFT          (0x00000000u)
#define CSL_HDMI_AN__5_AN_RESETVAL       (0x00000000u)

#define CSL_HDMI_AN__5_RESETVAL          (0x00000000u)

/* AN__6 */
#define CSL_HDMI_AN__6_AN_MASK           (0x000000FFu)
#define CSL_HDMI_AN__6_AN_SHIFT          (0x00000000u)
#define CSL_HDMI_AN__6_AN_RESETVAL       (0x00000000u)

#define CSL_HDMI_AN__6_RESETVAL          (0x00000000u)

/* AN__7 */
#define CSL_HDMI_AN__7_AN_MASK           (0x000000FFu)
#define CSL_HDMI_AN__7_AN_SHIFT          (0x00000000u)
#define CSL_HDMI_AN__7_AN_RESETVAL       (0x00000000u)

#define CSL_HDMI_AN__7_RESETVAL          (0x00000000u)

/* AKSV__0 */
#define CSL_HDMI_AKSV__0_AKSV_MASK       (0x000000FFu)
#define CSL_HDMI_AKSV__0_AKSV_SHIFT      (0x00000000u)
#define CSL_HDMI_AKSV__0_AKSV_RESETVAL   (0x00000000u)

#define CSL_HDMI_AKSV__0_RESETVAL        (0x00000000u)

/* AKSV__1 */
#define CSL_HDMI_AKSV__1_AKSV_MASK       (0x000000FFu)
#define CSL_HDMI_AKSV__1_AKSV_SHIFT      (0x00000000u)
#define CSL_HDMI_AKSV__1_AKSV_RESETVAL   (0x00000000u)

#define CSL_HDMI_AKSV__1_RESETVAL        (0x00000000u)

/* AKSV__2 */
#define CSL_HDMI_AKSV__2_AKSV_MASK       (0x000000FFu)
#define CSL_HDMI_AKSV__2_AKSV_SHIFT      (0x00000000u)
#define CSL_HDMI_AKSV__2_AKSV_RESETVAL   (0x00000000u)

#define CSL_HDMI_AKSV__2_RESETVAL        (0x00000000u)

/* AKSV__3 */
#define CSL_HDMI_AKSV__3_AKSV_MASK       (0x000000FFu)
#define CSL_HDMI_AKSV__3_AKSV_SHIFT      (0x00000000u)
#define CSL_HDMI_AKSV__3_AKSV_RESETVAL   (0x00000000u)

#define CSL_HDMI_AKSV__3_RESETVAL        (0x00000000u)

/* AKSV__4 */
#define CSL_HDMI_AKSV__4_AKSV_MASK       (0x000000FFu)
#define CSL_HDMI_AKSV__4_AKSV_SHIFT      (0x00000000u)
#define CSL_HDMI_AKSV__4_AKSV_RESETVAL   (0x00000000u)

#define CSL_HDMI_AKSV__4_RESETVAL        (0x00000000u)

/* RI1 */
#define CSL_HDMI_RI1_RI_MASK             (0x000000FFu)
#define CSL_HDMI_RI1_RI_SHIFT            (0x00000000u)
#define CSL_HDMI_RI1_RI_RESETVAL         (0x00000000u)

#define CSL_HDMI_RI1_RESETVAL            (0x00000000u)

/* RI2 */
#define CSL_HDMI_RI2_RI_MASK             (0x000000FFu)
#define CSL_HDMI_RI2_RI_SHIFT            (0x00000000u)
#define CSL_HDMI_RI2_RI_RESETVAL         (0x00000000u)

#define CSL_HDMI_RI2_RESETVAL            (0x00000000u)

/* RI_128_COMP */
#define CSL_HDMI_RI_128_COMP_RI_128_COMP_MASK (0x0000007Fu)
#define CSL_HDMI_RI_128_COMP_RI_128_COMP_SHIFT (0x00000000u)
#define CSL_HDMI_RI_128_COMP_RI_128_COMP_RESETVAL (0x00000000u)

#define CSL_HDMI_RI_128_COMP_RESETVAL    (0x00000000u)

/* I_CNT */
#define CSL_HDMI_I_CNT_I_CNT_MASK        (0x0000007Fu)
#define CSL_HDMI_I_CNT_I_CNT_SHIFT       (0x00000000u)
#define CSL_HDMI_I_CNT_I_CNT_RESETVAL    (0x00000000u)

#define CSL_HDMI_I_CNT_RESETVAL          (0x00000000u)

/* RI_STAT */
#define CSL_HDMI_RI_STAT_RI_STARTED_MASK (0x00000001u)  /* Sujith TBD update excel */
#define CSL_HDMI_RI_STAT_RI_STARTED_SHIFT (0x00000000u)
#define CSL_HDMI_RI_STAT_RI_STARTED_RESETVAL (0x00000000u)

#define CSL_HDMI_RI_STAT_RESETVAL        (0x00000000u)

/* RI_CMD */
#define CSL_HDMI_RI_CMD_STR_1STST_EN_MASK (0x00000004u)
#define CSL_HDMI_RI_CMD_STR_1STST_EN_SHIFT (0x00000002u)
#define CSL_HDMI_RI_CMD_STR_1STST_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_RI_CMD_BCAP_EN_MASK     (0x00000002u)
#define CSL_HDMI_RI_CMD_BCAP_EN_SHIFT    (0x00000001u)
#define CSL_HDMI_RI_CMD_BCAP_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_RI_CMD_RI_EN_MASK       (0x00000001u)
#define CSL_HDMI_RI_CMD_RI_EN_SHIFT      (0x00000000u)
#define CSL_HDMI_RI_CMD_RI_EN_RESETVAL   (0x00000000u)

#define CSL_HDMI_RI_CMD_RESETVAL         (0x00000000u)

/* RI_START */


#define CSL_HDMI_RI_START_RI_LINE_START_MASK (0x000000FFu)
#define CSL_HDMI_RI_START_RI_LINE_START_SHIFT (0x00000000u)
#define CSL_HDMI_RI_START_RI_LINE_START_RESETVAL (0x00000004u)

#define CSL_HDMI_RI_START_RESETVAL       (0x00000004u)

/* RI_RX_L */


#define CSL_HDMI_RI_RX_L_RI_RX_MASK      (0x000000FFu)
#define CSL_HDMI_RI_RX_L_RI_RX_SHIFT     (0x00000000u)
#define CSL_HDMI_RI_RX_L_RI_RX_RESETVAL  (0x00000097u)

#define CSL_HDMI_RI_RX_L_RESETVAL        (0x00000097u)

/* RI_DEBUG */


#define CSL_HDMI_RI_DEBUG_RI_DBG_TRASH_MASK (0x00000080u)
#define CSL_HDMI_RI_DEBUG_RI_DBG_TRASH_SHIFT (0x00000007u)
#define CSL_HDMI_RI_DEBUG_RI_DBG_TRASH_RESETVAL (0x00000000u)

#define CSL_HDMI_RI_DEBUG_RI_DBG_HOLD_MASK (0x00000040u)
#define CSL_HDMI_RI_DEBUG_RI_DBG_HOLD_SHIFT (0x00000006u)
#define CSL_HDMI_RI_DEBUG_RI_DBG_HOLD_RESETVAL (0x00000000u)



#define CSL_HDMI_RI_DEBUG_RESETVAL       (0x00000000u)

/* DE_DLY */

#define CSL_HDMI_DE_DLY_DE_DLY_MASK (0x000000FFu)
#define CSL_HDMI_DE_DLY_DE_DLY_SHIFT (0x00000000u)
#define CSL_HDMI_DE_DLY_DE_DLY_RESETVAL (0x00000000u)


#define CSL_HDMI_DE_DLY_RESETVAL       (0x00000000u)

/* DE_CTRL */

#define CSL_HDMI_DE_CTRL_DE_GEN_MASK (0x00000040u)
#define CSL_HDMI_DE_CTRL_DE_GEN_SHIFT (0x00000006u)
#define CSL_HDMI_DE_CTRL_DE_GEN_RESETVAL (0x00000000u)

#define CSL_HDMI_DE_CTRL_VS_POL_MASK (0x00000020u)
#define CSL_HDMI_DE_CTRL_VS_POL_SHIFT (0x00000005u)
#define CSL_HDMI_DE_CTRL_VS_POL_RESETVAL (0x00000000u)

#define CSL_HDMI_DE_CTRL_HS_POL_MASK (0x00000010u)
#define CSL_HDMI_DE_CTRL_HS_POL_SHIFT (0x00000004u)
#define CSL_HDMI_DE_CTRL_HS_POL_RESETVAL (0x00000000u)

#define CSL_HDMI_DE_CTRL_DE_DLY_MASK (0x0000000Fu)
#define CSL_HDMI_DE_CTRL_DE_DLY_SHIFT (0x00000000u)
#define CSL_HDMI_DE_CTRL_DE_DLY_RESETVAL (0x00000000u)


#define CSL_HDMI_DE_CTRL_RESETVAL       (0x00000000u)

/* DE_TOP */



#define CSL_HDMI_DE_TOP_DE_TOP_MASK (0x0000007Fu)
#define CSL_HDMI_DE_TOP_DE_TOP_SHIFT (0x00000000u)
#define CSL_HDMI_DE_TOP_DE_TOP_RESETVAL (0x00000000u)

#define CSL_HDMI_DE_TOP_RESETVAL         (0x00000000u)

/* DE_CNTL */


#define CSL_HDMI_DE_CNTL_DE_CNT_MASK     (0x000000FFu)
#define CSL_HDMI_DE_CNTL_DE_CNT_SHIFT    (0x00000000u)
#define CSL_HDMI_DE_CNTL_DE_CNT_RESETVAL (0x00000000u)

#define CSL_HDMI_DE_CNTL_RESETVAL        (0x00000000u)

/* DE_CNTH */



#define CSL_HDMI_DE_CNTH_DE_CNT_MASK (0x0000000Fu)
#define CSL_HDMI_DE_CNTH_DE_CNT_SHIFT (0x00000000u)
#define CSL_HDMI_DE_CNTH_DE_CNT_RESETVAL (0x00000000u)

#define CSL_HDMI_DE_CNTH_RESETVAL        (0x00000000u)

/* DE_LINL */


#define CSL_HDMI_DE_LINL_DE_LIN_MASK     (0x000000FFu)
#define CSL_HDMI_DE_LINL_DE_LIN_SHIFT    (0x00000000u)
#define CSL_HDMI_DE_LINL_DE_LIN_RESETVAL (0x00000000u)

#define CSL_HDMI_DE_LINL_RESETVAL        (0x00000000u)

/* DE_LINH */



#define CSL_HDMI_DE_LINH_DE_LIN_MASK     (0x00000007u)
#define CSL_HDMI_DE_LINH_DE_LIN_SHIFT    (0x00000000u)
#define CSL_HDMI_DE_LINH_DE_LIN_RESETVAL (0x00000000u)

#define CSL_HDMI_DE_LINH_RESETVAL        (0x00000000u)

/* HRES_L */


#define CSL_HDMI_HRES_L_H_RES_MASK       (0x000000FFu)
#define CSL_HDMI_HRES_L_H_RES_SHIFT      (0x00000000u)
#define CSL_HDMI_HRES_L_H_RES_RESETVAL   (0x00000000u)

#define CSL_HDMI_HRES_L_RESETVAL         (0x00000000u)

/* HRES_H */



#define CSL_HDMI_HRES_H_H_RES_MASK       (0x0000001Fu)
#define CSL_HDMI_HRES_H_H_RES_SHIFT      (0x00000000u)
#define CSL_HDMI_HRES_H_H_RES_RESETVAL   (0x00000000u)

#define CSL_HDMI_HRES_H_RESETVAL         (0x00000000u)

/* HRES_L */


#define CSL_HDMI_VRES_L_V_RES_MASK       (0x000000FFu)
#define CSL_HDMI_VRES_L_V_RES_SHIFT      (0x00000000u)
#define CSL_HDMI_VRES_L_V_RES_RESETVAL   (0x00000000u)

#define CSL_HDMI_VRES_L_RESETVAL         (0x00000000u)

/* VRES_H */



#define CSL_HDMI_VRES_H_V_RES_MASK       (0x00000007u)
#define CSL_HDMI_VRES_H_V_RES_SHIFT      (0x00000000u)
#define CSL_HDMI_VRES_H_V_RES_RESETVAL   (0x00000000u)

#define CSL_HDMI_VRES_H_RESETVAL         (0x00000000u)

/* IADJUST */



#define CSL_HDMI_IADJUST_DE_ADJ_MASK     (0x00000004u)
#define CSL_HDMI_IADJUST_DE_ADJ_SHIFT    (0x00000002u)
#define CSL_HDMI_IADJUST_DE_ADJ_RESETVAL (0x00000001u)

#define CSL_HDMI_IADJUST_F2VADJ_MASK     (0x00000002u)
#define CSL_HDMI_IADJUST_F2VADJ_SHIFT    (0x00000001u)
#define CSL_HDMI_IADJUST_F2VADJ_RESETVAL (0x00000001u)

#define CSL_HDMI_IADJUST_F2VOFST_MASK    (0x00000001u)
#define CSL_HDMI_IADJUST_F2VOFST_SHIFT   (0x00000000u)
#define CSL_HDMI_IADJUST_F2VOFST_RESETVAL (0x00000000u)

#define CSL_HDMI_IADJUST_RESETVAL        (0x00000006u)

/* POL_DETECT */



#define CSL_HDMI_POL_DETECT_I_DET_MASK   (0x00000004u)
#define CSL_HDMI_POL_DETECT_I_DET_SHIFT  (0x00000002u)
#define CSL_HDMI_POL_DETECT_I_DET_RESETVAL (0x00000001u)

#define CSL_HDMI_POL_DETECT_VPOL_DET_MASK (0x00000002u)
#define CSL_HDMI_POL_DETECT_VPOL_DET_SHIFT (0x00000001u)
#define CSL_HDMI_POL_DETECT_VPOL_DET_RESETVAL (0x00000001u)

#define CSL_HDMI_POL_DETECT_HPOL_DET_MASK (0x00000001u)
#define CSL_HDMI_POL_DETECT_HPOL_DET_SHIFT (0x00000000u)
#define CSL_HDMI_POL_DETECT_HPOL_DET_RESETVAL (0x00000000u)

#define CSL_HDMI_POL_DETECT_RESETVAL     (0x00000006u)

/* HBIT_2HSYNC1 */


#define CSL_HDMI_HBIT_2HSYNC1_HBIT_TO_HSYNC_MASK (0x000000FFu)
#define CSL_HDMI_HBIT_2HSYNC1_HBIT_TO_HSYNC_SHIFT (0x00000000u)
#define CSL_HDMI_HBIT_2HSYNC1_HBIT_TO_HSYNC_RESETVAL (0x00000000u)

#define CSL_HDMI_HBIT_2HSYNC1_RESETVAL   (0x00000000u)

/* HBIT_2HSYNC2 */



#define CSL_HDMI_HBIT_2HSYNC2_HBIT_TO_HSYNC_MASK (0x00000003u)
#define CSL_HDMI_HBIT_2HSYNC2_HBIT_TO_HSYNC_SHIFT (0x00000000u)
#define CSL_HDMI_HBIT_2HSYNC2_HBIT_TO_HSYNC_RESETVAL (0x00000000u)

#define CSL_HDMI_HBIT_2HSYNC2_RESETVAL   (0x00000004u)

/* FLD2_HS_OFSTL */


#define CSL_HDMI_FLD2_HS_OFSTL_FIELD2_OFST_MASK (0x000000FFu)
#define CSL_HDMI_FLD2_HS_OFSTL_FIELD2_OFST_SHIFT (0x00000000u)
#define CSL_HDMI_FLD2_HS_OFSTL_FIELD2_OFST_RESETVAL (0x00000000u)

#define CSL_HDMI_FLD2_HS_OFSTL_RESETVAL  (0x00000000u)

/* FLD2_HS_OFSTH */



#define CSL_HDMI_FLD2_HS_OFSTH_FIELD2_OFST_MASK (0x0000000Fu)
#define CSL_HDMI_FLD2_HS_OFSTH_FIELD2_OFST_SHIFT (0x00000000u)
#define CSL_HDMI_FLD2_HS_OFSTH_FIELD2_OFST_RESETVAL (0x00000000u)

#define CSL_HDMI_FLD2_HS_OFSTH_RESETVAL  (0x00000000u)

/* HWIDTH1 */


#define CSL_HDMI_HWIDTH1_HWIDTH_MASK     (0x000000FFu)
#define CSL_HDMI_HWIDTH1_HWIDTH_SHIFT    (0x00000000u)
#define CSL_HDMI_HWIDTH1_HWIDTH_RESETVAL (0x00000000u)

#define CSL_HDMI_HWIDTH1_RESETVAL        (0x00000000u)

/* HWIDTH2 */



#define CSL_HDMI_HWIDTH2_HWIDTH_MASK     (0x00000003u)
#define CSL_HDMI_HWIDTH2_HWIDTH_SHIFT    (0x00000000u)
#define CSL_HDMI_HWIDTH2_HWIDTH_RESETVAL (0x00000000u)

#define CSL_HDMI_HWIDTH2_RESETVAL        (0x00000000u)

/* VBIT_TO_VSYNC */



#define CSL_HDMI_VBIT_TO_VSYNC_VBIT_TO_VSYNC_MASK (0x0000003Fu)
#define CSL_HDMI_VBIT_TO_VSYNC_VBIT_TO_VSYNC_SHIFT (0x00000000u)
#define CSL_HDMI_VBIT_TO_VSYNC_VBIT_TO_VSYNC_RESETVAL (0x00000000u)

#define CSL_HDMI_VBIT_TO_VSYNC_RESETVAL  (0x00000000u)

/* VWIDTH */



#define CSL_HDMI_VWIDTH_VWIDTH_MASK      (0x0000003Fu)
#define CSL_HDMI_VWIDTH_VWIDTH_SHIFT     (0x00000000u)
#define CSL_HDMI_VWIDTH_VWIDTH_RESETVAL  (0x00000000u)

#define CSL_HDMI_VWIDTH_RESETVAL         (0x00000000u)

/* VID_CTRL */


#define CSL_HDMI_VID_CTRL_IFPOL_MASK     (0x00000080u)
#define CSL_HDMI_VID_CTRL_IFPOL_SHIFT    (0x00000007u)
#define CSL_HDMI_VID_CTRL_IFPOL_RESETVAL (0x00000000u)


#define CSL_HDMI_VID_CTRL_EXTN_MASK      (0x00000020u)
#define CSL_HDMI_VID_CTRL_EXTN_SHIFT     (0x00000005u)
#define CSL_HDMI_VID_CTRL_EXTN_RESETVAL  (0x00000001u)

#define CSL_HDMI_VID_CTRL_CSCSEL_MASK    (0x00000010u)
#define CSL_HDMI_VID_CTRL_CSCSEL_SHIFT   (0x00000004u)
#define CSL_HDMI_VID_CTRL_CSCSEL_RESETVAL (0x00000000u)


#define CSL_HDMI_VID_CTRL_ICLK_MASK      (0x00000003u)
#define CSL_HDMI_VID_CTRL_ICLK_SHIFT     (0x00000000u)
#define CSL_HDMI_VID_CTRL_ICLK_RESETVAL  (0x00000000u)

#define CSL_HDMI_VID_CTRL_RESETVAL       (0x00000060u)

/* VID_ACEN */


#define CSL_HDMI_VID_ACEN_WIDE_BUS_MASK  (0x000000C0u)
#define CSL_HDMI_VID_ACEN_WIDE_BUS_SHIFT (0x00000006u)
#define CSL_HDMI_VID_ACEN_WIDE_BUS_RESETVAL (0x00000000u)


#define CSL_HDMI_VID_ACEN_CLIP_CS_ID_MASK (0x00000010u)
#define CSL_HDMI_VID_ACEN_CLIP_CS_ID_SHIFT (0x00000004u)
#define CSL_HDMI_VID_ACEN_CLIP_CS_ID_RESETVAL (0x00000001u)

#define CSL_HDMI_VID_ACEN_RANGE_CLIP_MASK (0x00000008u)
#define CSL_HDMI_VID_ACEN_RANGE_CLIP_SHIFT (0x00000003u)
#define CSL_HDMI_VID_ACEN_RANGE_CLIP_RESETVAL (0x00000000u)

#define CSL_HDMI_VID_ACEN_RGB_2_YCBCR_MASK (0x00000004u)
#define CSL_HDMI_VID_ACEN_RGB_2_YCBCR_SHIFT (0x00000002u)
#define CSL_HDMI_VID_ACEN_RGB_2_YCBCR_RESETVAL (0x00000000u)

#define CSL_HDMI_VID_ACEN_RANGE_CMPS_MASK (0x00000002u)
#define CSL_HDMI_VID_ACEN_RANGE_CMPS_SHIFT (0x00000001u)
#define CSL_HDMI_VID_ACEN_RANGE_CMPS_RESETVAL (0x00000000u)

#define CSL_HDMI_VID_ACEN_DOWN_SMPL_MASK (0x00000001u)
#define CSL_HDMI_VID_ACEN_DOWN_SMPL_SHIFT (0x00000000u)
#define CSL_HDMI_VID_ACEN_DOWN_SMPL_RESETVAL (0x00000000u)

#define CSL_HDMI_VID_ACEN_RESETVAL       (0x00000010u)

/* VID_MODE */


#define CSL_HDMI_VID_MODE_DITHER_MODE_MASK (0x000000C0u)
#define CSL_HDMI_VID_MODE_DITHER_MODE_SHIFT (0x00000006u)
#define CSL_HDMI_VID_MODE_DITHER_MODE_RESETVAL (0x00000000u)

#define CSL_HDMI_VID_MODE_DITHER_MASK    (0x00000020u)
#define CSL_HDMI_VID_MODE_DITHER_SHIFT   (0x00000005u)
#define CSL_HDMI_VID_MODE_DITHER_RESETVAL (0x00000001u)

#define CSL_HDMI_VID_MODE_RANGE_MASK     (0x00000010u)
#define CSL_HDMI_VID_MODE_RANGE_SHIFT    (0x00000004u)
#define CSL_HDMI_VID_MODE_RANGE_RESETVAL (0x00000001u)

#define CSL_HDMI_VID_MODE_CSC_MASK       (0x00000008u)
#define CSL_HDMI_VID_MODE_CSC_SHIFT      (0x00000003u)
#define CSL_HDMI_VID_MODE_CSC_RESETVAL   (0x00000000u)

#define CSL_HDMI_VID_MODE_UPSMP_MASK     (0x00000004u)
#define CSL_HDMI_VID_MODE_UPSMP_SHIFT    (0x00000002u)
#define CSL_HDMI_VID_MODE_UPSMP_RESETVAL (0x00000000u)

#define CSL_HDMI_VID_MODE_DEMUX_MASK     (0x00000002u)
#define CSL_HDMI_VID_MODE_DEMUX_SHIFT    (0x00000001u)
#define CSL_HDMI_VID_MODE_DEMUX_RESETVAL (0x00000000u)

#define CSL_HDMI_VID_MODE_SYNCEX_MASK    (0x00000001u)
#define CSL_HDMI_VID_MODE_SYNCEX_SHIFT   (0x00000000u)
#define CSL_HDMI_VID_MODE_SYNCEX_RESETVAL (0x00000000u)

#define CSL_HDMI_VID_MODE_RESETVAL       (0x00000030u)

/* VID_BLANK1 */


#define CSL_HDMI_VID_BLANK1_VID_BLANK1_MASK (0x000000FFu)
#define CSL_HDMI_VID_BLANK1_VID_BLANK1_SHIFT (0x00000000u)
#define CSL_HDMI_VID_BLANK1_VID_BLANK1_RESETVAL (0x00000000u)

#define CSL_HDMI_VID_BLANK1_RESETVAL     (0x00000000u)

/* VID_BLANK2 */


#define CSL_HDMI_VID_BLANK2_VID_BLANK2_MASK (0x000000FFu)
#define CSL_HDMI_VID_BLANK2_VID_BLANK2_SHIFT (0x00000000u)
#define CSL_HDMI_VID_BLANK2_VID_BLANK2_RESETVAL (0x00000000u)

#define CSL_HDMI_VID_BLANK2_RESETVAL     (0x00000000u)

/* VID_BLANK3 */


#define CSL_HDMI_VID_BLANK3_VID_BLANK3_MASK (0x000000FFu)
#define CSL_HDMI_VID_BLANK3_VID_BLANK3_SHIFT (0x00000000u)
#define CSL_HDMI_VID_BLANK3_VID_BLANK3_RESETVAL (0x00000000u)

#define CSL_HDMI_VID_BLANK3_RESETVAL     (0x00000000u)

/* DC_HEADER */


#define CSL_HDMI_DC_HEADER_DC_HEADER_MASK (0x000000FFu)
#define CSL_HDMI_DC_HEADER_DC_HEADER_SHIFT (0x00000000u)
#define CSL_HDMI_DC_HEADER_DC_HEADER_RESETVAL (0x00000003u)

#define CSL_HDMI_DC_HEADER_RESETVAL      (0x00000003u)

/* VID_DITHER */



#define CSL_HDMI_VID_DITHER_M_D2_MASK    (0x00000040u)
#define CSL_HDMI_VID_DITHER_M_D2_SHIFT   (0x00000006u)
#define CSL_HDMI_VID_DITHER_M_D2_RESETVAL (0x00000000u)

#define CSL_HDMI_VID_DITHER_UP2_MASK     (0x00000020u)
#define CSL_HDMI_VID_DITHER_UP2_SHIFT    (0x00000005u)
#define CSL_HDMI_VID_DITHER_UP2_RESETVAL (0x00000001u)

#define CSL_HDMI_VID_DITHER_STR_422_EN_MASK (0x00000010u)
#define CSL_HDMI_VID_DITHER_STR_422_EN_SHIFT (0x00000004u)
#define CSL_HDMI_VID_DITHER_STR_422_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_VID_DITHER_D_BC_EN_MASK (0x00000008u)
#define CSL_HDMI_VID_DITHER_D_BC_EN_SHIFT (0x00000003u)
#define CSL_HDMI_VID_DITHER_D_BC_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_VID_DITHER_D_GC_EN_MASK (0x00000004u)
#define CSL_HDMI_VID_DITHER_D_GC_EN_SHIFT (0x00000002u)
#define CSL_HDMI_VID_DITHER_D_GC_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_VID_DITHER_D_RC_EN_MASK (0x00000002u)
#define CSL_HDMI_VID_DITHER_D_RC_EN_SHIFT (0x00000001u)
#define CSL_HDMI_VID_DITHER_D_RC_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_VID_DITHER_DRD_MASK     (0x00000001u)
#define CSL_HDMI_VID_DITHER_DRD_SHIFT    (0x00000000u)
#define CSL_HDMI_VID_DITHER_DRD_RESETVAL (0x00000000u)

#define CSL_HDMI_VID_DITHER_RESETVAL     (0x00000020u)

/* RGB2XVYCC_CT */



#define CSL_HDMI_RGB2XVYCC_CT_XV_CO_OV_MASK (0x00000004u)
#define CSL_HDMI_RGB2XVYCC_CT_XV_CO_OV_SHIFT (0x00000002u)
#define CSL_HDMI_RGB2XVYCC_CT_XV_CO_OV_RESETVAL (0x00000000u)

#define CSL_HDMI_RGB2XVYCC_CT_XV_FUS_MASK (0x00000002u)
#define CSL_HDMI_RGB2XVYCC_CT_XV_FUS_SHIFT (0x00000001u)
#define CSL_HDMI_RGB2XVYCC_CT_XV_FUS_RESETVAL (0x00000000u)

#define CSL_HDMI_RGB2XVYCC_CT_XV_EN_MASK (0x00000001u)
#define CSL_HDMI_RGB2XVYCC_CT_XV_EN_SHIFT (0x00000000u)
#define CSL_HDMI_RGB2XVYCC_CT_XV_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_RGB2XVYCC_CT_RESETVAL   (0x00000000u)

/* R2Y_COEFF_LOW */


#define CSL_HDMI_R2Y_COEFF_LOW_R2YCOEFF_L_MASK (0x000000FFu)
#define CSL_HDMI_R2Y_COEFF_LOW_R2YCOEFF_L_SHIFT (0x00000000u)
#define CSL_HDMI_R2Y_COEFF_LOW_R2YCOEFF_L_RESETVAL (0x00000000u)

#define CSL_HDMI_R2Y_COEFF_LOW_RESETVAL  (0x00000000u)

/* R2Y_COEFF_UP */


#define CSL_HDMI_R2Y_COEFF_UP_R2YCOEFF_H_MASK (0x000000FFu)
#define CSL_HDMI_R2Y_COEFF_UP_R2YCOEFF_H_SHIFT (0x00000000u)
#define CSL_HDMI_R2Y_COEFF_UP_R2YCOEFF_H_RESETVAL (0x00000000u)

#define CSL_HDMI_R2Y_COEFF_UP_RESETVAL   (0x00000000u)

/* G2Y_COEFF_LOW */


#define CSL_HDMI_G2Y_COEFF_LOW_G2YCOEFF_L_MASK (0x000000FFu)
#define CSL_HDMI_G2Y_COEFF_LOW_G2YCOEFF_L_SHIFT (0x00000000u)
#define CSL_HDMI_G2Y_COEFF_LOW_G2YCOEFF_L_RESETVAL (0x00000000u)

#define CSL_HDMI_G2Y_COEFF_LOW_RESETVAL  (0x00000000u)

/* G2Y_COEFF_UP */


#define CSL_HDMI_G2Y_COEFF_UP_G2YCOEFF_H_MASK (0x000000FFu)
#define CSL_HDMI_G2Y_COEFF_UP_G2YCOEFF_H_SHIFT (0x00000000u)
#define CSL_HDMI_G2Y_COEFF_UP_G2YCOEFF_H_RESETVAL (0x00000000u)

#define CSL_HDMI_G2Y_COEFF_UP_RESETVAL   (0x00000000u)

/* B2Y_COEFF_LOW */


#define CSL_HDMI_B2Y_COEFF_LOW_B2YCOEFF_L_MASK (0x000000FFu)
#define CSL_HDMI_B2Y_COEFF_LOW_B2YCOEFF_L_SHIFT (0x00000000u)
#define CSL_HDMI_B2Y_COEFF_LOW_B2YCOEFF_L_RESETVAL (0x00000000u)

#define CSL_HDMI_B2Y_COEFF_LOW_RESETVAL  (0x00000000u)

/* B2Y_COEFF_UP */


#define CSL_HDMI_B2Y_COEFF_UP_B2YCOEFF_H_MASK (0x000000FFu)
#define CSL_HDMI_B2Y_COEFF_UP_B2YCOEFF_H_SHIFT (0x00000000u)
#define CSL_HDMI_B2Y_COEFF_UP_B2YCOEFF_H_RESETVAL (0x00000000u)

#define CSL_HDMI_B2Y_COEFF_UP_RESETVAL   (0x00000000u)

/* R2CB_COEFF_LOW */


#define CSL_HDMI_R2CB_COEFF_LOW_R2CBCOEFF_L_MASK (0x000000FFu)
#define CSL_HDMI_R2CB_COEFF_LOW_R2CBCOEFF_L_SHIFT (0x00000000u)
#define CSL_HDMI_R2CB_COEFF_LOW_R2CBCOEFF_L_RESETVAL (0x00000000u)

#define CSL_HDMI_R2CB_COEFF_LOW_RESETVAL (0x00000000u)

/* R2CB_COEFF_UP */


#define CSL_HDMI_R2CB_COEFF_UP_R2CBCOEFF_H_MASK (0x000000FFu)
#define CSL_HDMI_R2CB_COEFF_UP_R2CBCOEFF_H_SHIFT (0x00000000u)
#define CSL_HDMI_R2CB_COEFF_UP_R2CBCOEFF_H_RESETVAL (0x00000000u)

#define CSL_HDMI_R2CB_COEFF_UP_RESETVAL  (0x00000000u)

/* G2CB_COEFF_LOW */


#define CSL_HDMI_G2CB_COEFF_LOW_G2CBCOEFF_L_MASK (0x000000FFu)
#define CSL_HDMI_G2CB_COEFF_LOW_G2CBCOEFF_L_SHIFT (0x00000000u)
#define CSL_HDMI_G2CB_COEFF_LOW_G2CBCOEFF_L_RESETVAL (0x00000000u)

#define CSL_HDMI_G2CB_COEFF_LOW_RESETVAL (0x00000000u)

/* G2CB_COEFF_UP */


#define CSL_HDMI_G2CB_COEFF_UP_G2CBCOEFF_H_MASK (0x000000FFu)
#define CSL_HDMI_G2CB_COEFF_UP_G2CBCOEFF_H_SHIFT (0x00000000u)
#define CSL_HDMI_G2CB_COEFF_UP_G2CBCOEFF_H_RESETVAL (0x00000000u)

#define CSL_HDMI_G2CB_COEFF_UP_RESETVAL  (0x00000000u)

/* B2CB_COEFF_LOW */


#define CSL_HDMI_B2CB_COEFF_LOW_B2CBCOEFF_L_MASK (0x000000FFu)
#define CSL_HDMI_B2CB_COEFF_LOW_B2CBCOEFF_L_SHIFT (0x00000000u)
#define CSL_HDMI_B2CB_COEFF_LOW_B2CBCOEFF_L_RESETVAL (0x00000000u)

#define CSL_HDMI_B2CB_COEFF_LOW_RESETVAL (0x00000000u)

/* B2CB_COEFF_UP */


#define CSL_HDMI_B2CB_COEFF_UP_B2CBCOEFF_H_MASK (0x000000FFu)
#define CSL_HDMI_B2CB_COEFF_UP_B2CBCOEFF_H_SHIFT (0x00000000u)
#define CSL_HDMI_B2CB_COEFF_UP_B2CBCOEFF_H_RESETVAL (0x00000000u)

#define CSL_HDMI_B2CB_COEFF_UP_RESETVAL  (0x00000000u)

/* R2CR_COEFF_LOW */


#define CSL_HDMI_R2CR_COEFF_LOW_R2CRCOEFF_L_MASK (0x000000FFu)
#define CSL_HDMI_R2CR_COEFF_LOW_R2CRCOEFF_L_SHIFT (0x00000000u)
#define CSL_HDMI_R2CR_COEFF_LOW_R2CRCOEFF_L_RESETVAL (0x00000000u)

#define CSL_HDMI_R2CR_COEFF_LOW_RESETVAL (0x00000000u)

/* G2CR_COEFF_LOW */


#define CSL_HDMI_G2CR_COEFF_LOW_G2CRCOEFF_L_MASK (0x000000FFu)
#define CSL_HDMI_G2CR_COEFF_LOW_G2CRCOEFF_L_SHIFT (0x00000000u)
#define CSL_HDMI_G2CR_COEFF_LOW_G2CRCOEFF_L_RESETVAL (0x00000000u)

#define CSL_HDMI_G2CR_COEFF_LOW_RESETVAL (0x00000000u)

/* G2CR_COEFF_UP */


#define CSL_HDMI_G2CR_COEFF_UP_G2CRCOEFF_H_MASK (0x000000FFu)
#define CSL_HDMI_G2CR_COEFF_UP_G2CRCOEFF_H_SHIFT (0x00000000u)
#define CSL_HDMI_G2CR_COEFF_UP_G2CRCOEFF_H_RESETVAL (0x00000000u)

#define CSL_HDMI_G2CR_COEFF_UP_RESETVAL  (0x00000000u)

/* B2CR_COEFF_LOW */


#define CSL_HDMI_B2CR_COEFF_LOW_B2CRCOEFF_L_MASK (0x000000FFu)
#define CSL_HDMI_B2CR_COEFF_LOW_B2CRCOEFF_L_SHIFT (0x00000000u)
#define CSL_HDMI_B2CR_COEFF_LOW_B2CRCOEFF_L_RESETVAL (0x00000000u)

#define CSL_HDMI_B2CR_COEFF_LOW_RESETVAL (0x00000000u)

/* B2CR_COEFF_UP */


#define CSL_HDMI_B2CR_COEFF_UP_B2CRCOEFF_H_MASK (0x000000FFu)
#define CSL_HDMI_B2CR_COEFF_UP_B2CRCOEFF_H_SHIFT (0x00000000u)
#define CSL_HDMI_B2CR_COEFF_UP_B2CRCOEFF_H_RESETVAL (0x00000000u)

#define CSL_HDMI_B2CR_COEFF_UP_RESETVAL  (0x00000000u)

/* RGB_OFFSET_LOW */


#define CSL_HDMI_RGB_OFFSET_LOW_RGB_OFFS_L_MASK (0x000000FFu)
#define CSL_HDMI_RGB_OFFSET_LOW_RGB_OFFS_L_SHIFT (0x00000000u)
#define CSL_HDMI_RGB_OFFSET_LOW_RGB_OFFS_L_RESETVAL (0x00000000u)

#define CSL_HDMI_RGB_OFFSET_LOW_RESETVAL (0x00000000u)

/* RGB_OFFSET_UP */



#define CSL_HDMI_RGB_OFFSET_UP_RGB_OFFS_H_MASK (0x0000007Fu)
#define CSL_HDMI_RGB_OFFSET_UP_RGB_OFFS_H_SHIFT (0x00000000u)
#define CSL_HDMI_RGB_OFFSET_UP_RGB_OFFS_H_RESETVAL (0x00000000u)

#define CSL_HDMI_RGB_OFFSET_UP_RESETVAL  (0x00000000u)

/* Y_OFFSET_LOW */



#define CSL_HDMI_Y_OFFSET_LOW_Y_OFFS_L_MASK (0x0000007Fu)
#define CSL_HDMI_Y_OFFSET_LOW_Y_OFFS_L_SHIFT (0x00000000u)
#define CSL_HDMI_Y_OFFSET_LOW_Y_OFFS_L_RESETVAL (0x00000000u)

#define CSL_HDMI_Y_OFFSET_LOW_RESETVAL   (0x00000000u)

/* Y_OFFSET_UP */



#define CSL_HDMI_Y_OFFSET_UP_Y_OFFS_H_MASK (0x0000007Fu)
#define CSL_HDMI_Y_OFFSET_UP_Y_OFFS_H_SHIFT (0x00000000u)
#define CSL_HDMI_Y_OFFSET_UP_Y_OFFS_H_RESETVAL (0x00000000u)

#define CSL_HDMI_Y_OFFSET_UP_RESETVAL    (0x00000000u)

/* CBCR_OFFSET_LOW */



#define CSL_HDMI_CBCR_OFFSET_LOW_CBCR_OFFS_L_MASK (0x0000007Fu)
#define CSL_HDMI_CBCR_OFFSET_LOW_CBCR_OFFS_L_SHIFT (0x00000000u)
#define CSL_HDMI_CBCR_OFFSET_LOW_CBCR_OFFS_L_RESETVAL (0x00000000u)

#define CSL_HDMI_CBCR_OFFSET_LOW_RESETVAL (0x00000000u)

/* CBCR_OFFSET_UP */



#define CSL_HDMI_CBCR_OFFSET_UP_CBCR_OFFS_H_MASK (0x0000007Fu)
#define CSL_HDMI_CBCR_OFFSET_UP_CBCR_OFFS_H_SHIFT (0x00000000u)
#define CSL_HDMI_CBCR_OFFSET_UP_CBCR_OFFS_H_RESETVAL (0x00000000u)

#define CSL_HDMI_CBCR_OFFSET_UP_RESETVAL (0x00000000u)

/* INTR_STATE */



#define CSL_HDMI_INTR_STATE_INTR_MASK    (0x00000001u)
#define CSL_HDMI_INTR_STATE_INTR_SHIFT   (0x00000000u)
#define CSL_HDMI_INTR_STATE_INTR_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR_STATE_RESETVAL     (0x00000000u)

/* INTR1 */


#define CSL_HDMI_INTR1_SOFT_MASK         (0x00000080u)
#define CSL_HDMI_INTR1_SOFT_SHIFT        (0x00000007u)
#define CSL_HDMI_INTR1_SOFT_RESETVAL     (0x00000000u)

#define CSL_HDMI_INTR1_HPD_MASK          (0x00000040u)
#define CSL_HDMI_INTR1_HPD_SHIFT         (0x00000006u)
#define CSL_HDMI_INTR1_HPD_RESETVAL      (0x00000000u)

#define CSL_HDMI_INTR1_RSEN_MASK         (0x00000020u)
#define CSL_HDMI_INTR1_RSEN_SHIFT        (0x00000005u)
#define CSL_HDMI_INTR1_RSEN_RESETVAL     (0x00000000u)

#define CSL_HDMI_INTR1_DROP_SAMPLE_MASK  (0x00000010u)
#define CSL_HDMI_INTR1_DROP_SAMPLE_SHIFT (0x00000004u)
#define CSL_HDMI_INTR1_DROP_SAMPLE_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR1_BIP_HASE_ERR_MASK (0x00000008u)
#define CSL_HDMI_INTR1_BIP_HASE_ERR_SHIFT (0x00000003u)
#define CSL_HDMI_INTR1_BIP_HASE_ERR_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR1_RI_128_MASK       (0x00000004u)
#define CSL_HDMI_INTR1_RI_128_SHIFT      (0x00000002u)
#define CSL_HDMI_INTR1_RI_128_RESETVAL   (0x00000000u)

#define CSL_HDMI_INTR1_OVER_RUN_MASK     (0x00000002u)
#define CSL_HDMI_INTR1_OVER_RUN_SHIFT    (0x00000001u)
#define CSL_HDMI_INTR1_OVER_RUN_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR1_UNDER_RUN_MASK    (0x00000001u)
#define CSL_HDMI_INTR1_UNDER_RUN_SHIFT   (0x00000000u)
#define CSL_HDMI_INTR1_UNDER_RUN_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR1_RESETVAL          (0x00000000u)

/* INTR2 */


#define CSL_HDMI_INTR2_BCAP_DONE_MASK    (0x00000080u)
#define CSL_HDMI_INTR2_BCAP_DONE_SHIFT   (0x00000007u)
#define CSL_HDMI_INTR2_BCAP_DONE_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR2_SPDIF_PAR_MASK    (0x00000040u)
#define CSL_HDMI_INTR2_SPDIF_PAR_SHIFT   (0x00000006u)
#define CSL_HDMI_INTR2_SPDIF_PAR_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR2_ENC_DIS_MASK      (0x00000020u)
#define CSL_HDMI_INTR2_ENC_DIS_SHIFT     (0x00000005u)
#define CSL_HDMI_INTR2_ENC_DIS_RESETVAL  (0x00000000u)

#define CSL_HDMI_INTR2_PREAM_ERR_MASK    (0x00000010u)
#define CSL_HDMI_INTR2_PREAM_ERR_SHIFT   (0x00000004u)
#define CSL_HDMI_INTR2_PREAM_ERR_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR2_CTS_CHG_MASK      (0x00000008u)
#define CSL_HDMI_INTR2_CTS_CHG_SHIFT     (0x00000003u)
#define CSL_HDMI_INTR2_CTS_CHG_RESETVAL  (0x00000000u)

#define CSL_HDMI_INTR2_ACR_OVR_MASK      (0x00000004u)
#define CSL_HDMI_INTR2_ACR_OVR_SHIFT     (0x00000002u)
#define CSL_HDMI_INTR2_ACR_OVR_RESETVAL  (0x00000000u)

#define CSL_HDMI_INTR2_TCLK_STBL_MASK    (0x00000002u)
#define CSL_HDMI_INTR2_TCLK_STBL_SHIFT   (0x00000001u)
#define CSL_HDMI_INTR2_TCLK_STBL_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR2_VSYNC_REC_MASK    (0x00000001u)
#define CSL_HDMI_INTR2_VSYNC_REC_SHIFT   (0x00000000u)
#define CSL_HDMI_INTR2_VSYNC_REC_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR2_RESETVAL          (0x00000000u)

/* INTR3 */


#define CSL_HDMI_INTR3_RI_ERR_3_MASK     (0x00000080u)
#define CSL_HDMI_INTR3_RI_ERR_3_SHIFT    (0x00000007u)
#define CSL_HDMI_INTR3_RI_ERR_3_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR3_RI_ERR_2_MASK     (0x00000040u)
#define CSL_HDMI_INTR3_RI_ERR_2_SHIFT    (0x00000006u)
#define CSL_HDMI_INTR3_RI_ERR_2_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR3_RI_ERR_1_MASK     (0x00000020u)
#define CSL_HDMI_INTR3_RI_ERR_1_SHIFT    (0x00000005u)
#define CSL_HDMI_INTR3_RI_ERR_1_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR3_RI_ERR_0_MASK     (0x00000010u)
#define CSL_HDMI_INTR3_RI_ERR_0_SHIFT    (0x00000004u)
#define CSL_HDMI_INTR3_RI_ERR_0_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR3_DDC_CMD_DONE_MASK (0x00000008u)
#define CSL_HDMI_INTR3_DDC_CMD_DONE_SHIFT (0x00000003u)
#define CSL_HDMI_INTR3_DDC_CMD_DONE_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR3_DDC_FIFO_HALF_MASK (0x00000004u)
#define CSL_HDMI_INTR3_DDC_FIFO_HALF_SHIFT (0x00000002u)
#define CSL_HDMI_INTR3_DDC_FIFO_HALF_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR3_DDC_FIFO_FULL_MASK (0x00000002u)
#define CSL_HDMI_INTR3_DDC_FIFO_FULL_SHIFT (0x00000001u)
#define CSL_HDMI_INTR3_DDC_FIFO_FULL_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR3_DDC_FIFO_EMPTY_MASK (0x00000001u)
#define CSL_HDMI_INTR3_DDC_FIFO_EMPTY_SHIFT (0x00000000u)
#define CSL_HDMI_INTR3_DDC_FIFO_EMPTY_RESETVAL (0x00000000u)

#define CSL_HDMI_INTR3_RESETVAL          (0x00000000u)

/* INTR4 */


#define CSL_HDMI_INTR4_REG_INTR4_STAT1_MASK (0x00000002u)
#define CSL_HDMI_INTR4_REG_INTR4_STAT2_MASK (0x00000004u)

#define CSL_HDMI_INTR4_RESETVAL          (0x00000000u)

/* INT_UNMASK1 */


#define CSL_HDMI_INT_UNMASK1_SOFT_MASK   (0x00000080u)
#define CSL_HDMI_INT_UNMASK1_SOFT_SHIFT  (0x00000007u)
#define CSL_HDMI_INT_UNMASK1_SOFT_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK1_HPD_MASK    (0x00000040u)
#define CSL_HDMI_INT_UNMASK1_HPD_SHIFT   (0x00000006u)
#define CSL_HDMI_INT_UNMASK1_HPD_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK1_RSEN_MASK   (0x00000020u)
#define CSL_HDMI_INT_UNMASK1_RSEN_SHIFT  (0x00000005u)
#define CSL_HDMI_INT_UNMASK1_RSEN_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK1_DROP_SAMPLE_MASK (0x00000010u)
#define CSL_HDMI_INT_UNMASK1_DROP_SAMPLE_SHIFT (0x00000004u)
#define CSL_HDMI_INT_UNMASK1_DROP_SAMPLE_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK1_BIP_HASE_ERR_MASK (0x00000008u)
#define CSL_HDMI_INT_UNMASK1_BIP_HASE_ERR_SHIFT (0x00000003u)
#define CSL_HDMI_INT_UNMASK1_BIP_HASE_ERR_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK1_RI_128_MASK (0x00000004u)
#define CSL_HDMI_INT_UNMASK1_RI_128_SHIFT (0x00000002u)
#define CSL_HDMI_INT_UNMASK1_RI_128_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK1_OVER_RUN_MASK (0x00000002u)
#define CSL_HDMI_INT_UNMASK1_OVER_RUN_SHIFT (0x00000001u)
#define CSL_HDMI_INT_UNMASK1_OVER_RUN_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK1_UNDER_RUN_MASK (0x00000001u)
#define CSL_HDMI_INT_UNMASK1_UNDER_RUN_SHIFT (0x00000000u)
#define CSL_HDMI_INT_UNMASK1_UNDER_RUN_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK1_RESETVAL    (0x00000000u)

/* INT_UNMASK2 */


#define CSL_HDMI_INT_UNMASK2_BCAP_DONE_MASK (0x00000080u)
#define CSL_HDMI_INT_UNMASK2_BCAP_DONE_SHIFT (0x00000007u)
#define CSL_HDMI_INT_UNMASK2_BCAP_DONE_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK2_SPDIF_PAR_MASK (0x00000040u)
#define CSL_HDMI_INT_UNMASK2_SPDIF_PAR_SHIFT (0x00000006u)
#define CSL_HDMI_INT_UNMASK2_SPDIF_PAR_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK2_ENC_DIS_MASK (0x00000020u)
#define CSL_HDMI_INT_UNMASK2_ENC_DIS_SHIFT (0x00000005u)
#define CSL_HDMI_INT_UNMASK2_ENC_DIS_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK2_PREAM_ERR_MASK (0x00000010u)
#define CSL_HDMI_INT_UNMASK2_PREAM_ERR_SHIFT (0x00000004u)
#define CSL_HDMI_INT_UNMASK2_PREAM_ERR_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK2_CTS_CHG_MASK (0x00000008u)
#define CSL_HDMI_INT_UNMASK2_CTS_CHG_SHIFT (0x00000003u)
#define CSL_HDMI_INT_UNMASK2_CTS_CHG_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK2_ACR_OVR_MASK (0x00000004u)
#define CSL_HDMI_INT_UNMASK2_ACR_OVR_SHIFT (0x00000002u)
#define CSL_HDMI_INT_UNMASK2_ACR_OVR_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK2_TCLK_STBL_MASK (0x00000002u)
#define CSL_HDMI_INT_UNMASK2_TCLK_STBL_SHIFT (0x00000001u)
#define CSL_HDMI_INT_UNMASK2_TCLK_STBL_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK2_VSYNC_REC_MASK (0x00000001u)
#define CSL_HDMI_INT_UNMASK2_VSYNC_REC_SHIFT (0x00000000u)
#define CSL_HDMI_INT_UNMASK2_VSYNC_REC_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK2_RESETVAL    (0x00000000u)

/* INT_UNMASK3 */


#define CSL_HDMI_INT_UNMASK3_RI_ERR_3_MASK (0x00000080u)
#define CSL_HDMI_INT_UNMASK3_RI_ERR_3_SHIFT (0x00000007u)
#define CSL_HDMI_INT_UNMASK3_RI_ERR_3_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK3_RI_ERR_2_MASK (0x00000040u)
#define CSL_HDMI_INT_UNMASK3_RI_ERR_2_SHIFT (0x00000006u)
#define CSL_HDMI_INT_UNMASK3_RI_ERR_2_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK3_RI_ERR_1_MASK (0x00000020u)
#define CSL_HDMI_INT_UNMASK3_RI_ERR_1_SHIFT (0x00000005u)
#define CSL_HDMI_INT_UNMASK3_RI_ERR_1_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK3_RI_ERR_0_MASK (0x00000010u)
#define CSL_HDMI_INT_UNMASK3_RI_ERR_0_SHIFT (0x00000004u)
#define CSL_HDMI_INT_UNMASK3_RI_ERR_0_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK3_DDC_CMD_DONE_MASK (0x00000008u)
#define CSL_HDMI_INT_UNMASK3_DDC_CMD_DONE_SHIFT (0x00000003u)
#define CSL_HDMI_INT_UNMASK3_DDC_CMD_DONE_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK3_DDC_FIFO_HALF_MASK (0x00000004u)
#define CSL_HDMI_INT_UNMASK3_DDC_FIFO_HALF_SHIFT (0x00000002u)
#define CSL_HDMI_INT_UNMASK3_DDC_FIFO_HALF_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK3_DDC_FIFO_FULL_MASK (0x00000002u)
#define CSL_HDMI_INT_UNMASK3_DDC_FIFO_FULL_SHIFT (0x00000001u)
#define CSL_HDMI_INT_UNMASK3_DDC_FIFO_FULL_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK3_DDC_FIFO_EMPTY_MASK (0x00000001u)
#define CSL_HDMI_INT_UNMASK3_DDC_FIFO_EMPTY_SHIFT (0x00000000u)
#define CSL_HDMI_INT_UNMASK3_DDC_FIFO_EMPTY_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK3_RESETVAL    (0x00000000u)

/* INT_UNMASK4 */



#define CSL_HDMI_INT_UNMASK4_REG_INTR4_STAT3_MASK (0x00000008u)
#define CSL_HDMI_INT_UNMASK4_REG_INTR4_STAT3_SHIFT (0x00000003u)
#define CSL_HDMI_INT_UNMASK4_REG_INTR4_STAT3_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK4_REG_INTR4_STAT2_MASK (0x00000004u)
#define CSL_HDMI_INT_UNMASK4_REG_INTR4_STAT2_SHIFT (0x00000002u)
#define CSL_HDMI_INT_UNMASK4_REG_INTR4_STAT2_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK4_REG_INTR4_STAT1_MASK (0x00000002u)
#define CSL_HDMI_INT_UNMASK4_REG_INTR4_STAT1_SHIFT (0x00000001u)
#define CSL_HDMI_INT_UNMASK4_REG_INTR4_STAT1_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK4_DSD_INVALID_MASK (0x00000001u)
#define CSL_HDMI_INT_UNMASK4_DSD_INVALID_SHIFT (0x00000000u)
#define CSL_HDMI_INT_UNMASK4_DSD_INVALID_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_UNMASK4_RESETVAL    (0x00000000u)

/* INT_CTRL */



#define CSL_HDMI_INT_CTRL_SOFT_INTR_MASK (0x00000008u)
#define CSL_HDMI_INT_CTRL_SOFT_INTR_SHIFT (0x00000003u)
#define CSL_HDMI_INT_CTRL_SOFT_INTR_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_CTRL_OPEN_DRAIN_MASK (0x00000004u)
#define CSL_HDMI_INT_CTRL_OPEN_DRAIN_SHIFT (0x00000002u)
#define CSL_HDMI_INT_CTRL_OPEN_DRAIN_RESETVAL (0x00000000u)

#define CSL_HDMI_INT_CTRL_POLARITY_MASK  (0x00000002u)
#define CSL_HDMI_INT_CTRL_POLARITY_SHIFT (0x00000001u)
#define CSL_HDMI_INT_CTRL_POLARITY_RESETVAL (0x00000000u)


#define CSL_HDMI_INT_CTRL_RESETVAL       (0x00000000u)

/* TMDS_CCTRL */



#define CSL_HDMI_TMDS_CCTRL_FAPOSTCOUNT_MASK (0x00000020u)
#define CSL_HDMI_TMDS_CCTRL_FAPOSTCOUNT_SHIFT (0x00000005u)
#define CSL_HDMI_TMDS_CCTRL_FAPOSTCOUNT_RESETVAL (0x00000000u)


#define CSL_HDMI_TMDS_CCTRL_RESETVAL     (0x00000000u)

/* TMDS_CTRL */



#define CSL_HDMI_TMDS_CTRL_TCLKSEL_MASK  (0x00000060u)
#define CSL_HDMI_TMDS_CTRL_TCLKSEL_SHIFT (0x00000005u)
#define CSL_HDMI_TMDS_CTRL_TCLKSEL_RESETVAL (0x00000000u)


#define CSL_HDMI_TMDS_CTRL_LVBIAS_MASK   (0x00000004u)
#define CSL_HDMI_TMDS_CTRL_LVBIAS_SHIFT  (0x00000002u)
#define CSL_HDMI_TMDS_CTRL_LVBIAS_RESETVAL (0x00000000u)


#define CSL_HDMI_TMDS_CTRL_STERM_MASK    (0x00000001u)
#define CSL_HDMI_TMDS_CTRL_STERM_SHIFT   (0x00000000u)
#define CSL_HDMI_TMDS_CTRL_STERM_RESETVAL (0x00000000u)

#define CSL_HDMI_TMDS_CTRL_RESETVAL      (0x00000000u)

/* TMDS_CTRL2 */



#define CSL_HDMI_TMDS_CTRL2_TCLKSEL_MASK (0x00000060u)
#define CSL_HDMI_TMDS_CTRL2_TCLKSEL_SHIFT (0x00000005u)
#define CSL_HDMI_TMDS_CTRL2_TCLKSEL_RESETVAL (0x00000000u)


#define CSL_HDMI_TMDS_CTRL2_LVBIAS_MASK  (0x00000004u)
#define CSL_HDMI_TMDS_CTRL2_LVBIAS_SHIFT (0x00000002u)
#define CSL_HDMI_TMDS_CTRL2_LVBIAS_RESETVAL (0x00000000u)


#define CSL_HDMI_TMDS_CTRL2_STERM_MASK   (0x00000001u)
#define CSL_HDMI_TMDS_CTRL2_STERM_SHIFT  (0x00000000u)
#define CSL_HDMI_TMDS_CTRL2_STERM_RESETVAL (0x00000000u)

#define CSL_HDMI_TMDS_CTRL2_RESETVAL     (0x00000000u)

/* TMDS_CTRL3 */



#define CSL_HDMI_TMDS_CTRL3_ITPLL_MASK   (0x00000078u)
#define CSL_HDMI_TMDS_CTRL3_ITPLL_SHIFT  (0x00000003u)
#define CSL_HDMI_TMDS_CTRL3_ITPLL_RESETVAL (0x00000000u)

#define CSL_HDMI_TMDS_CTRL3_FPOST_COUNT_MASK (0x00000007u)
#define CSL_HDMI_TMDS_CTRL3_FPOST_COUNT_SHIFT (0x00000000u)
#define CSL_HDMI_TMDS_CTRL3_FPOST_COUNT_RESETVAL (0x00000000u)

#define CSL_HDMI_TMDS_CTRL3_RESETVAL     (0x00000000u)

/* TMDS_CTRL4 */



#define CSL_HDMI_TMDS_CTRL4_TFR_COUNT_MASK (0x00000007u)
#define CSL_HDMI_TMDS_CTRL4_TFR_COUNT_SHIFT (0x00000000u)
#define CSL_HDMI_TMDS_CTRL4_TFR_COUNT_RESETVAL (0x00000000u)

#define CSL_HDMI_TMDS_CTRL4_RESETVAL     (0x00000000u)

/* XVYCC2RGB_CTL */



#define CSL_HDMI_XVYCC2RGB_CTL_EXP_ONLY_MASK (0x00000060u)
#define CSL_HDMI_XVYCC2RGB_CTL_EXP_ONLY_SHIFT (0x00000005u)
#define CSL_HDMI_XVYCC2RGB_CTL_EXP_ONLY_RESETVAL (0x00000000u)

#define CSL_HDMI_XVYCC2RGB_CTL_BYP_ALL_MASK (0x00000018u)
#define CSL_HDMI_XVYCC2RGB_CTL_BYP_ALL_SHIFT (0x00000003u)
#define CSL_HDMI_XVYCC2RGB_CTL_BYP_ALL_RESETVAL (0x00000000u)

#define CSL_HDMI_XVYCC2RGB_CTL_SW_OVR_MASK (0x00000004u)
#define CSL_HDMI_XVYCC2RGB_CTL_SW_OVR_SHIFT (0x00000002u)
#define CSL_HDMI_XVYCC2RGB_CTL_SW_OVR_RESETVAL (0x00000000u)

#define CSL_HDMI_XVYCC2RGB_CTL_FULLRANGE_MASK (0x00000002u)
#define CSL_HDMI_XVYCC2RGB_CTL_FULLRANGE_SHIFT (0x00000001u)
#define CSL_HDMI_XVYCC2RGB_CTL_FULLRANGE_RESETVAL (0x00000000u)

#define CSL_HDMI_XVYCC2RGB_CTL_XVYCCSEL_MASK (0x00000001u)
#define CSL_HDMI_XVYCC2RGB_CTL_XVYCCSEL_SHIFT (0x00000000u)
#define CSL_HDMI_XVYCC2RGB_CTL_XVYCCSEL_RESETVAL (0x00000000u)

#define CSL_HDMI_XVYCC2RGB_CTL_RESETVAL  (0x00000000u)

/* Y2R_COEFF_LOW */


#define CSL_HDMI_Y2R_COEFF_LOW_Y2RCOEFF_L_MASK (0x000000FFu)
#define CSL_HDMI_Y2R_COEFF_LOW_Y2RCOEFF_L_SHIFT (0x00000000u)
#define CSL_HDMI_Y2R_COEFF_LOW_Y2RCOEFF_L_RESETVAL (0x00000000u)

#define CSL_HDMI_Y2R_COEFF_LOW_RESETVAL  (0x00000000u)

/* Y2R_COEFF_UP */



#define CSL_HDMI_Y2R_COEFF_UP_Y2RCOEFF_H_MASK (0x000000FFu)
#define CSL_HDMI_Y2R_COEFF_UP_Y2RCOEFF_H_SHIFT (0x00000000u)
#define CSL_HDMI_Y2R_COEFF_UP_Y2RCOEFF_H_RESETVAL (0x00000000u)

#define CSL_HDMI_Y2R_COEFF_UP_RESETVAL   (0x00000000u)

/* CR2R_COEFF_LOW */



#define CSL_HDMI_CR2R_COEFF_LOW_CR2RCOEFF_L_MASK (0x0000001Fu)
#define CSL_HDMI_CR2R_COEFF_LOW_CR2RCOEFF_L_SHIFT (0x00000000u)
#define CSL_HDMI_CR2R_COEFF_LOW_CR2RCOEFF_L_RESETVAL (0x00000000u)

#define CSL_HDMI_CR2R_COEFF_LOW_RESETVAL (0x00000000u)

/* CR2R_COEFF_UP */



#define CSL_HDMI_CR2R_COEFF_UP_CR2RCOEFF_H_MASK (0x000000FFu)
#define CSL_HDMI_CR2R_COEFF_UP_CR2RCOEFF_H_SHIFT (0x00000000u)
#define CSL_HDMI_CR2R_COEFF_UP_CR2RCOEFF_H_RESETVAL (0x00000000u)

#define CSL_HDMI_CR2R_COEFF_UP_RESETVAL  (0x00000000u)

/* CB2B_COEFF_LOW */



#define CSL_HDMI_CB2B_COEFF_LOW_CB2BCOEFF_L_MASK (0x0000001Fu)
#define CSL_HDMI_CB2B_COEFF_LOW_CB2BCOEFF_L_SHIFT (0x00000000u)
#define CSL_HDMI_CB2B_COEFF_LOW_CB2BCOEFF_L_RESETVAL (0x00000000u)

#define CSL_HDMI_CB2B_COEFF_LOW_RESETVAL (0x00000000u)

/* CB2B_COEFF_UP */


#define CSL_HDMI_CB2B_COEFF_UP_CB2BCOEFF_H_MASK (0x000000FFu)
#define CSL_HDMI_CB2B_COEFF_UP_CB2BCOEFF_H_SHIFT (0x00000000u)
#define CSL_HDMI_CB2B_COEFF_UP_CB2BCOEFF_H_RESETVAL (0x00000000u)

#define CSL_HDMI_CB2B_COEFF_UP_RESETVAL  (0x00000000u)

/* CR2G_COEFF_LOW */


#define CSL_HDMI_CR2G_COEFF_LOW_CR2GCOEFF_L_MASK (0x000000FFu)
#define CSL_HDMI_CR2G_COEFF_LOW_CR2GCOEFF_L_SHIFT (0x00000000u)
#define CSL_HDMI_CR2G_COEFF_LOW_CR2GCOEFF_L_RESETVAL (0x00000000u)

#define CSL_HDMI_CR2G_COEFF_LOW_RESETVAL (0x00000000u)

/* CR2G_COEFF_UP */


#define CSL_HDMI_CR2G_COEFF_UP_CR2GCOEFF_H_MASK (0x000000FFu)
#define CSL_HDMI_CR2G_COEFF_UP_CR2GCOEFF_H_SHIFT (0x00000000u)
#define CSL_HDMI_CR2G_COEFF_UP_CR2GCOEFF_H_RESETVAL (0x00000000u)

#define CSL_HDMI_CR2G_COEFF_UP_RESETVAL  (0x00000000u)

/* CB2G_COEFF_LOW */


#define CSL_HDMI_CB2G_COEFF_LOW_CR2GCOEFF_L_MASK (0x000000FFu)
#define CSL_HDMI_CB2G_COEFF_LOW_CR2GCOEFF_L_SHIFT (0x00000000u)
#define CSL_HDMI_CB2G_COEFF_LOW_CR2GCOEFF_L_RESETVAL (0x00000000u)

#define CSL_HDMI_CB2G_COEFF_LOW_RESETVAL (0x00000000u)

/* CB2G_COEFF_UP */



#define CSL_HDMI_CB2G_COEFF_UP_CB2GCOEFF_H_MASK (0x0000001Fu)
#define CSL_HDMI_CB2G_COEFF_UP_CB2GCOEFF_H_SHIFT (0x00000000u)
#define CSL_HDMI_CB2G_COEFF_UP_CB2GCOEFF_H_RESETVAL (0x00000000u)

#define CSL_HDMI_CB2G_COEFF_UP_RESETVAL  (0x00000000u)

/* YOFFSET1_LOW */


#define CSL_HDMI_YOFFSET1_LOW_YOFFS1_L_MASK (0x000000FFu)
#define CSL_HDMI_YOFFSET1_LOW_YOFFS1_L_SHIFT (0x00000000u)
#define CSL_HDMI_YOFFSET1_LOW_YOFFS1_L_RESETVAL (0x00000000u)

#define CSL_HDMI_YOFFSET1_LOW_RESETVAL   (0x00000000u)

/* YOFFSET1_UP */



#define CSL_HDMI_YOFFSET1_UP_YOFFS1_U_MASK (0x0000001Fu)
#define CSL_HDMI_YOFFSET1_UP_YOFFS1_U_SHIFT (0x00000000u)
#define CSL_HDMI_YOFFSET1_UP_YOFFS1_U_RESETVAL (0x00000000u)

#define CSL_HDMI_YOFFSET1_UP_RESETVAL    (0x00000000u)

/* OFFSET1_LOW */


#define CSL_HDMI_OFFSET1_LOW_OFFS1_L_MASK (0x000000FFu)
#define CSL_HDMI_OFFSET1_LOW_OFFS1_L_SHIFT (0x00000000u)
#define CSL_HDMI_OFFSET1_LOW_OFFS1_L_RESETVAL (0x00000000u)

#define CSL_HDMI_OFFSET1_LOW_RESETVAL    (0x00000000u)

/* OFFSET1_MID */


#define CSL_HDMI_OFFSET1_MID_OFFS1_M_MASK (0x000000FFu)
#define CSL_HDMI_OFFSET1_MID_OFFS1_M_SHIFT (0x00000000u)
#define CSL_HDMI_OFFSET1_MID_OFFS1_M_RESETVAL (0x00000000u)

#define CSL_HDMI_OFFSET1_MID_RESETVAL    (0x00000000u)

/* OFFSET1_UP */


#define CSL_HDMI_OFFSET1_UP_OFFS1_H_MASK (0x000000FFu)
#define CSL_HDMI_OFFSET1_UP_OFFS1_H_SHIFT (0x00000000u)
#define CSL_HDMI_OFFSET1_UP_OFFS1_H_RESETVAL (0x00000000u)

#define CSL_HDMI_OFFSET1_UP_RESETVAL     (0x00000000u)

/* OFFSET2_LOW */


#define CSL_HDMI_OFFSET2_LOW_OFFS2_L_MASK (0x000000FFu)
#define CSL_HDMI_OFFSET2_LOW_OFFS2_L_SHIFT (0x00000000u)
#define CSL_HDMI_OFFSET2_LOW_OFFS2_L_RESETVAL (0x00000000u)

#define CSL_HDMI_OFFSET2_LOW_RESETVAL    (0x00000000u)

/* OFFSET2_UP */


#define CSL_HDMI_OFFSET2_UP_OFFS2_H_MASK (0x0000000Fu)
#define CSL_HDMI_OFFSET2_UP_OFFS2_H_SHIFT (0x00000000u)
#define CSL_HDMI_OFFSET2_UP_OFFS2_H_RESETVAL (0x00000000u)

#define CSL_HDMI_OFFSET2_UP_RESETVAL    (0x00000000u)



/* DCLEVEL_LOW */


#define CSL_HDMI_DCLEVEL_LOW_DC_LEV_L_MASK (0x000000FFu)
#define CSL_HDMI_DCLEVEL_LOW_DC_LEV_L_SHIFT (0x00000000u)
#define CSL_HDMI_DCLEVEL_LOW_DC_LEV_L_RESETVAL (0x00000000u)

#define CSL_HDMI_DCLEVEL_LOW_RESETVAL    (0x00000000u)

/* DC_LEVEL_UP */




#define CSL_HDMI_DC_LEVEL_UP_DC_LEV_H_MASK (0x0000003Fu)
#define CSL_HDMI_DC_LEVEL_UP_DC_LEV_H_SHIFT (0x00000000u)
#define CSL_HDMI_DC_LEVEL_UP_DC_LEV_H_RESETVAL (0x00000000u)

#define CSL_HDMI_DC_LEVEL_UP_RESETVAL    (0x00000000u)

/* SHA_CTRL */



#define CSL_HDMI_SHA_CTRL_M0_RD_EN_MASK  (0x00000008u)
#define CSL_HDMI_SHA_CTRL_M0_RD_EN_SHIFT (0x00000003u)
#define CSL_HDMI_SHA_CTRL_M0_RD_EN_RESETVAL (0x00000000u)


#define CSL_HDMI_SHA_CTRL_RESETVAL       (0x00000000u)

/* DDC_MAN */


#define CSL_HDMI_DDC_MAN_MAN_OVR_MASK    (0x00000080u)
#define CSL_HDMI_DDC_MAN_MAN_OVR_SHIFT   (0x00000007u)
#define CSL_HDMI_DDC_MAN_MAN_OVR_RESETVAL (0x00000000u)


#define CSL_HDMI_DDC_MAN_MAN_SDA_MASK    (0x00000020u)
#define CSL_HDMI_DDC_MAN_MAN_SDA_SHIFT   (0x00000005u)
#define CSL_HDMI_DDC_MAN_MAN_SDA_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_MAN_MAN_SCL_MASK    (0x00000010u)
#define CSL_HDMI_DDC_MAN_MAN_SCL_SHIFT   (0x00000004u)
#define CSL_HDMI_DDC_MAN_MAN_SCL_RESETVAL (0x00000000u)


#define CSL_HDMI_DDC_MAN_IO_SCL_MASK     (0x00000002u)
#define CSL_HDMI_DDC_MAN_IO_SCL_SHIFT    (0x00000001u)
#define CSL_HDMI_DDC_MAN_IO_SCL_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_MAN_IO_SDA_MASK     (0x00000001u)
#define CSL_HDMI_DDC_MAN_IO_SDA_SHIFT    (0x00000000u)
#define CSL_HDMI_DDC_MAN_IO_SDA_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_MAN_RESETVAL        (0x00000000u)

/* DDC_ADDR */


#define CSL_HDMI_DDC_ADDR_DDC_ADDR_MASK  (0x000000FEu)
#define CSL_HDMI_DDC_ADDR_DDC_ADDR_SHIFT (0x00000001u)
#define CSL_HDMI_DDC_ADDR_DDC_ADDR_RESETVAL (0x00000000u)


#define CSL_HDMI_DDC_ADDR_RESETVAL       (0x00000000u)

/* DDC_SEGM */


#define CSL_HDMI_DDC_SEGM_DDC_SEGM_MASK  (0x000000FFu)
#define CSL_HDMI_DDC_SEGM_DDC_SEGM_SHIFT (0x00000000u)
#define CSL_HDMI_DDC_SEGM_DDC_SEGM_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_SEGM_RESETVAL       (0x00000000u)

/* DDC_OFFSET */


#define CSL_HDMI_DDC_OFFSET_DDC_OFFSET_MASK (0x000000FFu)
#define CSL_HDMI_DDC_OFFSET_DDC_OFFSET_SHIFT (0x00000000u)
#define CSL_HDMI_DDC_OFFSET_DDC_OFFSET_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_OFFSET_RESETVAL     (0x00000000u)

/* DDC_COUNT1 */


#define CSL_HDMI_DDC_COUNT1_DDC_COUNT_MASK (0x000000FFu)
#define CSL_HDMI_DDC_COUNT1_DDC_COUNT_SHIFT (0x00000000u)
#define CSL_HDMI_DDC_COUNT1_DDC_COUNT_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_COUNT1_RESETVAL     (0x00000000u)

/* DDC_COUNT2 */


#define CSL_HDMI_DDC_COUNT2_DDC_COUNT_MASK (0x00000003u)
#define CSL_HDMI_DDC_COUNT2_DDC_COUNT_SHIFT (0x00000000u)
#define CSL_HDMI_DDC_COUNT2_DDC_COUNT_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_COUNT2_RESETVAL     (0x00000000u)




/* DDC_STATUS */



#define CSL_HDMI_DDC_STATUS_BUS_LOW_MASK (0x00000040u)
#define CSL_HDMI_DDC_STATUS_BUS_LOW_SHIFT (0x00000006u)
#define CSL_HDMI_DDC_STATUS_BUS_LOW_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_STATUS_NO_ACK_MASK  (0x00000020u)
#define CSL_HDMI_DDC_STATUS_NO_ACK_SHIFT (0x00000005u)
#define CSL_HDMI_DDC_STATUS_NO_ACK_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_STATUS_IN_PROG_MASK (0x00000010u)
#define CSL_HDMI_DDC_STATUS_IN_PROG_SHIFT (0x00000004u)
#define CSL_HDMI_DDC_STATUS_IN_PROG_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_STATUS_FIFO_FULL_MASK (0x00000008u)
#define CSL_HDMI_DDC_STATUS_FIFO_FULL_SHIFT (0x00000003u)
#define CSL_HDMI_DDC_STATUS_FIFO_FULL_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_STATUS_FIFO_EMP_MASK (0x00000004u)
#define CSL_HDMI_DDC_STATUS_FIFO_EMP_SHIFT (0x00000002u)
#define CSL_HDMI_DDC_STATUS_FIFO_EMP_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_STATUS_FRD_USE_MASK (0x00000002u)
#define CSL_HDMI_DDC_STATUS_FRD_USE_SHIFT (0x00000001u)
#define CSL_HDMI_DDC_STATUS_FRD_USE_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_STATUS_FWT_USE_MASK (0x00000001u)
#define CSL_HDMI_DDC_STATUS_FWT_USE_SHIFT (0x00000000u)
#define CSL_HDMI_DDC_STATUS_FWT_USE_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_STATUS_RESETVAL     (0x00000000u)

/* DDC_CMD */



#define CSL_HDMI_DDC_CMD_DDC_FLT_EN_MASK (0x00000020u)
#define CSL_HDMI_DDC_CMD_DDC_FLT_EN_SHIFT (0x00000005u)
#define CSL_HDMI_DDC_CMD_DDC_FLT_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_CMD_SDA_DEL_EN_MASK (0x00000010u)
#define CSL_HDMI_DDC_CMD_SDA_DEL_EN_SHIFT (0x00000004u)
#define CSL_HDMI_DDC_CMD_SDA_DEL_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_CMD_DDC_CMD_MASK    (0x0000000Fu)
#define CSL_HDMI_DDC_CMD_DDC_CMD_SHIFT   (0x00000000u)
#define CSL_HDMI_DDC_CMD_DDC_CMD_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_CMD_RESETVAL        (0x00000000u)

/* Not Generated */
#define CSL_HDMI_DDC_CMD_CLOCK_SCL      (0x0000000Au)
#define CSL_HDMI_DDC_CMD_CLEAR_FIFO     (0x00000009u)
#define CSL_HDMI_DDC_CMD_SEQ_R_NO_ACK_ON_LAST_BYTE  (0x00000002u)
#define CSL_HDMI_DDC_CMD_EDDC_R_NO_ACK_ON_LAST_BYTE (0x00000004u)
#define CSL_HDMI_DDC_CMD_ABORT          (0x0000000Fu)

/* DDC_DATA */


#define CSL_HDMI_DDC_DATA_DDC_DATA_MASK  (0x000000FFu)
#define CSL_HDMI_DDC_DATA_DDC_DATA_SHIFT (0x00000000u)
#define CSL_HDMI_DDC_DATA_DDC_DATA_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_DATA_RESETVAL       (0x00000000u)

/* DDC_FIFOCNT */



#define CSL_HDMI_DDC_FIFOCNT_DDC_FIFOCNT_MASK (0x0000001Fu)
#define CSL_HDMI_DDC_FIFOCNT_DDC_FIFOCNT_SHIFT (0x00000000u)
#define CSL_HDMI_DDC_FIFOCNT_DDC_FIFOCNT_RESETVAL (0x00000000u)

#define CSL_HDMI_DDC_FIFOCNT_RESETVAL    (0x00000000u)

/* EPST */



#define CSL_HDMI_EPST_BIST2_ERR_MASK     (0x00000040u)
#define CSL_HDMI_EPST_BIST2_ERR_SHIFT    (0x00000006u)
#define CSL_HDMI_EPST_BIST2_ERR_RESETVAL (0x00000000u)

#define CSL_HDMI_EPST_BIST1_ERR_MASK     (0x00000020u)
#define CSL_HDMI_EPST_BIST1_ERR_SHIFT    (0x00000005u)
#define CSL_HDMI_EPST_BIST1_ERR_RESETVAL (0x00000000u)


#define CSL_HDMI_EPST_CRC_ERR_MASK       (0x00000002u)
#define CSL_HDMI_EPST_CRC_ERR_SHIFT      (0x00000001u)
#define CSL_HDMI_EPST_CRC_ERR_RESETVAL   (0x00000000u)

#define CSL_HDMI_EPST_CMDD_MASK          (0x00000001u)
#define CSL_HDMI_EPST_CMDD_SHIFT         (0x00000000u)
#define CSL_HDMI_EPST_CMDD_RESETVAL      (0x00000000u)

#define CSL_HDMI_EPST_RESETVAL           (0x00000000u)

/* EPCM */



#define CSL_HDMI_EPCM_LD_KSV_MASK        (0x00000020u)
#define CSL_HDMI_EPCM_LD_KSV_SHIFT       (0x00000005u)
#define CSL_HDMI_EPCM_LD_KSV_RESETVAL    (0x00000000u)

#define CSL_HDMI_EPCM_EPCM_MASK          (0x0000001Fu)
#define CSL_HDMI_EPCM_EPCM_SHIFT         (0x00000000u)
#define CSL_HDMI_EPCM_EPCM_RESETVAL      (0x00000000u)

#define CSL_HDMI_EPCM_RESETVAL           (0x00000000u)

/* REG_BANK0__1 */



#define CSL_HDMI_REG_BANK0__1_BANK_MASK  (0x00000020u)
#define CSL_HDMI_REG_BANK0__1_BANK_SHIFT (0x00000005u)
#define CSL_HDMI_REG_BANK0__1_BANK_RESETVAL (0x00000000u)


#define CSL_HDMI_REG_BANK0__1_RESETVAL   (0x00000000u)

/* GAMUT_HEADER1 */


#define CSL_HDMI_GAMUT_HEADER1_HEADER1_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_HEADER1_HEADER1_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_HEADER1_HEADER1_RESETVAL (0x0000000Au)

#define CSL_HDMI_GAMUT_HEADER1_RESETVAL  (0x0000000Au)

/* GAMUT_HEADER2 */


#define CSL_HDMI_GAMUT_HEADER2_NEXT_FIELD_MASK (0x00000080u)
#define CSL_HDMI_GAMUT_HEADER2_NEXT_FIELD_SHIFT (0x00000007u)
#define CSL_HDMI_GAMUT_HEADER2_NEXT_FIELD_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_HEADER2_GBD_PROFILE_MASK (0x00000070u)
#define CSL_HDMI_GAMUT_HEADER2_GBD_PROFILE_SHIFT (0x00000004u)
#define CSL_HDMI_GAMUT_HEADER2_GBD_PROFILE_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_HEADER2_AFF_GAM_SEQ_NUM_MASK (0x0000000Fu)
#define CSL_HDMI_GAMUT_HEADER2_AFF_GAM_SEQ_NUM_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_HEADER2_AFF_GAM_SEQ_NUM_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_HEADER2_RESETVAL  (0x00000000u)

/* GAMUT_HEADER3 */


#define CSL_HDMI_GAMUT_HEADER3_NO_CRNT_GBD_MASK (0x00000080u)
#define CSL_HDMI_GAMUT_HEADER3_NO_CRNT_GBD_SHIFT (0x00000007u)
#define CSL_HDMI_GAMUT_HEADER3_NO_CRNT_GBD_RESETVAL (0x00000000u)


#define CSL_HDMI_GAMUT_HEADER3_PACKET_SEQ_MASK (0x00000030u)
#define CSL_HDMI_GAMUT_HEADER3_PACKET_SEQ_SHIFT (0x00000004u)
#define CSL_HDMI_GAMUT_HEADER3_PACKET_SEQ_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_HEADER3_CUR_GAM_SEQ_NUM_MASK (0x0000000Fu)
#define CSL_HDMI_GAMUT_HEADER3_CUR_GAM_SEQ_NUM_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_HEADER3_CUR_GAM_SEQ_NUM_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_HEADER3_RESETVAL  (0x00000000u)

/* GAMUT_DBYTE */


#define CSL_HDMI_GAMUT_DBYTE_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_1_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_1_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_1_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_1_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_2_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_2_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_2_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_2_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_3_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_3_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_3_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_3_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_4_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_4_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_4_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_4_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_5_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_5_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_5_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_5_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_6_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_6_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_6_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_6_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_7_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_7_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_7_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_7_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_8_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_8_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_8_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_8_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_9_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_9_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_9_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_9_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_10_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_10_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_10_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_10_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_11_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_11_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_11_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_11_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_12_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_12_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_12_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_12_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_13_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_13_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_13_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_13_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_14_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_14_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_14_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_14_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_15_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_15_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_15_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_15_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_16_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_16_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_16_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_16_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_17_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_17_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_17_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_17_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_18_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_18_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_18_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_18_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_19_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_19_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_19_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_19_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_20_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_20_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_20_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_20_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_21_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_21_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_21_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_21_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_22_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_22_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_22_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_22_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_23_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_23_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_23_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_23_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_24_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_24_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_24_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_24_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_25_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_25_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_25_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_25_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_26_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_26_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_26_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_26_RESETVAL    (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_27_GAM_MDATA_MASK (0x000000FFu)
#define CSL_HDMI_GAMUT_DBYTE_27_GAM_MDATA_SHIFT (0x00000000u)
#define CSL_HDMI_GAMUT_DBYTE_27_GAM_MDATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GAMUT_DBYTE_27_RESETVAL    (0x00000000u)


/* ACR_CTRL */



#define CSL_HDMI_ACR_CTRL_NCTSPKT_EN_MASK (0x00000030u)
#define CSL_HDMI_ACR_CTRL_NCTSPKT_EN_SHIFT (0x00000004u)
#define CSL_HDMI_ACR_CTRL_NCTSPKT_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_ACR_CTRL_CTS_SEL_MASK   (0x0000000Fu)
#define CSL_HDMI_ACR_CTRL_CTS_SEL_SHIFT  (0x00000000u)
#define CSL_HDMI_ACR_CTRL_CTS_SEL_RESETVAL (0x00000000u)

#define CSL_HDMI_ACR_CTRL_RESETVAL       (0x00000000u)

/* FREQ_SVAL */



#define CSL_HDMI_FREQ_SVAL_MCLK_CONF_MASK (0x00000007u)
#define CSL_HDMI_FREQ_SVAL_MCLK_CONF_SHIFT (0x00000000u)
#define CSL_HDMI_FREQ_SVAL_MCLK_CONF_RESETVAL (0x00000000u)

#define CSL_HDMI_FREQ_SVAL_RESETVAL      (0x00000000u)

/* N_SVAL1 */


#define CSL_HDMI_N_SVAL1_N_SVAL_MASK     (0x000000FFu)
#define CSL_HDMI_N_SVAL1_N_SVAL_SHIFT    (0x00000000u)
#define CSL_HDMI_N_SVAL1_N_SVAL_RESETVAL (0x00000000u)

#define CSL_HDMI_N_SVAL1_RESETVAL        (0x00000000u)

/* N_SVAL2 */


#define CSL_HDMI_N_SVAL2_N_SVAL2_MASK    (0x000000FFu)
#define CSL_HDMI_N_SVAL2_N_SVAL2_SHIFT   (0x00000000u)
#define CSL_HDMI_N_SVAL2_N_SVAL2_RESETVAL (0x00000000u)

#define CSL_HDMI_N_SVAL2_RESETVAL        (0x00000000u)

/* N_SVAL3 */



#define CSL_HDMI_N_SVAL3_N_SVAL_MASK     (0x0000000Fu)
#define CSL_HDMI_N_SVAL3_N_SVAL_SHIFT    (0x00000000u)
#define CSL_HDMI_N_SVAL3_N_SVAL_RESETVAL (0x00000000u)

#define CSL_HDMI_N_SVAL3_RESETVAL        (0x00000000u)

/* CTS_SVAL1 */


#define CSL_HDMI_CTS_SVAL1_CTS_SVAL_MASK (0x000000FFu)
#define CSL_HDMI_CTS_SVAL1_CTS_SVAL_SHIFT (0x00000000u)
#define CSL_HDMI_CTS_SVAL1_CTS_SVAL_RESETVAL (0x00000000u)

#define CSL_HDMI_CTS_SVAL1_RESETVAL      (0x00000000u)

/* CTS_SVAL2 */


#define CSL_HDMI_CTS_SVAL2_CTS_SVAL_MASK (0x000000FFu)
#define CSL_HDMI_CTS_SVAL2_CTS_SVAL_SHIFT (0x00000000u)
#define CSL_HDMI_CTS_SVAL2_CTS_SVAL_RESETVAL (0x00000000u)

#define CSL_HDMI_CTS_SVAL2_RESETVAL      (0x00000000u)

/* CTS_SVAL3 */



#define CSL_HDMI_CTS_SVAL3_CTS_SVAL_MASK (0x0000000Fu)
#define CSL_HDMI_CTS_SVAL3_CTS_SVAL_SHIFT (0x00000000u)
#define CSL_HDMI_CTS_SVAL3_CTS_SVAL_RESETVAL (0x00000000u)

#define CSL_HDMI_CTS_SVAL3_RESETVAL      (0x00000000u)

/* CTS_HVAL1 */


#define CSL_HDMI_CTS_HVAL1_CTS_HVAL_MASK (0x000000FFu)
#define CSL_HDMI_CTS_HVAL1_CTS_HVAL_SHIFT (0x00000000u)
#define CSL_HDMI_CTS_HVAL1_CTS_HVAL_RESETVAL (0x00000000u)

#define CSL_HDMI_CTS_HVAL1_RESETVAL      (0x00000000u)

/* CTS_HVAL2 */


#define CSL_HDMI_CTS_HVAL2_CTS_HVAL_MASK (0x000000FFu)
#define CSL_HDMI_CTS_HVAL2_CTS_HVAL_SHIFT (0x00000000u)
#define CSL_HDMI_CTS_HVAL2_CTS_HVAL_RESETVAL (0x00000000u)

#define CSL_HDMI_CTS_HVAL2_RESETVAL      (0x00000000u)

/* CTS_HVAL3 */



#define CSL_HDMI_CTS_HVAL3_CTS_HVAL3_MASK (0x0000000Fu)
#define CSL_HDMI_CTS_HVAL3_CTS_HVAL3_SHIFT (0x00000000u)
#define CSL_HDMI_CTS_HVAL3_CTS_HVAL3_RESETVAL (0x00000000u)

#define CSL_HDMI_CTS_HVAL3_RESETVAL      (0x00000000u)

/* AUD_MODE */


#define CSL_HDMI_AUD_MODE_SD3_EN_MASK    (0x00000080u)
#define CSL_HDMI_AUD_MODE_SD3_EN_SHIFT   (0x00000007u)
#define CSL_HDMI_AUD_MODE_SD3_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_AUD_MODE_SD2_EN_MASK    (0x00000040u)
#define CSL_HDMI_AUD_MODE_SD2_EN_SHIFT   (0x00000006u)
#define CSL_HDMI_AUD_MODE_SD2_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_AUD_MODE_SD1_EN_MASK    (0x00000020u)
#define CSL_HDMI_AUD_MODE_SD1_EN_SHIFT   (0x00000005u)
#define CSL_HDMI_AUD_MODE_SD1_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_AUD_MODE_SD0_EN_MASK    (0x00000010u)
#define CSL_HDMI_AUD_MODE_SD0_EN_SHIFT   (0x00000004u)
#define CSL_HDMI_AUD_MODE_SD0_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_AUD_MODE_DSD_EN_MASK    (0x00000008u)
#define CSL_HDMI_AUD_MODE_DSD_EN_SHIFT   (0x00000003u)
#define CSL_HDMI_AUD_MODE_DSD_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_AUD_MODE_AUD_PAR_EN_MASK (0x00000004u)
#define CSL_HDMI_AUD_MODE_AUD_PAR_EN_SHIFT (0x00000002u)
#define CSL_HDMI_AUD_MODE_AUD_PAR_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_AUD_MODE_SPDIF_EN_MASK  (0x00000002u)
#define CSL_HDMI_AUD_MODE_SPDIF_EN_SHIFT (0x00000001u)
#define CSL_HDMI_AUD_MODE_SPDIF_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_AUD_MODE_AUD_EN_MASK    (0x00000001u)
#define CSL_HDMI_AUD_MODE_AUD_EN_SHIFT   (0x00000000u)
#define CSL_HDMI_AUD_MODE_AUD_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_AUD_MODE_RESETVAL       (0x00000000u)

/* SPDIF_CTRL */



#define CSL_HDMI_SPDIF_CTRL_NOAUDIO_MASK (0x00000008u)
#define CSL_HDMI_SPDIF_CTRL_NOAUDIO_SHIFT (0x00000003u)
#define CSL_HDMI_SPDIF_CTRL_NOAUDIO_RESETVAL (0x00000000u)


#define CSL_HDMI_SPDIF_CTRL_FS_OVERRIDE_MASK (0x00000002u)
#define CSL_HDMI_SPDIF_CTRL_FS_OVERRIDE_SHIFT (0x00000001u)
#define CSL_HDMI_SPDIF_CTRL_FS_OVERRIDE_RESETVAL (0x00000000u)


#define CSL_HDMI_SPDIF_CTRL_RESETVAL     (0x00000000u)

/* HW_SPDIF_FS */


#define CSL_HDMI_HW_SPDIF_FS_HW_SPDIF_LEN_MASK (0x000000E0u)
#define CSL_HDMI_HW_SPDIF_FS_HW_SPDIF_LEN_SHIFT (0x00000005u)
#define CSL_HDMI_HW_SPDIF_FS_HW_SPDIF_LEN_RESETVAL (0x00000000u)


#define CSL_HDMI_HW_SPDIF_FS_HW_SPDIF_FS_MASK (0x0000000Fu)
#define CSL_HDMI_HW_SPDIF_FS_HW_SPDIF_FS_SHIFT (0x00000000u)
#define CSL_HDMI_HW_SPDIF_FS_HW_SPDIF_FS_RESETVAL (0x00000000u)

#define CSL_HDMI_HW_SPDIF_FS_HW_MAXLEN_MASK (0x00000004u)
#define CSL_HDMI_HW_SPDIF_FS_HW_MAXLEN_SHIFT (0x00000002u)
#define CSL_HDMI_HW_SPDIF_FS_HW_MAXLEN_RESETVAL (0x00000000u)


#define CSL_HDMI_HW_SPDIF_FS_RESETVAL    (0x00000000u)

/* SWAP_I2S */


#define CSL_HDMI_SWAP_I2S_SWCH3_MASK     (0x00000080u)
#define CSL_HDMI_SWAP_I2S_SWCH3_SHIFT    (0x00000007u)
#define CSL_HDMI_SWAP_I2S_SWCH3_RESETVAL (0x00000000u)

#define CSL_HDMI_SWAP_I2S_SWCH2_MASK     (0x00000040u)
#define CSL_HDMI_SWAP_I2S_SWCH2_SHIFT    (0x00000006u)
#define CSL_HDMI_SWAP_I2S_SWCH2_RESETVAL (0x00000000u)

#define CSL_HDMI_SWAP_I2S_SWCH1_MASK     (0x00000020u)
#define CSL_HDMI_SWAP_I2S_SWCH1_SHIFT    (0x00000005u)
#define CSL_HDMI_SWAP_I2S_SWCH1_RESETVAL (0x00000000u)

#define CSL_HDMI_SWAP_I2S_SWCH0_MASK     (0x00000010u)
#define CSL_HDMI_SWAP_I2S_SWCH0_SHIFT    (0x00000004u)
#define CSL_HDMI_SWAP_I2S_SWCH0_RESETVAL (0x00000000u)


#define CSL_HDMI_SWAP_I2S_RESETVAL       (0x00000000u)

/* SPDIF_ERTH */



#define CSL_HDMI_SPDIF_ERTH_AUD_ERR_THRESH_MASK (0x0000003Fu)
#define CSL_HDMI_SPDIF_ERTH_AUD_ERR_THRESH_SHIFT (0x00000000u)
#define CSL_HDMI_SPDIF_ERTH_AUD_ERR_THRESH_RESETVAL (0x00000000u)

#define CSL_HDMI_SPDIF_ERTH_RESETVAL     (0x00000000u)

/* I2S_IN_MAP */


#define CSL_HDMI_I2S_IN_MAP_FIFO3_MAP_MASK     (0x000000C0u)
#define CSL_HDMI_I2S_IN_MAP_FIFO3_MAP_SHIFT    (0x00000006u)
#define CSL_HDMI_I2S_IN_MAP_FIFO3_MAP_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_IN_MAP_FIFO2_MAP_MASK     (0x00000030u)
#define CSL_HDMI_I2S_IN_MAP_FIFO2_MAP_SHIFT    (0x00000004u)
#define CSL_HDMI_I2S_IN_MAP_FIFO2_MAP_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_IN_MAP_FIFO1_MAP_MASK     (0x0000000Cu)
#define CSL_HDMI_I2S_IN_MAP_FIFO1_MAP_SHIFT    (0x00000002u)
#define CSL_HDMI_I2S_IN_MAP_FIFO1_MAP_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_IN_MAP_FIFO0_MAP_MASK     (0x00000003u)
#define CSL_HDMI_I2S_IN_MAP_FIFO0_MAP_SHIFT    (0x00000000u)
#define CSL_HDMI_I2S_IN_MAP_FIFO0_MAP_RESETVAL (0x00000000u)


#define CSL_HDMI_I2S_IN_MAP_RESETVAL       (0x00000000u)

/* I2S_IN_CTRL */


#define CSL_HDMI_I2S_IN_CTRL_HBRA_ON_MASK (0x00000080u)
#define CSL_HDMI_I2S_IN_CTRL_HBRA_ON_SHIFT (0x00000007u)
#define CSL_HDMI_I2S_IN_CTRL_HBRA_ON_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_IN_CTRL_SCK_EDGE_MASK (0x00000040u)
#define CSL_HDMI_I2S_IN_CTRL_SCK_EDGE_SHIFT (0x00000006u)
#define CSL_HDMI_I2S_IN_CTRL_SCK_EDGE_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_IN_CTRL_CBIT_ORDER_MASK (0x00000020u)
#define CSL_HDMI_I2S_IN_CTRL_CBIT_ORDER_SHIFT (0x00000005u)
#define CSL_HDMI_I2S_IN_CTRL_CBIT_ORDER_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_IN_CTRL_VBIT_MASK   (0x00000010u)
#define CSL_HDMI_I2S_IN_CTRL_VBIT_SHIFT  (0x00000004u)
#define CSL_HDMI_I2S_IN_CTRL_VBIT_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_IN_CTRL_I2S_WS_MASK (0x00000008u)
#define CSL_HDMI_I2S_IN_CTRL_I2S_WS_SHIFT (0x00000003u)
#define CSL_HDMI_I2S_IN_CTRL_I2S_WS_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_IN_CTRL_I2S_JUST_MASK (0x00000004u)
#define CSL_HDMI_I2S_IN_CTRL_I2S_JUST_SHIFT (0x00000002u)
#define CSL_HDMI_I2S_IN_CTRL_I2S_JUST_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_IN_CTRL_I2S_DIR_MASK (0x00000002u)
#define CSL_HDMI_I2S_IN_CTRL_I2S_DIR_SHIFT (0x00000001u)
#define CSL_HDMI_I2S_IN_CTRL_I2S_DIR_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_IN_CTRL_I2S_SHIFT_MASK (0x00000001u)
#define CSL_HDMI_I2S_IN_CTRL_I2S_SHIFT_SHIFT (0x00000000u)
#define CSL_HDMI_I2S_IN_CTRL_I2S_SHIFT_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_IN_CTRL_RESETVAL    (0x00000000u)

/* I2S_CHST0 */


#define CSL_HDMI_I2S_CHST0_I2S_CHST0_MASK (0x000000FFu)
#define CSL_HDMI_I2S_CHST0_I2S_CHST0_SHIFT (0x00000000u)
#define CSL_HDMI_I2S_CHST0_I2S_CHST0_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_CHST0_RESETVAL      (0x00000000u)

/* I2S_CHST1 */


#define CSL_HDMI_I2S_CHST1_I2S_CHST1_MASK (0x000000FFu)
#define CSL_HDMI_I2S_CHST1_I2S_CHST1_SHIFT (0x00000000u)
#define CSL_HDMI_I2S_CHST1_I2S_CHST1_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_CHST1_RESETVAL      (0x00000000u)

/* I2S_CHST2 */


#define CSL_HDMI_I2S_CHST2_I2S_CHAN_NUM_MASK (0x000000F0u)
#define CSL_HDMI_I2S_CHST2_I2S_CHAN_NUM_SHIFT (0x00000004u)
#define CSL_HDMI_I2S_CHST2_I2S_CHAN_NUM_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_CHST2_I2S_SRC_NUM_MASK (0x0000000Fu)
#define CSL_HDMI_I2S_CHST2_I2S_SRC_NUM_SHIFT (0x00000000u)
#define CSL_HDMI_I2S_CHST2_I2S_SRC_NUM_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_CHST2_RESETVAL      (0x00000000u)

/* I2S_CHST4 */


#define CSL_HDMI_I2S_CHST4_CLK_ACCUR_MASK (0x000000F0u)
#define CSL_HDMI_I2S_CHST4_CLK_ACCUR_SHIFT (0x00000004u)
#define CSL_HDMI_I2S_CHST4_CLK_ACCUR_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_CHST4_SW_SPDIF_FS_MASK (0x0000000Fu)
#define CSL_HDMI_I2S_CHST4_SW_SPDIF_FS_SHIFT (0x00000000u)
#define CSL_HDMI_I2S_CHST4_SW_SPDIF_FS_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_CHST4_RESETVAL      (0x00000000u)

/* I2S_CHST5 */


#define CSL_HDMI_I2S_CHST5_FS_ORIG_MASK  (0x000000F0u)
#define CSL_HDMI_I2S_CHST5_FS_ORIG_SHIFT (0x00000004u)
#define CSL_HDMI_I2S_CHST5_FS_ORIG_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_CHST5_I2S_LEN_MASK  (0x0000000Eu)
#define CSL_HDMI_I2S_CHST5_I2S_LEN_SHIFT (0x00000001u)
#define CSL_HDMI_I2S_CHST5_I2S_LEN_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_CHST5_I2S_MAXLEN_MASK (0x00000001u)
#define CSL_HDMI_I2S_CHST5_I2S_MAXLEN_SHIFT (0x00000000u)
#define CSL_HDMI_I2S_CHST5_I2S_MAXLEN_RESETVAL (0x00000000u)

#define CSL_HDMI_I2S_CHST5_RESETVAL      (0x00000000u)

/* ASRC */


#define CSL_HDMI_ASRC_HBR_SPR_MASK_MASK  (0x000000F0u)
#define CSL_HDMI_ASRC_HBR_SPR_MASK_SHIFT (0x00000004u)
#define CSL_HDMI_ASRC_HBR_SPR_MASK_RESETVAL (0x00000000u)


#define CSL_HDMI_ASRC_RATIO_MASK         (0x00000002u)
#define CSL_HDMI_ASRC_RATIO_SHIFT        (0x00000001u)
#define CSL_HDMI_ASRC_RATIO_RESETVAL     (0x00000000u)

#define CSL_HDMI_ASRC_SRC_EN_MASK        (0x00000001u)
#define CSL_HDMI_ASRC_SRC_EN_SHIFT       (0x00000000u)
#define CSL_HDMI_ASRC_SRC_EN_RESETVAL    (0x00000000u)

#define CSL_HDMI_ASRC_RESETVAL           (0x00000000u)

/* I2S_IN_LEN */


#define CSL_HDMI_I2S_IN_LEN_HDR_PKT_ID_MASK (0x000000F0u)
#define CSL_HDMI_I2S_IN_LEN_HDR_PKT_ID_SHIFT (0x00000004u)
#define CSL_HDMI_I2S_IN_LEN_HDR_PKT_ID_RESETVAL (0x00000009u)

#define CSL_HDMI_I2S_IN_LEN_IN_LENGTH_MASK (0x0000000Fu)
#define CSL_HDMI_I2S_IN_LEN_IN_LENGTH_SHIFT (0x00000000u)
#define CSL_HDMI_I2S_IN_LEN_IN_LENGTH_RESETVAL (0x0000000Bu)

#define CSL_HDMI_I2S_IN_LEN_RESETVAL     (0x0000009Bu)

/* HDMI_CTRL */



#define CSL_HDMI_HDMI_CTRL_DC_EN_MASK    (0x00000040u)
#define CSL_HDMI_HDMI_CTRL_DC_EN_SHIFT   (0x00000006u)
#define CSL_HDMI_HDMI_CTRL_DC_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_HDMI_CTRL_PACKET_MODE_MASK (0x00000038u)
#define CSL_HDMI_HDMI_CTRL_PACKET_MODE_SHIFT (0x00000003u)
#define CSL_HDMI_HDMI_CTRL_PACKET_MODE_RESETVAL (0x00000000u)

#define CSL_HDMI_HDMI_CTRL_LAYOUT_MASK   (0x00000006u)
#define CSL_HDMI_HDMI_CTRL_LAYOUT_SHIFT  (0x00000001u)
#define CSL_HDMI_HDMI_CTRL_LAYOUT_RESETVAL (0x00000000u)

#define CSL_HDMI_HDMI_CTRL_HDMI_MODE_MASK (0x00000001u)
#define CSL_HDMI_HDMI_CTRL_HDMI_MODE_SHIFT (0x00000000u)
#define CSL_HDMI_HDMI_CTRL_HDMI_MODE_RESETVAL (0x00000000u)

#define CSL_HDMI_HDMI_CTRL_RESETVAL      (0x00000000u)

/* AUDIO_TXSTAT */



#define CSL_HDMI_AUDIO_TXSTAT_MUTE_MASK  (0x00000004u)
#define CSL_HDMI_AUDIO_TXSTAT_MUTE_SHIFT (0x00000002u)
#define CSL_HDMI_AUDIO_TXSTAT_MUTE_RESETVAL (0x00000000u)

#define CSL_HDMI_AUDIO_TXSTAT_NPACKET_EN_VS_HIGH_MASK (0x00000002u)
#define CSL_HDMI_AUDIO_TXSTAT_NPACKET_EN_VS_HIGH_SHIFT (0x00000001u)
#define CSL_HDMI_AUDIO_TXSTAT_NPACKET_EN_VS_HIGH_RESETVAL (0x00000000u)

#define CSL_HDMI_AUDIO_TXSTAT_NPACKET_EN_MASK (0x00000001u)
#define CSL_HDMI_AUDIO_TXSTAT_NPACKET_EN_SHIFT (0x00000000u)
#define CSL_HDMI_AUDIO_TXSTAT_NPACKET_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_AUDIO_TXSTAT_RESETVAL   (0x00000000u)

/* AUD_PAR_BUSCLK_1 */


#define CSL_HDMI_AUD_PAR_BUSCLK_1_AUD_PAR_BUSCLK_1_MASK (0x000000FFu)
#define CSL_HDMI_AUD_PAR_BUSCLK_1_AUD_PAR_BUSCLK_1_SHIFT (0x00000000u)
#define CSL_HDMI_AUD_PAR_BUSCLK_1_AUD_PAR_BUSCLK_1_RESETVAL (0x00000000u)

#define CSL_HDMI_AUD_PAR_BUSCLK_1_RESETVAL (0x00000000u)

/* AUD_PAR_BUSCLK_2 */


#define CSL_HDMI_AUD_PAR_BUSCLK_2_AUD_PAR_BUSCLK_2_MASK (0x000000FFu)
#define CSL_HDMI_AUD_PAR_BUSCLK_2_AUD_PAR_BUSCLK_2_SHIFT (0x00000000u)
#define CSL_HDMI_AUD_PAR_BUSCLK_2_AUD_PAR_BUSCLK_2_RESETVAL (0x00000000u)

#define CSL_HDMI_AUD_PAR_BUSCLK_2_RESETVAL (0x00000000u)

/* AUD_PAR_BUSCLK_3 */


#define CSL_HDMI_AUD_PAR_BUSCLK_3_AUD_PAR_BUSCLK_3_MASK (0x000000FFu)
#define CSL_HDMI_AUD_PAR_BUSCLK_3_AUD_PAR_BUSCLK_3_SHIFT (0x00000000u)
#define CSL_HDMI_AUD_PAR_BUSCLK_3_AUD_PAR_BUSCLK_3_RESETVAL (0x00000000u)

#define CSL_HDMI_AUD_PAR_BUSCLK_3_RESETVAL (0x00000000u)

/* TEST_TXCTRL */




#define CSL_HDMI_TEST_TXCTRL_DIV_ENC_BYP_MASK (0x00000008u)     /* Sujith Updated from 4 to 8 */
#define CSL_HDMI_TEST_TXCTRL_DIV_ENC_BYP_SHIFT (0x00000003u)    /* Sujith updated form 2 to 3 */
#define CSL_HDMI_TEST_TXCTRL_DIV_ENC_BYP_RESETVAL (0x00000000u)

#define CSL_HDMI_TEST_TXCTRL_CORE_ISO_EN_MASK (0x00000004u)     /* Sujith updated from 2 to 4 */
#define CSL_HDMI_TEST_TXCTRL_CORE_ISO_EN_SHIFT (0x00000002u)    /* Sujith updated from 1 to 2 */
#define CSL_HDMI_TEST_TXCTRL_CORE_ISO_EN_RESETVAL (0x00000000u)


#define CSL_HDMI_TEST_TXCTRL_RESETVAL    (0x00000000u)

/* DPD */


#define CSL_HDMI_DPD_VID_BYP_EN_MASK     (0x00000080u)
#define CSL_HDMI_DPD_VID_BYP_EN_SHIFT    (0x00000007u)
#define CSL_HDMI_DPD_VID_BYP_EN_RESETVAL (0x00000000u)


#define CSL_HDMI_DPD_TCLKPHZ_MASK        (0x00000008u)
#define CSL_HDMI_DPD_TCLKPHZ_SHIFT       (0x00000003u)
#define CSL_HDMI_DPD_TCLKPHZ_RESETVAL    (0x00000000u)

#define CSL_HDMI_DPD_PDIDCK_MASK         (0x00000004u)
#define CSL_HDMI_DPD_PDIDCK_SHIFT        (0x00000002u)
#define CSL_HDMI_DPD_PDIDCK_RESETVAL     (0x00000000u)

#define CSL_HDMI_DPD_PDOSC_MASK          (0x00000002u)
#define CSL_HDMI_DPD_PDOSC_SHIFT         (0x00000001u)
#define CSL_HDMI_DPD_PDOSC_RESETVAL      (0x00000000u)

#define CSL_HDMI_DPD_PDTOT_MASK          (0x00000001u)
#define CSL_HDMI_DPD_PDTOT_SHIFT         (0x00000000u)
#define CSL_HDMI_DPD_PDTOT_RESETVAL      (0x00000000u)

#define CSL_HDMI_DPD_RESETVAL            (0x00000000u)

/* PB_CTRL1 */


#define CSL_HDMI_PB_CTRL1_MPEG_EN_MASK   (0x00000080u)
#define CSL_HDMI_PB_CTRL1_MPEG_EN_SHIFT  (0x00000007u)
#define CSL_HDMI_PB_CTRL1_MPEG_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_PB_CTRL1_MPEG_RPT_MASK  (0x00000040u)
#define CSL_HDMI_PB_CTRL1_MPEG_RPT_SHIFT (0x00000006u)
#define CSL_HDMI_PB_CTRL1_MPEG_RPT_RESETVAL (0x00000000u)

#define CSL_HDMI_PB_CTRL1_AUD_EN_MASK    (0x00000020u)
#define CSL_HDMI_PB_CTRL1_AUD_EN_SHIFT   (0x00000005u)
#define CSL_HDMI_PB_CTRL1_AUD_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_PB_CTRL1_AUD_RPT_MASK   (0x00000010u)
#define CSL_HDMI_PB_CTRL1_AUD_RPT_SHIFT  (0x00000004u)
#define CSL_HDMI_PB_CTRL1_AUD_RPT_RESETVAL (0x00000000u)

#define CSL_HDMI_PB_CTRL1_SPD_EN_MASK    (0x00000008u)
#define CSL_HDMI_PB_CTRL1_SPD_EN_SHIFT   (0x00000003u)
#define CSL_HDMI_PB_CTRL1_SPD_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_PB_CTRL1_SPD_RPT_MASK   (0x00000004u)
#define CSL_HDMI_PB_CTRL1_SPD_RPT_SHIFT  (0x00000002u)
#define CSL_HDMI_PB_CTRL1_SPD_RPT_RESETVAL (0x00000000u)

#define CSL_HDMI_PB_CTRL1_AVI_EN_MASK    (0x00000002u)
#define CSL_HDMI_PB_CTRL1_AVI_EN_SHIFT   (0x00000001u)
#define CSL_HDMI_PB_CTRL1_AVI_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_PB_CTRL1_AVI_RPT_MASK   (0x00000001u)
#define CSL_HDMI_PB_CTRL1_AVI_RPT_SHIFT  (0x00000000u)
#define CSL_HDMI_PB_CTRL1_AVI_RPT_RESETVAL (0x00000000u)

#define CSL_HDMI_PB_CTRL1_RESETVAL       (0x00000000u)

/* PB_CTRL2 */


#define CSL_HDMI_PB_CTRL2_GAM_EN_MASK    (0x00000080u)
#define CSL_HDMI_PB_CTRL2_GAM_EN_SHIFT   (0x00000007u)
#define CSL_HDMI_PB_CTRL2_GAM_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_PB_CTRL2_GAM_RPT_MASK   (0x00000040u)
#define CSL_HDMI_PB_CTRL2_GAM_RPT_SHIFT  (0x00000006u)
#define CSL_HDMI_PB_CTRL2_GAM_RPT_RESETVAL (0x00000000u)

#define CSL_HDMI_PB_CTRL2_GEN2_EN_MASK   (0x00000020u)
#define CSL_HDMI_PB_CTRL2_GEN2_EN_SHIFT  (0x00000005u)
#define CSL_HDMI_PB_CTRL2_GEN2_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_PB_CTRL2_GEN2_RPT_MASK  (0x00000010u)
#define CSL_HDMI_PB_CTRL2_GEN2_RPT_SHIFT (0x00000004u)
#define CSL_HDMI_PB_CTRL2_GEN2_RPT_RESETVAL (0x00000000u)

#define CSL_HDMI_PB_CTRL2_CP_EN_MASK     (0x00000008u)
#define CSL_HDMI_PB_CTRL2_CP_EN_SHIFT    (0x00000003u)
#define CSL_HDMI_PB_CTRL2_CP_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_PB_CTRL2_CP_RPT_MASK    (0x00000004u)
#define CSL_HDMI_PB_CTRL2_CP_RPT_SHIFT   (0x00000002u)
#define CSL_HDMI_PB_CTRL2_CP_RPT_RESETVAL (0x00000000u)

#define CSL_HDMI_PB_CTRL2_GEN_EN_MASK    (0x00000002u)
#define CSL_HDMI_PB_CTRL2_GEN_EN_SHIFT   (0x00000001u)
#define CSL_HDMI_PB_CTRL2_GEN_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_PB_CTRL2_GEN_RPT_MASK   (0x00000001u)
#define CSL_HDMI_PB_CTRL2_GEN_RPT_SHIFT  (0x00000000u)
#define CSL_HDMI_PB_CTRL2_GEN_RPT_RESETVAL (0x00000000u)

#define CSL_HDMI_PB_CTRL2_RESETVAL       (0x00000000u)

/* AVI_TYPE */


#define CSL_HDMI_AVI_TYPE_AVI_TYPE_MASK  (0x000000FFu)
#define CSL_HDMI_AVI_TYPE_AVI_TYPE_SHIFT (0x00000000u)
#define CSL_HDMI_AVI_TYPE_AVI_TYPE_RESETVAL (0x00000000u)

#define CSL_HDMI_AVI_TYPE_RESETVAL       (0x00000000u)

/* AVI_VERS */


#define CSL_HDMI_AVI_VERS_AVI_VERS_MASK  (0x000000FFu)
#define CSL_HDMI_AVI_VERS_AVI_VERS_SHIFT (0x00000000u)
#define CSL_HDMI_AVI_VERS_AVI_VERS_RESETVAL (0x00000000u)

#define CSL_HDMI_AVI_VERS_RESETVAL       (0x00000000u)

/* AVI_LEN */


#define CSL_HDMI_AVI_LEN_AVI_LEN_MASK    (0x000000FFu)
#define CSL_HDMI_AVI_LEN_AVI_LEN_SHIFT   (0x00000000u)
#define CSL_HDMI_AVI_LEN_AVI_LEN_RESETVAL (0x00000000u)

#define CSL_HDMI_AVI_LEN_RESETVAL        (0x00000000u)

/* AVI_CHSUM */


#define CSL_HDMI_AVI_CHSUM_AVI_CHSUM_MASK (0x000000FFu)
#define CSL_HDMI_AVI_CHSUM_AVI_CHSUM_SHIFT (0x00000000u)
#define CSL_HDMI_AVI_CHSUM_AVI_CHSUM_RESETVAL (0x00000000u)

#define CSL_HDMI_AVI_CHSUM_RESETVAL      (0x00000000u)

/* AVI_DBYTE */


#define CSL_HDMI_AVI_DBYTE_AVI_DATA_MASK (0x000000FFu)
#define CSL_HDMI_AVI_DBYTE_AVI_DATA_SHIFT (0x00000000u)
#define CSL_HDMI_AVI_DBYTE_AVI_DATA_RESETVAL (0x00000000u)

#define CSL_HDMI_AVI_DBYTE_RESETVAL      (0x00000000u)

/* SPD_TYPE */


#define CSL_HDMI_SPD_TYPE_SPD_TYPE_MASK  (0x000000FFu)
#define CSL_HDMI_SPD_TYPE_SPD_TYPE_SHIFT (0x00000000u)
#define CSL_HDMI_SPD_TYPE_SPD_TYPE_RESETVAL (0x00000000u)

#define CSL_HDMI_SPD_TYPE_RESETVAL       (0x00000000u)

/* SPD_VERS */


#define CSL_HDMI_SPD_VERS_SPD_VERS_MASK  (0x000000FFu)
#define CSL_HDMI_SPD_VERS_SPD_VERS_SHIFT (0x00000000u)
#define CSL_HDMI_SPD_VERS_SPD_VERS_RESETVAL (0x00000000u)

#define CSL_HDMI_SPD_VERS_RESETVAL       (0x00000000u)

/* SPD_LEN */


#define CSL_HDMI_SPD_LEN_SPD_LEN_MASK    (0x000000FFu)
#define CSL_HDMI_SPD_LEN_SPD_LEN_SHIFT   (0x00000000u)
#define CSL_HDMI_SPD_LEN_SPD_LEN_RESETVAL (0x00000000u)

#define CSL_HDMI_SPD_LEN_RESETVAL        (0x00000000u)

/* SPD_CHSUM */


#define CSL_HDMI_SPD_CHSUM_SPD_CHSUM_MASK (0x000000FFu)
#define CSL_HDMI_SPD_CHSUM_SPD_CHSUM_SHIFT (0x00000000u)
#define CSL_HDMI_SPD_CHSUM_SPD_CHSUM_RESETVAL (0x00000000u)

#define CSL_HDMI_SPD_CHSUM_RESETVAL      (0x00000000u)

/* SPD_DBYTE */


#define CSL_HDMI_SPD_DBYTE_SPD_DATA_MASK (0x000000FFu)
#define CSL_HDMI_SPD_DBYTE_SPD_DATA_SHIFT (0x00000000u)
#define CSL_HDMI_SPD_DBYTE_SPD_DATA_RESETVAL (0x00000000u)

#define CSL_HDMI_SPD_DBYTE_RESETVAL      (0x00000000u)

/* AUDIO_TYPE */


#define CSL_HDMI_AUDIO_TYPE_AUDIO_TYPE_MASK (0x000000FFu)
#define CSL_HDMI_AUDIO_TYPE_AUDIO_TYPE_SHIFT (0x00000000u)
#define CSL_HDMI_AUDIO_TYPE_AUDIO_TYPE_RESETVAL (0x00000000u)

#define CSL_HDMI_AUDIO_TYPE_RESETVAL     (0x00000000u)

/* AUDIO_VERS */


#define CSL_HDMI_AUDIO_VERS_AUDIO_VERS_MASK (0x000000FFu)
#define CSL_HDMI_AUDIO_VERS_AUDIO_VERS_SHIFT (0x00000000u)
#define CSL_HDMI_AUDIO_VERS_AUDIO_VERS_RESETVAL (0x00000000u)

#define CSL_HDMI_AUDIO_VERS_RESETVAL     (0x00000000u)

/* AUDIO_LEN */


#define CSL_HDMI_AUDIO_LEN_AUDIO_LEN_MASK (0x000000FFu)
#define CSL_HDMI_AUDIO_LEN_AUDIO_LEN_SHIFT (0x00000000u)
#define CSL_HDMI_AUDIO_LEN_AUDIO_LEN_RESETVAL (0x00000000u)

#define CSL_HDMI_AUDIO_LEN_RESETVAL      (0x00000000u)

/* AUDIO_CHSUM */


#define CSL_HDMI_AUDIO_CHSUM_AUDIO_CHSUM_MASK (0x000000FFu)
#define CSL_HDMI_AUDIO_CHSUM_AUDIO_CHSUM_SHIFT (0x00000000u)
#define CSL_HDMI_AUDIO_CHSUM_AUDIO_CHSUM_RESETVAL (0x00000000u)

#define CSL_HDMI_AUDIO_CHSUM_RESETVAL    (0x00000000u)

/* AUDIO_DBYTE */


#define CSL_HDMI_AUDIO_DBYTE_AUDIO_DATA_MASK (0x000000FFu)
#define CSL_HDMI_AUDIO_DBYTE_AUDIO_DATA_SHIFT (0x00000000u)
#define CSL_HDMI_AUDIO_DBYTE_AUDIO_DATA_RESETVAL (0x00000000u)

#define CSL_HDMI_AUDIO_DBYTE_RESETVAL    (0x00000000u)

/* MPEG_TYPE */


#define CSL_HDMI_MPEG_TYPE_AUDIO_LEN_MASK (0x000000FFu)
#define CSL_HDMI_MPEG_TYPE_AUDIO_LEN_SHIFT (0x00000000u)
#define CSL_HDMI_MPEG_TYPE_AUDIO_LEN_RESETVAL (0x00000000u)

#define CSL_HDMI_MPEG_TYPE_RESETVAL      (0x00000000u)

/* MPEG_VERS */


#define CSL_HDMI_MPEG_VERS_MPEG_VERS_MASK (0x000000FFu)
#define CSL_HDMI_MPEG_VERS_MPEG_VERS_SHIFT (0x00000000u)
#define CSL_HDMI_MPEG_VERS_MPEG_VERS_RESETVAL (0x00000000u)

#define CSL_HDMI_MPEG_VERS_RESETVAL      (0x00000000u)

/* MPEG_LEN */


#define CSL_HDMI_MPEG_LEN_MPEG_LEN_MASK  (0x000000FFu)
#define CSL_HDMI_MPEG_LEN_MPEG_LEN_SHIFT (0x00000000u)
#define CSL_HDMI_MPEG_LEN_MPEG_LEN_RESETVAL (0x00000000u)

#define CSL_HDMI_MPEG_LEN_RESETVAL       (0x00000000u)

/* MPEG_CHSUM */


#define CSL_HDMI_MPEG_CHSUM_MPEG_CHSUM_MASK (0x000000FFu)
#define CSL_HDMI_MPEG_CHSUM_MPEG_CHSUM_SHIFT (0x00000000u)
#define CSL_HDMI_MPEG_CHSUM_MPEG_CHSUM_RESETVAL (0x00000000u)

#define CSL_HDMI_MPEG_CHSUM_RESETVAL     (0x00000000u)

/* MPEG_DBYTE */


#define CSL_HDMI_MPEG_DBYTE_MPEG_DATA_MASK (0x000000FFu)
#define CSL_HDMI_MPEG_DBYTE_MPEG_DATA_SHIFT (0x00000000u)
#define CSL_HDMI_MPEG_DBYTE_MPEG_DATA_RESETVAL (0x00000000u)

#define CSL_HDMI_MPEG_DBYTE_RESETVAL     (0x00000000u)

/* GEN_DBYTE */


#define CSL_HDMI_GEN_DBYTE_GEN_DATA_MASK (0x000000FFu)
#define CSL_HDMI_GEN_DBYTE_GEN_DATA_SHIFT (0x00000000u)
#define CSL_HDMI_GEN_DBYTE_GEN_DATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GEN_DBYTE_RESETVAL      (0x00000000u)

/* CP_BYTE1 */



#define CSL_HDMI_CP_BYTE1_CLRAVM_MASK    (0x00000010u)
#define CSL_HDMI_CP_BYTE1_CLRAVM_SHIFT   (0x00000004u)
#define CSL_HDMI_CP_BYTE1_CLRAVM_RESETVAL (0x00000000u)

#define CSL_HDMI_CP_BYTE1_SETAVM_MASK    (0x00000010u)
#define CSL_HDMI_CP_BYTE1_SETAVM_SHIFT   (0x00000004u)
#define CSL_HDMI_CP_BYTE1_SETAVM_RESETVAL (0x00000000u)



#define CSL_HDMI_CP_BYTE1_RESETVAL       (0x00000000u)

/* GEN2_DBYTE */


#define CSL_HDMI_GEN2_DBYTE_GEN2_DATA_MASK (0x000000FFu)
#define CSL_HDMI_GEN2_DBYTE_GEN2_DATA_SHIFT (0x00000000u)
#define CSL_HDMI_GEN2_DBYTE_GEN2_DATA_RESETVAL (0x00000000u)

#define CSL_HDMI_GEN2_DBYTE_RESETVAL     (0x00000000u)

/* CEC_ADDR_ID */


#define CSL_HDMI_CEC_ADDR_ID_CEC_ID_MASK (0x000000FFu)
#define CSL_HDMI_CEC_ADDR_ID_CEC_ID_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_ADDR_ID_CEC_ID_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_ADDR_ID_RESETVAL    (0x00000000u)

/* CEC_DEV_ID */


#define CSL_HDMI_CEC_DEV_ID_CEC_DEV_ID_MASK (0x000000FFu)
#define CSL_HDMI_CEC_DEV_ID_CEC_DEV_ID_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_DEV_ID_CEC_DEV_ID_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_DEV_ID_RESETVAL     (0x00000000u)

/* CEC_SPEC */


#define CSL_HDMI_CEC_SPEC_CEC_REL_MASK   (0x000000F0u)
#define CSL_HDMI_CEC_SPEC_CEC_REL_SHIFT  (0x00000004u)
#define CSL_HDMI_CEC_SPEC_CEC_REL_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_SPEC_CEC_REV_MASK   (0x0000000Fu)
#define CSL_HDMI_CEC_SPEC_CEC_REV_SHIFT  (0x00000000u)
#define CSL_HDMI_CEC_SPEC_CEC_REV_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_SPEC_RESETVAL       (0x00000000u)

/* CEC_SUFF */


#define CSL_HDMI_CEC_SUFF_SUB_SYS_MASK   (0x000000F0u)
#define CSL_HDMI_CEC_SUFF_SUB_SYS_SHIFT  (0x00000004u)
#define CSL_HDMI_CEC_SUFF_SUB_SYS_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_SUFF_CEC_SUFF_MASK   (0x0000000Fu)
#define CSL_HDMI_CEC_SUFF_CEC_SUFF_SHIFT  (0x00000000u)
#define CSL_HDMI_CEC_SUFF_CEC_SUFF_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_SUFF_RESETVAL       (0x00000000u)


/* CEC_FW */


#define CSL_HDMI_CEC_FW_FW_REL_ID_MASK   (0x000000F0u)
#define CSL_HDMI_CEC_FW_FW_REL_ID_SHIFT  (0x00000004u)
#define CSL_HDMI_CEC_FW_FW_REL_ID_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_FW_FW_REV_ID_MASK   (0x0000000Fu)
#define CSL_HDMI_CEC_FW_FW_REV_ID_SHIFT  (0x00000000u)
#define CSL_HDMI_CEC_FW_FW_REV_ID_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_FW_RESETVAL         (0x00000000u)

/* CEC_DBG_0 */


#define CSL_HDMI_CEC_DBG_0_STB_LOW_P_MASK (0x000000FFu)
#define CSL_HDMI_CEC_DBG_0_STB_LOW_P_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_DBG_0_STB_LOW_P_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_DBG_0_RESETVAL      (0x00000000u)

/* CEC_DBG_1 */


#define CSL_HDMI_CEC_DBG_1_STB_DUR_P_MASK (0x000000FFu)
#define CSL_HDMI_CEC_DBG_1_STB_DUR_P_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_DBG_1_STB_DUR_P_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_DBG_1_RESETVAL      (0x00000000u)

/* CEC_DBG_2 */


#define CSL_HDMI_CEC_DBG_2_CEC_SN_INI_MASK (0x000000F0u)
#define CSL_HDMI_CEC_DBG_2_CEC_SN_INI_SHIFT (0x00000004u)
#define CSL_HDMI_CEC_DBG_2_CEC_SN_INI_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_DBG_2_CEC_BUS_OWN_MASK (0x0000000Fu)
#define CSL_HDMI_CEC_DBG_2_CEC_BUS_OWN_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_DBG_2_CEC_BUS_OWN_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_DBG_2_RESETVAL      (0x00000000u)

/* CEC_DBG_3 */


#define CSL_HDMI_CEC_DBG_3_FL_FIF_MASK   (0x00000080u)
#define CSL_HDMI_CEC_DBG_3_FL_FIF_SHIFT  (0x00000007u)
#define CSL_HDMI_CEC_DBG_3_FL_FIF_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_DBG_3_FR_RT_CNT_MASK (0x00000070u)
#define CSL_HDMI_CEC_DBG_3_FR_RT_CNT_SHIFT (0x00000004u)
#define CSL_HDMI_CEC_DBG_3_FR_RT_CNT_RESETVAL (0x00000000u)


#define CSL_HDMI_CEC_DBG_3_INV_ACK_MASK  (0x00000004u)
#define CSL_HDMI_CEC_DBG_3_INV_ACK_SHIFT (0x00000002u)
#define CSL_HDMI_CEC_DBG_3_INV_ACK_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_DBG_3_ACKN_HEAD_MASK (0x00000002u)
#define CSL_HDMI_CEC_DBG_3_ACKN_HEAD_SHIFT (0x00000001u)
#define CSL_HDMI_CEC_DBG_3_ACKN_HEAD_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_DBG_3_CEC_SN_MASK   (0x00000001u)
#define CSL_HDMI_CEC_DBG_3_CEC_SN_SHIFT  (0x00000000u)
#define CSL_HDMI_CEC_DBG_3_CEC_SN_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_DBG_3_RESETVAL      (0x00000000u)

/* CEC_TX_INIT */



#define CSL_HDMI_CEC_TX_INIT_CEC_INIT_ID_MASK (0x0000000Fu)
#define CSL_HDMI_CEC_TX_INIT_CEC_INIT_ID_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_TX_INIT_CEC_INIT_ID_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_TX_INIT_RESETVAL    (0x00000000u)

/* CEC_TX_DEST */


#define CSL_HDMI_CEC_TX_DEST_CEC_SD_POLL_MASK (0x00000080u)
#define CSL_HDMI_CEC_TX_DEST_CEC_SD_POLL_SHIFT (0x00000007u)
#define CSL_HDMI_CEC_TX_DEST_CEC_SD_POLL_RESETVAL (0x00000000u)


#define CSL_HDMI_CEC_TX_DEST_CEC_DEST_ID_MASK (0x0000000Fu)
#define CSL_HDMI_CEC_TX_DEST_CEC_DEST_ID_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_TX_DEST_CEC_DEST_ID_RESETVAL (0x00000001u)

#define CSL_HDMI_CEC_TX_DEST_RESETVAL    (0x00000001u)

/* CEC_SET_UP */



#define CSL_HDMI_CEC_SETUP_CEC_PTRH_MASK (0x00000010u)
#define CSL_HDMI_CEC_SETUP_CEC_PTRH_SHIFT (0x00000004u)
#define CSL_HDMI_CEC_SETUP_CEC_PTRH_RESETVAL (0x00000001u)


#define CSL_HDMI_CEC_SETUP_CEC_FORCE_NON_CALIB_MASK (0x00000004u)
#define CSL_HDMI_CEC_SETUP_CEC_FORCE_NON_CALIB_SHIFT (0x00000002u)
#define CSL_HDMI_CEC_SETUP_CEC_FORCE_NON_CALIB_RESETVAL (0x00000001u)

#define CSL_HDMI_CEC_SETUP_CEC_CAL_EN_MASK (0x00000002u)
#define CSL_HDMI_CEC_SETUP_CEC_CAL_EN_SHIFT (0x00000001u)
#define CSL_HDMI_CEC_SETUP_CEC_CAL_EN_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_SETUP_CEC_CAL_SW_MASK (0x00000001u)
#define CSL_HDMI_CEC_SETUP_CEC_CAL_SW_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_SETUP_CEC_CAL_SW_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_SETUP_RESETVAL      (0x00000014u)

/* CEC_TX_COMMAND */


#define CSL_HDMI_CEC_TX_COMMAND_CEC_TX_COM_MASK (0x000000FFu)
#define CSL_HDMI_CEC_TX_COMMAND_CEC_TX_COM_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_TX_COMMAND_CEC_TX_COM_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_TX_COMMAND_RESETVAL (0x00000000u)

/* CEC_TX_OPERAND */


#define CSL_HDMI_CEC_TX_OPERAND_CEC_TX_OP_MASK (0x000000FFu)
#define CSL_HDMI_CEC_TX_OPERAND_CEC_TX_OP_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_TX_OPERAND_CEC_TX_OP_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_TX_OPERAND_RESETVAL (0x00000000u)

/* CEC_TRANSMIT_DATA */



#define CSL_HDMI_CEC_TRANSMIT_DATA_TX_BFR_ACC_MASK (0x00000040u)
#define CSL_HDMI_CEC_TRANSMIT_DATA_TX_BFR_ACC_SHIFT (0x00000006u)
#define CSL_HDMI_CEC_TRANSMIT_DATA_TX_BFR_ACC_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_TRANSMIT_DATA_TX_AUT_CALC_MASK (0x00000020u)
#define CSL_HDMI_CEC_TRANSMIT_DATA_TX_AUT_CALC_SHIFT (0x00000005u)
#define CSL_HDMI_CEC_TRANSMIT_DATA_TX_AUT_CALC_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_TRANSMIT_DATA_TRA_CEC_CMD_MASK (0x00000010u)
#define CSL_HDMI_CEC_TRANSMIT_DATA_TRA_CEC_CMD_SHIFT (0x00000004u)
#define CSL_HDMI_CEC_TRANSMIT_DATA_TRA_CEC_CMD_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_TRANSMIT_DATA_TX_CNT_MASK (0x0000000Fu)
#define CSL_HDMI_CEC_TRANSMIT_DATA_TX_CNT_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_TRANSMIT_DATA_TX_CNT_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_TRANSMIT_DATA_RESETVAL (0x00000000u)

/* CEC_CA_7_0 */


#define CSL_HDMI_CEC_CA_7_0_CEC_CAP_ID_MASK (0x000000FFu)
#define CSL_HDMI_CEC_CA_7_0_CEC_CAP_ID_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_CA_7_0_CEC_CAP_ID_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_CA_7_0_RESETVAL     (0x00000000u)

/* CEC_CA_15_8 */


#define CSL_HDMI_CEC_CA_15_8_CEC_CAP_ID_MASK (0x000000FFu)
#define CSL_HDMI_CEC_CA_15_8_CEC_CAP_ID_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_CA_15_8_CEC_CAP_ID_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_CA_15_8_RESETVAL    (0x00000000u)

/* CEC_INT_ENABLE_0 */



#define CSL_HDMI_CEC_INT_ENABLE_0_CEC_INTR1_MASK5_MASK (0x00000020u)
#define CSL_HDMI_CEC_INT_ENABLE_0_CEC_INTR1_MASK5_SHIFT (0x00000005u)
#define CSL_HDMI_CEC_INT_ENABLE_0_CEC_INTR1_MASK5_RESETVAL (0x00000000u)


#define CSL_HDMI_CEC_INT_ENABLE_0_CEC_INTR1_MASK2_MASK (0x00000004u)
#define CSL_HDMI_CEC_INT_ENABLE_0_CEC_INTR1_MASK2_SHIFT (0x00000002u)
#define CSL_HDMI_CEC_INT_ENABLE_0_CEC_INTR1_MASK2_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_INT_ENABLE_0_CEC_INTR1_MASK1_MASK (0x00000002u)
#define CSL_HDMI_CEC_INT_ENABLE_0_CEC_INTR1_MASK1_SHIFT (0x00000001u)
#define CSL_HDMI_CEC_INT_ENABLE_0_CEC_INTR1_MASK1_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_INT_ENABLE_0_CEC_INTR1_MASK0_MASK (0x00000001u)
#define CSL_HDMI_CEC_INT_ENABLE_0_CEC_INTR1_MASK0_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_INT_ENABLE_0_CEC_INTR1_MASK0_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_INT_ENABLE_0_RESETVAL (0x00000000u)

/* CEC_INT_ENABLE_1 */



#define CSL_HDMI_CEC_INT_ENABLE_1_CEC_INTR2_MASK3_MASK (0x00000008u)
#define CSL_HDMI_CEC_INT_ENABLE_1_CEC_INTR2_MASK3_SHIFT (0x00000003u)
#define CSL_HDMI_CEC_INT_ENABLE_1_CEC_INTR2_MASK3_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_INT_ENABLE_1_CEC_INTR2_MASK2_MASK (0x00000004u)
#define CSL_HDMI_CEC_INT_ENABLE_1_CEC_INTR2_MASK2_SHIFT (0x00000002u)
#define CSL_HDMI_CEC_INT_ENABLE_1_CEC_INTR2_MASK2_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_INT_ENABLE_1_CEC_INTR2_MASK1_MASK (0x00000002u)
#define CSL_HDMI_CEC_INT_ENABLE_1_CEC_INTR2_MASK1_SHIFT (0x00000001u)
#define CSL_HDMI_CEC_INT_ENABLE_1_CEC_INTR2_MASK1_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_INT_ENABLE_1_CEC_INTR2_MASK0_MASK (0x00000001u)
#define CSL_HDMI_CEC_INT_ENABLE_1_CEC_INTR2_MASK0_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_INT_ENABLE_1_CEC_INTR2_MASK0_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_INT_ENABLE_1_RESETVAL (0x00000000u)

/* CEC_INT_STATUS_0 */


#define CSL_HDMI_CEC_INT_STATUS_0_CEC_LSTAT_MASK (0x00000080u)
#define CSL_HDMI_CEC_INT_STATUS_0_CEC_LSTAT_SHIFT (0x00000007u)
#define CSL_HDMI_CEC_INT_STATUS_0_CEC_LSTAT_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_INT_STATUS_0_TFIF_BFULL_MASK (0x00000040u)
#define CSL_HDMI_CEC_INT_STATUS_0_TFIF_BFULL_SHIFT (0x00000006u)
#define CSL_HDMI_CEC_INT_STATUS_0_TFIF_BFULL_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_INT_STATUS_0_CEC_INTR1_STAT4_MASK (0x00000020u)
#define CSL_HDMI_CEC_INT_STATUS_0_CEC_INTR1_STAT4_SHIFT (0x00000005u)
#define CSL_HDMI_CEC_INT_STATUS_0_CEC_INTR1_STAT4_RESETVAL (0x00000000u)


#define CSL_HDMI_CEC_INT_STATUS_0_CEC_INTR1_STAT2_MASK (0x00000004u)
#define CSL_HDMI_CEC_INT_STATUS_0_CEC_INTR1_STAT2_SHIFT (0x00000002u)
#define CSL_HDMI_CEC_INT_STATUS_0_CEC_INTR1_STAT2_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_INT_STATUS_0_CEC_INTR1_STAT1_MASK (0x00000002u)
#define CSL_HDMI_CEC_INT_STATUS_0_CEC_INTR1_STAT1_SHIFT (0x00000001u)
#define CSL_HDMI_CEC_INT_STATUS_0_CEC_INTR1_STAT1_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_INT_STATUS_0_CEC_INTR1_STAT0_MASK (0x00000001u)
#define CSL_HDMI_CEC_INT_STATUS_0_CEC_INTR1_STAT0_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_INT_STATUS_0_CEC_INTR1_STAT0_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_INT_STATUS_0_RESETVAL (0x00000000u)

/* CEC_INT_STATUS_1 */



#define CSL_HDMI_CEC_INT_STATUS_1_CEC_INTR2_STAT3_MASK (0x00000008u)
#define CSL_HDMI_CEC_INT_STATUS_1_CEC_INTR2_STAT3_SHIFT (0x00000003u)
#define CSL_HDMI_CEC_INT_STATUS_1_CEC_INTR2_STAT3_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_INT_STATUS_1_CEC_INTR2_STAT2_MASK (0x00000004u)
#define CSL_HDMI_CEC_INT_STATUS_1_CEC_INTR2_STAT2_SHIFT (0x00000002u)
#define CSL_HDMI_CEC_INT_STATUS_1_CEC_INTR2_STAT2_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_INT_STATUS_1_CEC_INTR2_STAT1_MASK (0x00000002u)
#define CSL_HDMI_CEC_INT_STATUS_1_CEC_INTR2_STAT1_SHIFT (0x00000001u)
#define CSL_HDMI_CEC_INT_STATUS_1_CEC_INTR2_STAT1_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_INT_STATUS_1_CEC_INTR2_STAT0_MASK (0x00000001u)
#define CSL_HDMI_CEC_INT_STATUS_1_CEC_INTR2_STAT0_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_INT_STATUS_1_CEC_INTR2_STAT0_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_INT_STATUS_1_RESETVAL (0x00000000u)

/* CEC_RX_CONTROL */



#define CSL_HDMI_CEC_RX_CONTROL_CLR_RX_FIF_ALL_MASK (0x00000002u)
#define CSL_HDMI_CEC_RX_CONTROL_CLR_RX_FIF_ALL_SHIFT (0x00000001u)
#define CSL_HDMI_CEC_RX_CONTROL_CLR_RX_FIF_ALL_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_RX_CONTROL_CLR_RX_FIF_CUR_MASK (0x00000001u)
#define CSL_HDMI_CEC_RX_CONTROL_CLR_RX_FIF_CUR_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_RX_CONTROL_CLR_RX_FIF_CUR_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_RX_CONTROL_RESETVAL (0x00000000u)

/* CEC_RX_COUNT */


#define CSL_HDMI_CEC_RX_COUNT_RX_ERROR_MASK (0x00000080u)
#define CSL_HDMI_CEC_RX_COUNT_RX_ERROR_SHIFT (0x00000007u)
#define CSL_HDMI_CEC_RX_COUNT_RX_ERROR_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_RX_COUNT_CEC_RX_CMD_CNT_MASK (0x00000070u)
#define CSL_HDMI_CEC_RX_COUNT_CEC_RX_CMD_CNT_SHIFT (0x00000004u)
#define CSL_HDMI_CEC_RX_COUNT_CEC_RX_CMD_CNT_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_RX_COUNT_CEC_RX_BYTE_CNT_MASK (0x0000000Fu)
#define CSL_HDMI_CEC_RX_COUNT_CEC_RX_BYTE_CNT_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_RX_COUNT_CEC_RX_BYTE_CNT_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_RX_COUNT_RESETVAL   (0x00000000u)

/* CEC_RX_CMD_HEADER */


#define CSL_HDMI_CEC_RX_CMD_HEADER_CEC_RX_INIT_MASK (0x000000F0u)
#define CSL_HDMI_CEC_RX_CMD_HEADER_CEC_RX_INIT_SHIFT (0x00000004u)
#define CSL_HDMI_CEC_RX_CMD_HEADER_CEC_RX_INIT_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_RX_CMD_HEADER_CEC_RX_DEST_MASK (0x0000000Fu)
#define CSL_HDMI_CEC_RX_CMD_HEADER_CEC_RX_DEST_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_RX_CMD_HEADER_CEC_RX_DEST_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_RX_CMD_HEADER_RESETVAL (0x00000000u)

/* CEC_RX_COMMAND */


#define CSL_HDMI_CEC_RX_COMMAND_CEC_RX_COM_MASK (0x000000FFu)
#define CSL_HDMI_CEC_RX_COMMAND_CEC_RX_COM_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_RX_COMMAND_CEC_RX_COM_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_RX_COMMAND_RESETVAL (0x00000000u)

/* CEC_RX_OPERAND */


#define CSL_HDMI_CEC_RX_OPERAND_CEC_RX_OP_MASK (0x000000FFu)
#define CSL_HDMI_CEC_RX_OPERAND_CEC_RX_OP_SHIFT (0x00000000u)
#define CSL_HDMI_CEC_RX_OPERAND_CEC_RX_OP_RESETVAL (0x00000000u)

#define CSL_HDMI_CEC_RX_OPERAND_RESETVAL (0x00000000u)


/**************************************************************************\
 * Wrapper Field Definition Macros
\**************************************************************************/

/* IRQSTATUS*/
#define CSL_HDMI_WP_IRQSTATUS_PLL_LOCK_PEND_MASK            (0x20000000u)
#define CSL_HDMI_WP_IRQSTATUS_PLL_RECAL_PEND_MASK           (0x80000000u)

#define CSL_HDMI_WP_IRQSTATUS_PHY_LINK_DISCONNECT_PEND_MASK (0x04000000u)
#define CSL_HDMI_WP_IRQSTATUS_PHY_LINK_CONNECT_PEND_MASK    (0x02000000u)
#define CSL_HDMI_WP_IRQSTATUS_VIDEO_VSYNC_PEND_MASK         (0x00010000u)
#define CSL_HDMI_WP_IRQSTATUS_CORE_INTR_PEND_MASK           (0x00000001u)

/* WRAPPER SYSCONFIG */
#define CSL_HDMI_WP_SYSCONFIG_SOFTRESET_MASK                (0x00000001u)

/* IRQENABLE_SET */
#define CSL_HDMI_WP_IRQENABLE_ENABLE_PLL_RECAL_MASK         (0x80000000u)
#define CSL_HDMI_WP_IRQENABLE_PHY_LINK_DISCONNECT_MASK      (0x04000000u)
#define CSL_HDMI_WP_IRQENABLE_PHY_LINK_CONNECT_MASK         (0x02000000u)
#define CSL_HDMI_WP_IRQENABLE_VIDEO_VSYNC_MASK              (0x00010000u)
#define CSL_HDMI_WP_IRQENABLE_CORE_INTR_MASK                (0x00000001u)



#define CSL_HDMI_WP_VIDEO_TIMING_H_HBP_MASK                 (0xFFF00000u)
#define CSL_HDMI_WP_VIDEO_TIMING_H_HBP_SHIFT                (0x00000014u)

#define CSL_HDMI_WP_VIDEO_TIMING_H_HFP_MASK                 (0x000FFF00u)
#define CSL_HDMI_WP_VIDEO_TIMING_H_HFP_SHIFT                (0x00000008u)

#define CSL_HDMI_WP_VIDEO_TIMING_H_HSW_MASK                 (0x000000FFu)
#define CSL_HDMI_WP_VIDEO_TIMING_H_HSW_SHIFT                (0x00000000u)


#define CSL_HDMI_WP_VIDEO_TIMING_V_VBP_MASK                 (0xFFF00000u)
#define CSL_HDMI_WP_VIDEO_TIMING_V_VBP_SHIFT                (0x00000014u)

#define CSL_HDMI_WP_VIDEO_TIMING_V_VFP_MASK                 (0x000FFF00u)
#define CSL_HDMI_WP_VIDEO_TIMING_V_VFP_SHIFT                (0x00000008u)

#define CSL_HDMI_WP_VIDEO_TIMING_V_VSW_MASK                 (0x000000FFu)
#define CSL_HDMI_WP_VIDEO_TIMING_V_VSW_SHIFT                (0x00000000u)

/* DEBOUNCE */
#define CSL_HDMI_WP_DEBOUNCE_RXDET_MASK                     (0x00003F00u)
#define CSL_HDMI_WP_DEBOUNCE_RXDET_SHIFT                    (0x00000008u)

#define CSL_HDMI_WP_DEBOUNCE_LINE5VSHORT_MASK               (0x0000003Fu)
#define CSL_HDMI_WP_DEBOUNCE_LINE5VSHORT_SHIFT              (0x00000000u)

/* VIDEO_CFG */
#define CSL_HDMI_WP_VIDEO_CFG_ENABLE_MASK                   (0x80000000u)

#define CSL_HDMI_WP_VIDEO_CFG_SLAVE_CLOCK_USED_MASK         (0x00020000u)

#define CSL_HDMI_WP_VIDEO_CFG_TMDS_CLOCK_USED_MASK          (0x00010000u)

#define CSL_HDMI_WP_VIDEO_CFG_PACKING_MODE_MASK             (0x00000700u)
#define CSL_HDMI_WP_VIDEO_CFG_PACKING_MODE_SHIFT            (0x00000008u)

#define CSL_HDMI_WP_VIDEO_CFG_VSYNC_POL_MASK                (0x00000080u)

#define CSL_HDMI_WP_VIDEO_CFG_HSYNC_POL_MASK                (0x00000040u)

#define CSL_HDMI_WP_VIDEO_CFG_CORE_VSYNC_INV_MASK           (0x00000020u)

#define CSL_HDMI_WP_VIDEO_CFG_CORE_HSYNC_INV_MASK           (0x00000010u)

#define CSL_HDMI_WP_VIDEO_CFG_PROGRESSIVE_INTERLACE_MASK    (0x00000008u)

#define CSL_HDMI_WP_VIDEO_CFG_MODE_MASK                     (0x00000003u)
#define CSL_HDMI_WP_VIDEO_CFG_MODE_SHIFT                    (0x00000000u)

/* VIDEO_SIZE */
#define CSL_HDMI_WP_VIDEO_SIZE_DIVRATIO_MASK                (0xF8000000u)
#define CSL_HDMI_WP_VIDEO_SIZE_DIVRATIO_SHIFT               (0x0000001Bu)

#define CSL_HDMI_WP_VIDEO_SIZE_LPP_MASK                     (0x07FF0000u)
#define CSL_HDMI_WP_VIDEO_SIZE_LPP_SHIFT                   (0x00000010u)

#define CSL_HDMI_WP_VIDEO_SIZE_PPL_MASK                     (0x0000FFFFu)
#define CSL_HDMI_WP_VIDEO_SIZE_PPL_SHIFT                    (0x00000000u)

/* AUDIO_CTRL */
#define CSL_HDMI_WP_AUDIO_CTRL_DISABLE_MASK                 (0x80000000u)


/**************************************************************************\
 * Phy Field Definition Macros
\**************************************************************************/

/* TMDS_CNTL2 */

#define CSL_HDMI_PHY_TMDS_CNTL2_TERM_EN_MASK                (0x00000010u)
#define CSL_HDMI_PHY_TMDS_CNTL2_TERM_EN_SHIFT               (0x00000004u)

#define CSL_HDMI_PHY_TMDS_CNTL2_OE_MASK                     (0x00000020u)
#define CSL_HDMI_PHY_TMDS_CNTL2_OE_SHIFT                    (0x00000005u)


/* TMDS_CNTL3 */
#define CSL_HDMI_PHY_TMDS_CNTL3_PDB_MASK                    (0x00000001u)
#define CSL_HDMI_PHY_TMDS_CNTL3_PDB_SHIFT                   (0x00000000u)

#define CSL_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_MASK            (0x00000006u)
#define CSL_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_SHIFT           (0x00000001u)

#define CSL_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_NO              (0x00000000u) /* 8 bits/channel */
#define CSL_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_10BITCHANNEL    (0x00000001u)
#define CSL_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_12BITCHANNEL    (0x00000002u)


#define CSL_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_MASK            (0x00000018u)
#define CSL_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_SHIFT           (0x00000003u)

#define CSL_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_0_5X            (0x00000000u)
#define CSL_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_1_0X            (0x00000001u)
#define CSL_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_2_0X            (0x00000002u)
#define CSL_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_4_0X            (0x00000003u)

#define CSL_HDMI_PHY_TMDS_CNTL3_BIST_SEL_MASK               (0x00000040u)


/* BIST_CNTL */

#define CSL_HDMI_PHY_BIST_CNTL_BIST_EN_MASK                 (0x00000001u)
#define CSL_HDMI_PHY_BIST_CNTL_BIST_ON_MASK                 (0x00000002u)
#define CSL_HDMI_PHY_BIST_CNTL_ENC_BYP_MASK                 (0x00000004u)

/* TMDS_CNTL9 */

#define CSL_HDMI_PHY_TMDS_CNTL9_TEN_BIT_BYPASS              (0x00000080u)


/*********************PLL CTRL*************************************************/

#define CSL_HDMI_PLLCTRL_PLLGO_GO_MASK                      (0x00000001u)

#define CSL_HDMI_PLLCTRL_CONFIGURATION1_PLL_REGM_MASK       (0x001FFE00u)
#define CSL_HDMI_PLLCTRL_CONFIGURATION1_PLL_REGM_SHIFT      (0x00000009u)

#define CSL_HDMI_PLLCTRL_CONFIGURATION1_PLL_REGN_MASK       (0x000001FEu)
#define CSL_HDMI_PLLCTRL_CONFIGURATION1_PLL_REGN_SHIFT      (0x00000001u)


#define CSL_HDMI_PLLCTRL_CONFIGURATION2_REFSEL_MASK         (0x00600000u)
#define CSL_HDMI_PLLCTRL_CONFIGURATION2_REFSEL_SHIFT        (0x00000015u)

#define CSL_HDMI_PLLCTRL_CONFIGURATION2_REFSEL_PCLK         (0x00000000u)
#define CSL_HDMI_PLLCTRL_CONFIGURATION2_REFSEL_REF1         (0x00000001u)
#define CSL_HDMI_PLLCTRL_CONFIGURATION2_REFSEL_REF2         (0x00000002u)
#define CSL_HDMI_PLLCTRL_CONFIGURATION2_REFSEL_SYSCLK       (0x00000003u)

#define CSL_HDMI_PLLCTRL_CONFIGURATION2_PHY_CLKINEN_MASK    (0x00004000u)

#define CSL_HDMI_PLLCTRL_CONFIGURATION2_PLL_REFEN_MASK      (0x00002000u)

#define CSL_HDMI_PLLCTRL_CONFIGURATION2_PLL_HIGHFREQ_MASK   (0x00001000u)

#define CSL_HDMI_PLLCTRL_CONFIGURATION2_PLL_CLKSEL_MASK     (0x00000800u)

#define CSL_HDMI_PLLCTRL_CONFIGURATION2_PLL_DRIFTGUARDEN_MASK   (0x00000100u)

#define CSL_HDMI_PLLCTRL_CONFIGURATION2_PLL_SELFFREQDCO_MASK    (0x0000000Eu)
#define CSL_HDMI_PLLCTRL_CONFIGURATION2_PLL_SELFFREQDCO_SHIFT   (0x00000001u)

#define CSL_HDMI_PLLCTRL_CONFIGURATION3_PLL_SD_MASK         (0x0003FC00u)
#define CSL_HDMI_PLLCTRL_CONFIGURATION3_PLL_SD_SHIFT        (0x0000000Au)

#define CSL_HDMI_PLLCTRL_CONFIGURATION4_PLL_REGM2_MASK      (0x01FC0000u)
#define CSL_HDMI_PLLCTRL_CONFIGURATION4_PLL_REGM2_SHIFT     (0x00000012u)

#define CSL_HDMI_PLLCTRL_CONFIGURATION4_PLL_REGM_F_MASK     (0x0003FFFFu)
#define CSL_HDMI_PLLCTRL_CONFIGURATION4_PLL_REGM_F_SHIFT    (0x00000000u)




#endif /* _CSLR_VPS_HDMI_H_ */
