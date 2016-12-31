# This makefile to make jz4740 baselib

CROSS = /opt/dstwo-toolchain/bin/
CC = $(CROSS)mipsel-linux-gcc
AR = $(CROSS)mipsel-linux-gcc-ar rcsv


CLIB_DIR = clib

OBJS := cache.o	\
	clock.o	\
	dma.o	\
	excpt.o	\
	init.o	\
	intc.o	\
	pm.o	\
	timer.o	\
	uart.o	\
	version.o \
	$(CLIB_DIR)/libc.o	\
	$(CLIB_DIR)/memory.o	\
	mmc/mmc.o	\
	nds/game.o	\
	nds/test.o	\
	nds/plug.o	\
	nds/av.o

SOBJS := $(CLIB_DIR)/libc_s.o

CFLAGS	:= -mips32 -O3 -flto -ffat-lto-objects -mno-abicalls -fno-pic -fno-builtin \
	   -mlong-calls -mno-shared -fno-common\
	   -fomit-frame-pointer -msoft-float -G 0
	   

CFLAGS += -I./include -I./mmc -I./nds -I.

all : .depend libds2b.a

libds2b.a : $(OBJS) $(SOBJS)
	$(AR) $@ $(OBJS) $(SOBJS)
	cp $@ ../../lib/$@

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<
.cpp.o:
	$(CC) $(CFLAGS) -fno-rtti -fvtable-gc -o $@ -c $<
.S.o:
	$(CC) $(CFLAGS) -D_ASSEMBLER_ -D__ASSEMBLY__ -o $@ -c $<

clean: 
	rm -r *.o
	rm -r clib/*.o
	rm -r mmc/*.o
	rm -r nds/*.o

cleanall:
	rm -r *.o *.a
	rm -r clib/*.o
	rm -r mmc/*.o
	rm -r nds/*.o

.depend:	Makefile $(OBJS:.o=.c) $(SOBJS:.o=.S)
	$(CC) -MM $(CFLAGS) $(OBJS:.o=.c) $(SOBJS:.o=.S) > $@

sinclude .depend


