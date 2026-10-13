MODULE=ejemplo_kthread

KERNEL=`uname -r`
KERNEL_SRC=/lib/modules/${KERNEL}/build
 
obj-m += ${MODULE}.o
 
compile:
	@echo Usando kernel ${KERNEL}
	make -C ${KERNEL_SRC} M=${CURDIR} modules

install: 
	sudo insmod ${MODULE}.ko 
	dmesg | tail 

uninstall:
	sudo rmmod ${MODULE} 
	dmesg | tail
