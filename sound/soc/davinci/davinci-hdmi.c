/*
 * davinci-hdmi.c  --  Davinci ALSA SoC DAI driver for HDMI audio
 * Author:	Deepu Raj <deepu.raj@ti.com>
 * Author:	Vaibhav Bedia <vaibhav.bedia@ti.com>
 *
 * Copyright (C) 2009 Texas Instruments
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/clk.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/initval.h>
#include <sound/soc.h>

#include <plat/cpu.h>
#include <plat/dma.h>
#include "davinci-pcm.h"
#include "davinci-mcasp.h"
#include <plat/hdmi_lib.h>

#define DAVINCI_HDMI_RATES	(SNDRV_PCM_RATE_48000	\
				| SNDRV_PCM_RATE_32000	\
				| SNDRV_PCM_RATE_44100	\
				| SNDRV_PCM_RATE_96000	\
				| SNDRV_PCM_RATE_192000)

/* Currently, we support only 16b & 24b samples at HDMI */
#define DAVINCI_HDMI_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S24_LE)

/* Audio N / CTS values based on TMDS clks */
const struct audio_timings audio_timings[] = {
	/* TMDS 148.4 KHz */
	{148500, 32000, 4096, 148500, IF_FS_32000},
	{148500, 44100, 6272, 165000, IF_FS_44100},
	{148500, 48000, 6144, 148500, IF_FS_48000},
	{148500, 96000, 12288, 148500, IF_FS_96000},
	{148500, 192000, 24576, 148500, IF_FS_192000},

	/* TMDS 74.25 KHz */
	{74250, 32000, 4096, 74250, IF_FS_32000},
	{74250, 44100, 6272, 82500, IF_FS_44100},
	{74250, 48000, 6144, 74250, IF_FS_48000},
	{74250, 96000, 12288, 74250, IF_FS_96000},
	{74250, 192000, 24576, 74250, IF_FS_192000} ,

	/* TMDS 54 KHz */
	{54000, 32000, 4096, 54000, IF_FS_32000},
	{54000, 44100, 6272, 60000, IF_FS_44100},
	{54000, 48000, 6144, 54000, IF_FS_48000},
	{54000, 96000, 12288, 54000, IF_FS_96000},
	{54000, 192000, 24576, 54000, IF_FS_192000},
};

/*
 * select the ACR packet genetation method
 * a bug in the TI8168 PG < 2.0 support only SW
 * default is HW except the above case
 * define the HDMI_FORCE_SW_ACR for SW testing
 */
static int hdmi_acr_mode(void)
{
#ifdef HDMI_FORCE_SW_ACR
	return CTS_MODE_SW;
#else
	/* TI814x HW ACR supported is not added */
	if ((cpu_is_ti816x() && TI8168_REV_ES2_0 > omap_rev())
		|| cpu_is_ti814x())
		return CTS_MODE_SW;
	else
		return CTS_MODE_HW;
#endif
}

static int davinci_hdmi_dai_startup(struct snd_pcm_substream *substream,
				  struct snd_soc_dai *dai)
{
	int err = 0;
	struct davinci_audio_dev *dev = snd_soc_dai_get_drvdata(dai);

	snd_soc_dai_set_dma_data(dai, substream, dev->dma_params);

	/* Enable HDMI wrapped after  config */
	/* err = hdmi_w1_wrapper_enable(HDMI_WP); */

	return err;
}

static void davinci_hdmi_dai_shutdown(struct snd_pcm_substream *substream,
				    struct snd_soc_dai *dai)
{
	hdmi_w1_wrapper_disable(HDMI_WP);
	return;
}

static int davinci_hdmi_dai_trigger(struct snd_pcm_substream *substream, int cmd,
				  struct snd_soc_dai *dai)
{
	int err = 0;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		err = hdmi_w1_start_audio_transfer(HDMI_WP);
		break;

	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		err = hdmi_w1_stop_audio_transfer(HDMI_WP);
		break;
	default:
		err = -EINVAL;
	}

	return err;
}

static const struct audio_timings *get_audio_timings(u32 tmds, u32 rate)
{
	int i = 0;

	for (i = 0; i < ARRAY_SIZE(audio_timings); i++) {
		if (tmds == audio_timings[i].tmds &&
			rate == audio_timings[i].audio_fs)
			return &audio_timings[i];
	}

	return NULL;
}

static int davinci_hdmi_dai_hw_params(struct snd_pcm_substream *substream,
				    struct snd_pcm_hw_params *params,
				    struct snd_soc_dai *dai)
{
	int err = 0;
	u32 av_name = HDMI_CORE_AV;
	int ret = 0;
	int tdms = 0, rate = 0;
	struct hdmi_core_audio_config audio_cfg;
	struct hdmi_audio_format audio_fmt;
	struct hdmi_audio_dma audio_dma;
	const struct audio_timings *timing;
	struct davinci_audio_dev *dev = snd_soc_dai_get_drvdata(dai);
	long mclk_rate = 0;

	/* TODO Modify the N/CTS value selection based on the Video clkc */
	tdms = hdmi_get_video_timing();
	rate = params_rate(params);

	DBG(" TMDS: %d RATE: %d\n", tdms, rate);
	timing = get_audio_timings(tdms, rate);

	if (NULL == timing) {
		printk(KERN_ERR "sampling rate is not supported!\n");
		return -EINVAL;
	}
	audio_cfg.n = timing->audio_n;
	audio_cfg.cts = timing->audio_cts;

	DBG("FS: %d N: %d CTS: %d\n", rate, audio_cfg.n, audio_cfg.cts);

	/*
	 * configure MCLK for HW ACR generation
	 * MCLK = 128 *Fs = TMDS * (N/CTS)
	 */
	if (CTS_MODE_HW == hdmi_acr_mode()) {
		/* MCLK = 128 * Fs */
		mclk_rate = rate * 128;

		/* set the new rate */
		ret = clk_set_rate(dev->clk, mclk_rate);
		if (ret < 0) {
			printk(KERN_ERR "Audio MCLK set rate failed %ld\n",
								mclk_rate);
			return -EINVAL;
		}
	}

	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S16_LE:
		audio_fmt.sample_number = HDMI_ONEWORD_TWO_SAMPLES;
		audio_fmt.sample_size = HDMI_SAMPLE_16BITS;
		audio_cfg.if_sample_size = (IF_16BIT_PER_SAMPLE << 1) |
						 HDMI_SAMPLE_16BITS;
		break;
	case SNDRV_PCM_FORMAT_S24_LE:
		audio_fmt.sample_number = HDMI_ONEWORD_ONE_SAMPLE;
		audio_fmt.sample_size = HDMI_SAMPLE_24BITS;
		audio_cfg.if_sample_size = (IF_24BIT_PER_SAMPLE << 1) |
						HDMI_SAMPLE_24BITS;
		break;
	default:
		err = -EINVAL;
	}

	/* HDMI Wrapper config */
	audio_fmt.justify = HDMI_AUDIO_JUSTIFY_LEFT;
	audio_fmt.stereo_channel_enable = HDMI_STEREO_ONECHANNELS;
	audio_fmt.audio_channel_location = HDMI_CEA_CODE_03;
	audio_fmt.iec = HDMI_AUDIO_FORMAT_LPCM;
	audio_fmt.left_before = HDMI_SAMPLE_LEFT_FIRST;

	ret = hdmi_w1_audio_config_format(HDMI_WP, &audio_fmt);

	audio_dma.dma_transfer = 0x20;
	audio_dma.block_size = 0xC0;
	audio_dma.threshold_value = 0x20;
	audio_dma.dma_or_irq = HDMI_THRESHOLD_DMA;
	audio_dma.block_start_end = HDMI_BLOCK_STARTEND_ON;

	ret = hdmi_w1_audio_config_dma(HDMI_WP, &audio_dma);

	/* HDMI Core config */
	audio_cfg.if_fs = timing->audio_if_fs;
	/*Currently we are supporting only 2CH*/
	audio_cfg.layout = LAYOUT_2CH;
	audio_cfg.if_channel_number = HDMI_STEREO_TWOCHANNELS;
	audio_cfg.if_audio_channel_location = HDMI_CEA_CODE_00;

	/* TODO: Is this configuration correct? */
	audio_cfg.aud_par_busclk = 0;

	if (CTS_MODE_HW == hdmi_acr_mode()) {
		audio_cfg.cts_mode = CTS_MODE_HW;
		DBG("CTS mode is HW\n");
	} else {
		audio_cfg.cts_mode = CTS_MODE_SW;
		DBG("CTS mode is SW\n");
	}

	hdmi_core_audio_config(av_name, &audio_cfg);
	hdmi_core_audio_mode_enable(av_name);

	/* Enable Wrapper : Wrapper must be disable while config */
	err = hdmi_w1_wrapper_enable(HDMI_WP);

	return err;
}

static struct snd_soc_dai_ops davinci_hdmi_dai_ops = {
	.startup	= davinci_hdmi_dai_startup,
	.shutdown	= davinci_hdmi_dai_shutdown,
	.trigger	= davinci_hdmi_dai_trigger,
	.hw_params	= davinci_hdmi_dai_hw_params,
};

static struct snd_soc_dai_driver davinci_hdmi_dai = {
	.playback = {
		.channels_min = 2,
		/* currently we support only stereo HDMI */
		.channels_max = 2,
		.rates = DAVINCI_HDMI_RATES,
		.formats = DAVINCI_HDMI_FORMATS,
	},
	.ops = &davinci_hdmi_dai_ops,
};

static __devinit int davinci_hdmi_probe(struct platform_device *pdev)
{
	struct davinci_pcm_dma_params *dma_data;
	struct snd_hdmi_platform_data *pdata;
	struct davinci_audio_dev *dev;
	int ret = 0;

	dev = kzalloc(sizeof(struct davinci_audio_dev), GFP_KERNEL);
	if (!dev)
		return  -ENOMEM;

	pdata = pdev->dev.platform_data;

	/*
	 * Configure MCLK for HW ACR packet generation
	 */
	if (CTS_MODE_HW == hdmi_acr_mode()) {
		dev->clk = clk_get(&pdev->dev, NULL);
		if (IS_ERR(dev->clk)) {
			printk(KERN_ERR "%s Clock get Error\n", pdev->name);
			ret = -ENODEV;
		}
		/* Enable the clock */
		clk_enable(dev->clk);
		dev->clk_active = 1;
	}
	
	if (NULL != pdata) {
		dma_data = &dev->dma_params[SNDRV_PCM_STREAM_PLAYBACK];

		dma_data->dma_addr = (pdata->dma_addr);
		dma_data->channel = (pdata->channel);
		dma_data->data_type = (pdata->data_type);
		dma_data->acnt = (pdata->acnt);
		dma_data->fifo_level = (pdata->fifo_level);
	}

	dev_set_drvdata(&pdev->dev, dev);
	return snd_soc_register_dai(&pdev->dev, &davinci_hdmi_dai);

}

static int __devexit davinci_hdmi_remove(struct platform_device *pdev)
{
	struct davinci_audio_dev *dev = dev_get_drvdata(&pdev->dev);

	snd_soc_unregister_dai(&pdev->dev);

	/*
	 * free MCLK for HW ACR packet generation
	 */
	if (CTS_MODE_HW == hdmi_acr_mode()) {
		clk_disable(dev->clk);
		clk_put(dev->clk);
		dev->clk = NULL;
	}

	kfree(dev);
	return 0;
}

static struct platform_driver hdmi_dai_driver = {
	.driver = {
		.name = "hdmi-dai",
		.owner = THIS_MODULE,
	},
	.probe = davinci_hdmi_probe,
	.remove = __devexit_p(davinci_hdmi_remove),
};

static int __init hdmi_dai_init(void)
{
	return platform_driver_register(&hdmi_dai_driver);
}
module_init(hdmi_dai_init);

static void __exit hdmi_dai_exit(void)
{
	platform_driver_unregister(&hdmi_dai_driver);
}
module_exit(hdmi_dai_exit);

MODULE_AUTHOR("Vaibhav Bedia <vaibhav.bedia@ti.com>");
MODULE_DESCRIPTION("davinci HDMI SoC Interface");
MODULE_LICENSE("GPL");
