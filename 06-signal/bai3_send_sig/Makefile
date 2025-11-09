#Toolchain
TOOLCHAIN := /usr/bin
PREFIX := aarch64-linux-gnu-

#ARCH
ifeq (${ARCH}, arm)
	CC := $(TOOLCHAIN)/$(PREFIX)gcc
endif

all:
	$(CC) -o send_sig send_sig.c
clean:
	rm -rf send_sig