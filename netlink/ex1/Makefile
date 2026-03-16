obj-m += nl_kernel.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc nl_user.c -o nl_user

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm nl_user