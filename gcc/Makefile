
CC := /opt/mipsel-4.1.2-nopic/bin/mipsel-linux-gcc

#dependency with exact name for windows
ifeq "$(OS)" "Windows_NT"
CC := $(addsuffix .exe, $(CC))
endif


CURR_DIR = ${shell pwd}

all: $(CC)


$(CC):
	mkdir -p /opt
	cd /opt;tar -xjf $(CURR_DIR)/mipsel-4.1.2-nopic.tar.bz2
	chmod 777 $(CURR_DIR)/../tools/makeplug

