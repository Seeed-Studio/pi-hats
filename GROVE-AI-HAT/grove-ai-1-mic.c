/**
 * The MIT License (MIT)
 *
 * Author: Baozhu Zuo (baozhu.zuo@gmail.com)
 *
 * Copyright (C) 2019  Seeed Technology Co.,Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/initval.h>
#include <sound/tlv.h>

#define MICS_RATE_MIN 7190 /* Hz, from data sheet */
#define MICS_RATE_MAX 52800  /* Hz, from data sheet */

#define MICS_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S32)

static struct snd_soc_dai_driver grove_ai_mics_dai = {
	.name = "grove_ai_mics-hifi",
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 2,
		.rate_min = MICS_RATE_MIN,
		.rate_max = MICS_RATE_MAX,
		.rates = SNDRV_PCM_RATE_CONTINUOUS,
		.formats = MICS_FORMATS,
	},
};

static const struct snd_soc_codec_driver grove_ai_mics_codec_driver = {
};

static int grove_ai_mics_probe(struct platform_device *pdev)
{
	return snd_soc_register_codec(&pdev->dev, &grove_ai_mics_codec_driver,
			&grove_ai_mics_dai, 1);
}

static int grove_ai_mics_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id grove_ai_mics_ids[] = {
	{ .compatible = "seeed,grove_ai_mics", },
	{ }
};
MODULE_DEVICE_TABLE(of, grove_ai_mics_ids);
#endif

static struct platform_driver grove_ai_mics_driver = {
	.driver = {
		.name = "grove_ai_mics",
		.of_match_table = of_match_ptr(grove_ai_mics_ids),
	},
	.probe = grove_ai_mics_probe,
	.remove = grove_ai_mics_remove,
};

module_platform_driver(grove_ai_mics_driver);

MODULE_DESCRIPTION("ASoC MICS driver");
MODULE_AUTHOR("Baozhu Zuo <baozhu.zuo@gmail.com>");
MODULE_LICENSE("GPL v2");
