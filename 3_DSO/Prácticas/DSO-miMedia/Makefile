KERNEL= $(shell uname -r)
KERNEL_SRC=/lib/modules/${KERNEL}/build

MODULE=mean

obj-m += ${MODULE}.o
 
compile:
	make -C ${KERNEL_SRC} M=${PWD} modules

install:
	sudo insmod ${MODULE}.ko
	sudo dmesg | tail

clean:
	rm -rf *.o *.mod.c *.symvers *.order

uninstall:
	sudo rmmod &{MODULE}
	sudo dmesg | tail
