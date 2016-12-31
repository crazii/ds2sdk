
# CROSS :=#
CROSS = /opt/dstwo-toolchain/bin/
CC = $(CROSS)mipsel-linux-gcc
AR = $(CROSS)mipsel-linux-gcc-ar rcsv


TOPDIR = .
CONSOLE_DIR = $(TOPDIR)/console
FS_DIR = $(TOPDIR)/fs
KEY_DIR = $(TOPDIR)/key
ZLIB_DIR = $(TOPDIR)/zlib

SRC :=

SSRC :=

INCLUDES := -I../include

INC := $(INCLUDES)

CFLAGS := -mips32 -O3 -flto -ffat-lto-objects -mno-abicalls -fno-pic -fno-builtin \
	   -mlong-calls -mno-shared -fno-common\
	   -fomit-frame-pointer -msoft-float -G 0

include $(CONSOLE_DIR)/console.mk
include $(FS_DIR)/fs.mk
include $(KEY_DIR)/key.mk
include $(ZLIB_DIR)/zlib.mk

#OBJS	:= $(addsuffix .o , $(basename $(notdir $(SRC))))
#SOBJS	:= $(addsuffix .o , $(basename $(notdir $(SSRC))))

OBJS	:= $(SRC:.c=.o)
SOBJS	:= $(SSRC:.S=.o)

all : depend libds2a.a

libds2a.a : $(OBJS) $(SOBJS)
	$(AR) $@ $(OBJS) $(SOBJS)
	cp $@ ../lib

clrscreen:
	@clear
	@clear

clean :
	rm -rf $(OBJS) $(SOBJS) *.a
	rm -f depend

.c.o :
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<
.cpp.o :
	$(CC) $(CFLAGS) $(INC) -fno-rtti -fvtable-gc -o $@ -c $<
.S.o :
	$(CC) $(CFLAGS) $(INC) -D_ASSEMBLER_ -D__ASSEMBLY__ -o $@ -c $<

depend :	Makefile
	$(CC) -MM $(CFLAGS) $(INC) $(SSRC) $(SRC) > $@

sinclude depend

