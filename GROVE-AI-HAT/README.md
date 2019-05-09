# CAN-HAT


The drivers of [Grove AI HAT for Edge Computing devices](https://www.seeedstudio.com/Grove-AI-HAT-for-Edge-Computing-p-4026.html) for Raspberry Pi.

### Install Grove AI HAT for Edge Computing drivers
Get the Grove AI HAT for Edge Computing source code. and install all linux kernel drivers
```bash
git clone https://github.com/seeed-Studio/pi-hats
cd pi-hats/GROVE-AI-HAT
sudo ./install.sh 
sudo reboot
```

## Raspberry pi Grove AI HAT for Edge Computing

[![](https://www.seeedstudio.site/media/catalog/product/cache/ef3164306500b1080e8560b2e8b5cc0f/1/0/102991187-preview_1.png)](https://www.seeedstudio.com/Grove-AI-HAT-for-Edge-Computing-p-4026.html)


## Get microphone data from Grove AI HAT for Edge Computing
1. This code play the data recorded by the digital microphone onto another I2S. You can user [ArduinoCore-K210](https://github.com/Seeed-Studio/ArduinoCore-k210) IDE to compile and run it.
```
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "i2s.h"
#include "sysctl.h"
#include "fpioa.h"
#include "uarths.h"

#define DMA_IRQ 1

#define FRAME_LEN   128
uint32_t rx_buf[1024];

void io_mux_init(){

    fpioa_set_function(20, FUNC_I2S0_IN_D0);
    fpioa_set_function(19, FUNC_I2S0_WS);
    fpioa_set_function(18, FUNC_I2S0_SCLK);

    fpioa_set_function(32, FUNC_I2S2_OUT_D1);
    fpioa_set_function(35, FUNC_I2S2_SCLK);
    fpioa_set_function(33, FUNC_I2S2_WS);
}

typedef struct _index_t
{
    uint32_t r_index;
    uint32_t t_index;
} index_t;

index_t g_index_t;

int i2s_on_irq(void *ctx)
{
    index_t *index = (index_t *)ctx;
    index->r_index += (FRAME_LEN*2);
    if(index->r_index >= 1023)
    {
        index->r_index = 0;
    }
    i2s_data_t data = (i2s_data_t)
    {
        .rx_channel = DMAC_CHANNEL0,
        .rx_buf = &rx_buf[index->r_index],
        .rx_len = FRAME_LEN * 2,
        .transfer_mode = I2S_RECEIVE,
    };
    plic_interrupt_t irq = (plic_interrupt_t)
    {
        .callback = i2s_on_irq,
        .ctx = &g_index_t,
        .priority = 1,
    };
    i2s_handle_data_dma(I2S_DEVICE_0, data, &irq);

    if (index->r_index - index->t_index >= FRAME_LEN || index->t_index - index->r_index >= (1023 - FRAME_LEN * 2))
    {
        data = (i2s_data_t)
        {
            .tx_channel = DMAC_CHANNEL1,
            .tx_buf = &rx_buf[index->t_index],
            .tx_len = FRAME_LEN * 2,
            .transfer_mode = I2S_SEND,
        };
        i2s_handle_data_dma(I2S_DEVICE_2, data, NULL);
        index->t_index += (FRAME_LEN * 2);
        if (index->t_index >= 1023)
            index->t_index = 0;
    }
}

int main(void)
{
    sysctl_pll_set_freq(SYSCTL_PLL0, 320000000UL);
    sysctl_pll_set_freq(SYSCTL_PLL1, 160000000UL);
    sysctl_pll_set_freq(SYSCTL_PLL2, 45158400UL);
    uarths_init();
    io_mux_init();
    printf("I2S0 receive , I2S2 play...\n");

    i2s_init(I2S_DEVICE_0, I2S_RECEIVER, 0x3);
    i2s_init(I2S_DEVICE_2, I2S_TRANSMITTER, 0xC);

    i2s_rx_channel_config(I2S_DEVICE_0, I2S_CHANNEL_0,
        RESOLUTION_16_BIT, SCLK_CYCLES_32,
        TRIGGER_LEVEL_4, STANDARD_MODE);

    i2s_tx_channel_config(I2S_DEVICE_2, I2S_CHANNEL_1,
        RESOLUTION_16_BIT, SCLK_CYCLES_32,
        TRIGGER_LEVEL_4,
        STANDARD_MODE
        );

#if !DMA_IRQ
    i2s_data_t data = (i2s_data_t)
    {
        .rx_channel = DMAC_CHANNEL0,
        .rx_buf = rx_buf,
        .rx_len = FRAME_LEN * 2,
        .transfer_mode = I2S_RECEIVE,
    };
    i2s_handle_data_dma(I2S_DEVICE_0, data, NULL);
    while (1)
    {
        g_index += (FRAME_LEN*2);
        if(g_index >= 1023)
        {
            g_index = 0;
        }
        data = (i2s_data_t)
        {
            .rx_channel = DMAC_CHANNEL0,
            .rx_buf = &rx_buf[g_index],
            .rx_len = FRAME_LEN * 2,
            .transfer_mode = I2S_RECEIVE,
        };

        i2s_handle_data_dma(I2S_DEVICE_0, data, NULL);
        if (g_index - g_tx_len >= FRAME_LEN || g_tx_len - g_index >= (1023 - FRAME_LEN * 2))
        {
            data = (i2s_data_t)
            {
                .tx_channel = DMAC_CHANNEL1,
                .tx_buf = &rx_buf[g_tx_len],
                .tx_len = FRAME_LEN * 2,
                .transfer_mode = I2S_SEND,
            };
            i2s_handle_data_dma(I2S_DEVICE_2, data, NULL);
            g_tx_len += (FRAME_LEN * 2);
            if (g_tx_len >= 1023)
                g_tx_len = 0;
        }
    }
#else
    i2s_data_t data = (i2s_data_t)
    {
        .rx_channel = DMAC_CHANNEL0,
        .rx_buf = rx_buf,
        .rx_len = FRAME_LEN * 2,
        .transfer_mode = I2S_RECEIVE,
    };
    plic_interrupt_t irq = (plic_interrupt_t)
    {
        .callback = i2s_on_irq,
        .ctx = &g_index_t,
        .priority = 1,
    };
    i2s_handle_data_dma(I2S_DEVICE_0, data, &irq);
    while(1);
#endif

    return 0;
}

```

2. check the hardware and drivers
```
pi@raspberrypi:~/pi-hats/GROVE-AI-HAT $ arecord -l
**** List of CAPTURE Hardware Devices ****
card 1: seeed1voicecard [seeed-1-voicecard], device 0: bcm2835-i2s-grove_ai_mics-hifi grove_ai_mics-hifi-0 []
  Subdevices: 1/1
  Subdevice #0: subdevice #0
pi@raspberrypi:~/pi-hats/GROVE-AI-HAT $
```
3. Record the audio data
```
pi@raspberrypi:~/pi-hats/GROVE-AI-HAT $ arecord -Dhw:1,0 -f S32_LE -r 44100 -c 2 -v  a.wav
Recording WAVE 'a.wav' : Signed 32 bit Little Endian, Rate 44100 Hz, Stereo
Hardware PCM card 1 'seeed-1-voicecard' device 0 subdevice 0
Its setup is:
  stream       : CAPTURE
  access       : RW_INTERLEAVED
  format       : S32_LE
  subformat    : STD
  channels     : 2
  rate         : 44100
  exact rate   : 44100 (44100/1)
  msbits       : 32
  buffer_size  : 22052
  period_size  : 5513
  period_time  : 125011
  tstamp_mode  : NONE
  tstamp_type  : MONOTONIC
  period_step  : 1
  avail_min    : 5513
  period_event : 0
  start_threshold  : 1
  stop_threshold   : 22052
  silence_threshold: 0
  silence_size : 0
  boundary     : 1445199872
  appl_ptr     : 0
  hw_ptr       : 0

```
### uninstall GROVE-AI-HAT

```
pi@raspberrypi:~/pi-hats/GROVE-AI-HAT $ sudo ./uninstall.sh 
...
------------------------------------------------------
Please reboot your raspberry pi to apply all settings
Thank you!
------------------------------------------------------
```


Enjoy !
