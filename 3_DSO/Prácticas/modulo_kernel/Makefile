KERNEL= $(shell uname -r)
KERNEL_SRC=/lib/modules/${KERNEL}/build

MODULE=ejemplo_modulo

obj-m += ${MODULE}.o

.PHONY : compile clean
 
compile:
	make -C ${KERNEL_SRC} M=${CURDIR} modules

clean:
	rm -rf *.o *.mod.c *.symvers *.order
