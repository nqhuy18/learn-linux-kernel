#Toolchain
TOOLCHAIN := /usr/bin
PREFIX := aarch64-linux-gnu-

#ARCH
ifeq (${ARCH}, arm)
	CC := $(TOOLCHAIN)/$(PREFIX)gcc
endif

all:
	$(CC) -o receiver receiver.c
	$(CC) -o sender sender.c	
clean:
	rm -rf receiver sender