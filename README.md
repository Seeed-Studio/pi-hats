## RTC HAT(ds1307)
## High Accuracy RTC HAT(ds3231)
## 4-Channel 16-Bit ADC HAT(ads1115)
### for Raspberry Pi
##
## Installation
1. Driver Installation

    Only applied for Raspbian Jessie/Stretch

    *RTC HAT(ds1307)*
    ```bash
        sudo ./install.sh -u rtc_ds1307
    ```
    *High Accuracy RTC HAT(ds3231)*
    ```bash
        sudo ./install.sh -u rtc_ds3231
    ```
    *4-Channel 16-Bit ADC HAT(ads1115)*
    ```bash
        sudo ./install.sh -u adc_ads1115
    ```
2. Power off Raspberry Pi
3. Insert the HAT to Raspberry Pi
4. Power up Raspberry Pi

#### List install status
```bash
    ./install.sh -l
```

#### Uninstallation
```bash
    sudo ./install.sh -u
```

##
## RTC Guide
### RTC HAT(ds1307) or High Accuracy RTC HAT(ds3231)
Read hardware clock and print result
```bash
    sudo hwclock -r
```
Set the system time from the hardware clock
```bash
    sudo hwclock -s
```
Set the hardware clock from the current system time
```bash
    sudo hwclock -w
```
More usage
```bash
    hwclock --help
```
##
## ADC Guide
### 4-Channel 16-Bit ADC HAT(ads1115)
Read channel 0 adc result, for channel X, replace in0_input with inX_input.
```bash
    cat /sys/devices/platform/soc/*04000.i2c/i2c-1/1-0048/in0_input
```

