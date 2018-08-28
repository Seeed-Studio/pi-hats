#/bin/bash

path=/sys/devices/platform/soc

dir=$(ls $path | grep "04000.i2c")
echo $dir
if [ "$dir" != "" ];then
while :
do
    echo "four channels' value are :"
    cat $path/$dir/i2c-1/1-0048/in4_input
    echo ","
    cat $path/$dir/i2c-1/1-0048/in5_input
    echo ","
    cat $path/$dir/i2c-1/1-0048/in6_input
    echo ","
    cat $path/$dir/i2c-1/1-0048/in7_input
    sleep 2
done
else
    echo "can't detect ads-1115"
fi
