# RTC Guide
RTC HAT(ds1307) or High Accuracy RTC HAT(ds3231)

<table>
<tr>
<td>
<a><img src=https://www.seeedstudio.com/media/catalog/product/cache/ef3164306500b1080e8560b2e8b5cc0f/h/t/httpsstatics3.seeedstudio.comseeedfile2018-11bazaar988493_perspective.jpg></a>
</td>
<td>
<a><img src=https://www.seeedstudio.com/media/catalog/product/cache/ef3164306500b1080e8560b2e8b5cc0f/h/t/httpsstatics3.seeedstudio.comseeedfile2018-11bazaar988498_perspective.jpg></a>
</td>
</tr>
</table>


## Installation
**Please follow the steps strictly when installing,otherwise the installation may failed or even damage the module.**

1. Driver Installation，  **Only applied for Raspbian Jessie/Stretch.**
```
#open tools directory
cd tools

#for ds1307
sudo ./install.sh -u rtc_ds1307

#for ds3231
sudo ./install.sh -u rtc_ds3231

#sync all of settings
sync
```
Power off Raspberry Pi

Insert the HAT to Raspberry Pi

Power up Raspberry Pi

2. Test

Read hardware clock and print result

    sudo hwclock -r

Set the system time from the hardware clock

    sudo hwclock -s

Set the hardware clock from the current system time

    sudo hwclock -w

More usage

    hwclock --help

3. List install status
```sh
./install.sh -l
```
4. Uninstallation
```bash
sudo ./install.sh -u
```
