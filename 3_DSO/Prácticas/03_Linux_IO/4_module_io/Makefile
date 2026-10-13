KERNEL:= $(shell uname -r)
KERNEL_SRC:=/lib/modules/${KERNEL}/build

obj-m += gpioirq.o

all:
	make -C ${KERNEL_SRC} M=${CURDIR} modules
	
