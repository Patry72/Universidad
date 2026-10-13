/*
 * File: test3.c
 * Author: Sergio Romero Montiel
 * Date: 2018-04-23
 *
 * gcc -Wall test3.c -o test3 -Iinclude -Llib -lmapio
 * (se enlaza con la libreria libmapio.a)
 */
#include <stdio.h>
#include <unistd.h>

#include "mapio.h"


int main()
{
	volatile unsigned int*gpio;
	
	gpio = (volatile unsigned int *)mmap_io(0x3F000000+0x200000);

	printf("gpio address: %x %x\n",(unsigned int)gpio,(unsigned int)&gpio[7]);

			/* 9876543210 -> 3 bits each */
	gpio[0] &= 00777777777;	/* octal number!!! */
	gpio[0] |= 01000000000;
	
	gpio[7] = 0x200;		/* set */
	sleep(1);
	gpio[10] = 0x200;		/* clear */
	
	// [0] del 0 al 9
	// [1] del 10 al 19
	// [2] del 20 al 29
	// [3] del 30 al 39 ...

	return 0;
}
