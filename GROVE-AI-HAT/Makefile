obj-m				+= grove-ai-1-mic.o


DEST := /lib/modules/$(uname_r)/kernel
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	sudo cp grove-ai-1-mic.ko ${DEST}/sound/soc/codecs/
	sudo depmod -a
