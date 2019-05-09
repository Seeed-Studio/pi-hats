#dtoverlay -r seeed-dummy-voicecard

dtc -@ -I dts -O dtb -o seeed-dummy-voicecard.dtbo seeed-dummy-voicecard-overlay.dts

# cp *.dtbo /boot/overlays
# dtoverlay seeed-dummy-voicecard
