#!/bin/bash
#
: <<'EOF'
The MIT License (MIT)

Seeed-Studio Raspberry Pi Hats.
  Peter Yang, turmary@126.com
Copyright (C) 2018  Seeed Technology Co.,Ltd. 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
EOF


_DEBUG=0

declare -A overlay_map=(          \
  ["rtc_ds1307"]="i2c-rtc,ds1307" \
  ["rtc_ds3231"]="i2c-rtc,ds3231" \
  ["adc_ads1115"]="ads1115" )
RPI_HATS=${!overlay_map[@]}

BLACKLIST=/etc/modprobe.d/raspi-blacklist.conf
CONFIG=/boot/config.txt
SCRIPT_HWCLOCK_SET=/lib/udev/hwclock-set
if [ $_DEBUG -ne 0 ]; then
  BLACKLIST=./raspi-blacklist.conf
  CONFIG=./config.txt
  SCRIPT_HWCLOCK_SET=hwclock-set
fi

set_config_var() {
  lua - "$1" "$2" "$3" <<EOF > "$3.bak"
local key=assert(arg[1])
local value=assert(arg[2])
local fn=assert(arg[3])
local file=assert(io.open(fn))
local made_change=false
for line in file:lines() do
  if line:match("^#?%s*"..key.."=.*$") then
    line=key.."="..value
    made_change=true
  end
  print(line)
end

if not made_change then
  print(key.."="..value)
end
EOF
mv "$3.bak" "$3"
}

get_i2c() {
  if grep -q -E "^(device_tree_param|dtparam)=([^,]*,)*i2c(_arm)?(=(on|true|yes|1))?(,.*)?$" $CONFIG; then
    echo 0
  else
    echo 1
  fi
}

do_i2c() {
  DEFAULT=--defaultno
  if [ $(get_i2c) -eq 0 ]; then
    DEFAULT=
  fi
  RET=$1
  if [ $RET -eq 0 ]; then
    SETTING=on
    STATUS=enabled
  elif [ $RET -eq 1 ]; then
    SETTING=off
    STATUS=disabled
  else
    return $RET
  fi

  set_config_var dtparam=i2c_arm $SETTING $CONFIG &&
  if ! [ -e $BLACKLIST ]; then
    touch $BLACKLIST
  fi
  sed $BLACKLIST -i -e "s/^\(blacklist[[:space:]]*i2c[-_]bcm2708\)/#\1/"
  sed /etc/modules -i -e "s/^#[[:space:]]*\(i2c[-_]dev\)/\1/"
  if ! grep -q "^i2c[-_]dev" /etc/modules; then
    printf "i2c-dev\n" >> /etc/modules
  fi
  dtparam i2c_arm=$SETTING
  modprobe i2c-dev
}

get_overlay() {
  ov=$1
  if grep -q -E "^dtoverlay=$ov" $CONFIG; then
    echo 0
  else
    echo 1
  fi
}

do_overlay() {
  ov=$1
  RET=$2
  DEFAULT=--defaultno
  CURRENT=0
  if [ $(get_overlay $ov) -eq 0 ]; then
    DEFAULT=
    CURRENT=1
  fi
  if [ $RET -eq $CURRENT ]; then
    ASK_TO_REBOOT=1
  fi
  if [ $RET -eq 0 ]; then
    sed $CONFIG -i -e "s/^#dtoverlay=$ov/dtoverlay=$ov/"
    if ! grep -q -E "^dtoverlay=$ov" $CONFIG; then
      printf "dtoverlay=$ov\n" >> $CONFIG
    fi
    STATUS=enabled
  elif [ $RET -eq 1 ]; then
    sed $CONFIG -i -e "s/^dtoverlay=$ov/#dtoverlay=$ov/"
    STATUS=disabled
  else
    return $RET
  fi
}


### Module specific installation ####
rtc_ds1307_install() {
  apt-get -y remove fake-hwclock
  update-rc.d -f fake-hwclock remove
  systemctl disable fake-hwclock
  sed -i -e "s,^\(if \[ \-e /run/systemd/system \] ; then\),if false; then\n#\1," $SCRIPT_HWCLOCK_SET
}

rtc_ds3231_install() {
  apt-get -y remove fake-hwclock
  update-rc.d -f fake-hwclock remove
  systemctl disable fake-hwclock
  sed -i -e "s,^\(if \[ \-e /run/systemd/system \] ; then\),if false; then\n#\1," $SCRIPT_HWCLOCK_SET
}


### Check arguments ###
do_list=false
do_uninstall=false
module=

help_msg() {
  echo "Usage:"
  echo " $0 [-l] [-u] [-h] <module>"
  echo "  -l list installed module"
  echo "  -u uninstall exist module"
  echo "  <module> can be one of"
  echo "          $RPI_HATS"
  exit 1
}

while getopts "luh" opt; do
  case $opt in
  l) do_list=true;;
  u) do_uninstall=true;;
  h) help_msg;;
  esac
done

eval module="\$$OPTIND"
[ -z "$module" -a "$do_list" == "false" -a "$do_uninstall" == "false" ] && {
  help_msg
}

if [ -n "$module" ]; then
  found=false
  for i in $RPI_HATS; do
    if [ "$module" == "$i" ]; then
      found=true
      break;
    fi
  done
  if [ "$found" == "false" ]; then
    help_msg
  fi
fi


### List installation status ###
if [ "$do_list" == "true" ]; then
  for i in $RPI_HATS; do
    overlay=${overlay_map[$i]}
    printf "%-13s : " $i
    if [ "$(get_overlay $overlay)" -eq 0 ]; then
      echo "installed"
    else
      echo "not installed"
    fi
  done
fi



# Everything else needs to be run as root
[ -z "$module" -a "$do_uninstall" == "false" ] && {
  exit 0
}
if [ $(id -u) -ne 0 ]; then
  printf "Script must be run as root. Try 'sudo $0'\n"
  exit 1
fi



### Uninstall all modules ###
if [ "$do_uninstall" == "true" ];then
  for i in $RPI_HATS; do
    overlay=${overlay_map[$i]}
    echo Uninstall $i ...
    do_overlay $overlay 1
  done
fi


### Install specified module ###
if [ -n "$module" ]; then
  # enable i2c interface
  echo Enable I2C interface ...
  do_i2c 0

  # echo module=$module
  echo Install $module ...
  overlay=${overlay_map[$module]}
  do_overlay $overlay 0

  # extra module specific installation
  if [ "`type -t ${module}_install 2>/dev/null`" == "function" ]; then
    ${module}_install
  fi
fi

sync()

echo "#######################################################"
echo "Reboot the system to take a effect of Install/Uninstall"
echo "#######################################################"
exit 0

