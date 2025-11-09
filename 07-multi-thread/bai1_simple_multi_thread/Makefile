#Toolchain
TOOLCHAIN := /usr/bin
PREFIX := aarch64-linux-gnu-

#ARCH
ifeq (${ARCH}, arm)
	CC := $(TOOLCHAIN)/$(PREFIX)gcc
endif

all:
	$(CC) -o semaphore_mutex semaphore_mutex.c
clean:
	rm -rf semaphore_mutex
	rm -f fsemaphore*