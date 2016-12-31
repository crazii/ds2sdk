
CC := /opt/dstwo-toolchain/bin/mipsel-linux-gcc

CURR_DIR = ${shell pwd}

all: $(CC)


$(CC):
	mkdir -p /opt
	cd /opt;tar -xf $(CURR_DIR)/dstwo-toolchain-gcc-6.1.2-nopic.tar.xz
	chmod 777 $(CURR_DIR)/../tools/makeplug

