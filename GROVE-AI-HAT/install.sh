#!/bin/bash

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root (use sudo)" 1>&2
   exit 1
fi

is_Raspberry=$(cat /proc/device-tree/model | awk  '{print $1}')
if [ "x${is_Raspberry}" != "xRaspberry" ] ; then
  echo "Sorry, this drivers only works on raspberry pi"
  exit 1
fi

ver="0.1"


# we create a dir with this version to ensure that 'dkms remove' won't delete
# the sources during kernel updates
marker="0.0.0"

# update and install required packages
which apt &>/dev/null
if [[ $? -eq 0 ]]; then
  apt update -y
  apt-get -y install raspberrypi-kernel-headers raspberrypi-kernel 
  apt-get -y install dkms git can-utils
fi

# Arch Linux
which pacman &>/dev/null
if [[ $? -eq 0 ]]; then
  pacman -Syu --needed git gcc automake make dkms linux-raspberrypi-headers can-utils
fi

# locate currently installed kernels (may be different to running kernel if
# it's just been updated)
kernels=$(ls /lib/modules | sed "s/^/-k /")
uname_r=$(uname -r)

function install_module {
  src=$1
  mod=$2

  if [[ -d /var/lib/dkms/$mod/$ver/$marker ]]; then
    rmdir /var/lib/dkms/$mod/$ver/$marker
  fi

  if [[ -e /usr/src/$mod-$ver || -e /var/lib/dkms/$mod/$ver ]]; then
    dkms remove --force -m $mod -v $ver --all
    rm -rf /usr/src/$mod-$ver
  fi
  mkdir -p /usr/src/$mod-$ver
  cp -a $src/* /usr/src/$mod-$ver/
  dkms add -m $mod -v $ver
  dkms build $kernels -m $mod -v $ver && dkms install --force $kernels -m $mod -v $ver

  mkdir -p /var/lib/dkms/$mod/$ver/$marker
}

install_module "./" "grove-ai"


# install dtbos
cp seeed-dummy-voicecard.dtbo /boot/overlays



#set kernel moduels
grep -q "^grove-ai-1-mic$" /etc/modules || \
  echo "grove-ai-1-mic" >> /etc/modules

#set dtoverlays
grep -q "^dtoverlay=seeed-dummy-voicecard$" /boot/config.txt || \
  echo "dtoverlay=seeed-dummy-voicecard" >> /boot/config.txt


#install config files
cp 80-can.rules /etc/udev/rules.d/


echo "------------------------------------------------------"
echo "Please reboot your raspberry pi to apply all settings"
echo "Enjoy!"
echo "------------------------------------------------------"
