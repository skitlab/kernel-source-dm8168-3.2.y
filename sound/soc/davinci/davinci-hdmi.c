/*
 * davinci-hdmi.c  --  Davinci ALSA SoC DAI driver for HDMI audio
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
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/initval.h>
#include <sound/soc.h>

#include <plat/dma.h>
#include "davinci-pcm.h"
#include <plat/hdmi_lib.h>

#define DAVINCI_HDMI_RATES	(SNDRV_PCM_RATE_48000	\
				| SNDRV_PCM_RATE_32000	\
				| SNDRV_PCM_RATE_44100	\
				| SNDRV_PCM_RATE_96000	\
				| SNDRV_PCM_RATE_192000)

/* Currently, we support only 16b samples at HDMI */
#define DAVINCI_HDMI_FORMATS (SNDRV_PCM_FMTBIT_S16_LE)

/* Audio N / CTS values based on TMDS clks */
const struct audio_timings audio_timings[] = {
	/* TMDS 148.4 KHz */
	{148500, 32000, 4096, 148500},
	{148500, 44100, 6272, 165000},
	{148500, 48000, 6144, 148500},
	{148500, 96000, 12288, 148500},
	{148500, 192000, 24576, 148500},

	/* TMDS 74.25 KHz */
	{74250, 32000, 4096, 74250},
	{74250, 44100, 6272, 82500},
	{74250, 48000, 6144, 74250},
	{74250, 96000, 12288, 74250},
	{74250, 192000, 24576, 74250},

	/* TMDS 54 KHz */
	{54000, 32000, 4096, 54000},
	{54000, 44100, 6272, 60000},
	{54000, 48000, 6144, 54000},
	{54000, 96000, 12288, 54000},
	{54000, 192000, 24576, 54000},
};

static struct davinci_pcm_dma_params davinci_hdmi_dai_dma_params;

static int davinci_hdmi_dai_startup(struct snd_pcm_substream *substream,
				  struct snd_soc_dai *dai)
{
	int err = 0;

	struct davinci_audio_dev *dev = snd_soc_dai_get_drvdata(dai);

	davinci_hdmi_dai_dma_params.channel = 53;
	davinci_hdmi_dai_dma_params.dma_addr = TI81xx_HDMI_WP + HDMI_WP_AUDIO_DATA;
	davinci_hdmi_dai_dma_params.data_type = 4;
	davinci_hdmi_dai_dma_params.acnt = 4;
	davinci_hdmi_dai_dma_params.fifo_level = 0x20;

	snd_soc_dai_set_dma_data(dai, substream, &davinci_hdmi_dai_dma_params);
	err = hdmi_w1_wrapper_enable(HDMI_WP);

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

static int davinci_hdmi_dai_hw_params(struct snd_pcm_substream *substream,
				    struct snd_pcm_hw_params *params,
				    struct snd_soc_dai *dai)
{
	int err = 0;
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
	return snd_soc_register_dai(&pdev->dev, &davinci_hdmi_dai);
}

static int __devexit davinci_hdmi_remove(struct platform_device *pdev)
{
	snd_soc_unregister_dai(&pdev->dev);
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
