#
#
# Linux device driver for berryclip
#	This gpio board has 1 buzzer (BCM gpio number: 4)
#	                    2 buttons (BCM gpio numbers: 2, 3)
#	                    6 leds (BCM gpio numbers: 9, 10, 11, 17, 22, 27)

# Module purpose:
#	Introduce the kernel gpio interface
#	Show how a linux device module looks like
#		init/cleanup functions
#		kernel error, warn and info messages (dmesg)
#			printk
#		low level gpio input/output
#			gpio_request_one, gpio_direction_output, gpio_free, 
#			gpio_get_value, gpio_set_value, gpio_to_irq,
#		irq handlers (gpio mapping)
#			request_irq
#
#		

# Compiling:
	$ make

# Installing (loading module)
	$ sudo insmod interrupt.ko

# Check if installed (list modules):
	$ lsmod

# What the module do:
#	initiallize leds (output), and the right button (map irq)
#		every time a falling edge is triggered the value represented
#		with the leds increases (lsb:outter red, msb:outter green)
#
#	$ watch -n 1 cat /proc/interrupts

# Uninstalling the module
	$ sudo rmmod interrupt

# To see the messages printed by the interrupt module
	$ dmesg
	$ dmesg -wH  # hold the terminal to show messages as they are printed

