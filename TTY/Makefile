obj-m := tty.o
KERNEL_BUILD := /lib/modules/$(shell uname -r)/build
modules:
	make -C $(KERNEL_BUILD) M=$(PWD) modules
clean:
	make -C $(KERNEL_BUILD) M=$(PWD) clean
