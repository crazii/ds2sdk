# Top make file


all :
	make -C ./gcc
	make -C ./libsrc/base
	make -C ./libsrc
	make -C ./example

clean :
	make -C ./libsrc/base clean
	make -C ./libsrc clean
	make -C ./example clean
