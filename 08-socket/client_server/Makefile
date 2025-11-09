#Toolchain
TOOLCHAIN := /usr/bin
PREFIX := aarch64-linux-gnu-

#ARCH
ifeq (${ARCH}, arm)
	CC := $(TOOLCHAIN)/$(PREFIX)gcc
endif

all:
	$(CC) -o server server.c
	$(CC) -o client client.c	
clean:
	rm -rf server client