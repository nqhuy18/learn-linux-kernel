#Toolchain
TOOLCHAIN := /usr/bin
PREFIX := aarch64-linux-gnu-

#ARCH
ifeq (${ARCH}, arm)
	CC := $(TOOLCHAIN)/$(PREFIX)gcc
endif

all:
	$(CC) -o simple_signal simple_signal.c
clean:
	rm -rf simple_signal