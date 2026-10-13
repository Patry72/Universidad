/*
 * File: test2.c
 * Author: Sergio Romero Montiel
 * Date: 2018-04-23
 *
 * gcc -Wall test2.c -o test2 -Iinclude -Llib -lmapio
 * (se enlaza con la libreria libmapio.a)
 */
#include <stdio.h>
#include <unistd.h>

#include "mapio.h"


int main()
{
	mmap_io(0x3F000000+0x200000);   // 0x200000 indica dir de los pines

	// Configura el puerto 9 como salida
								/* 9876543210 -> 3 bits each */
	*((unsigned *) 0x3F200000) &= 00777777777;	/* octal number!!! */
	*((unsigned *) 0x3F200000) |= 01000000000;	/* 010 -> output */

	*((unsigned *) 0x3F20001C) = 0x200;		/* set */   
	sleep(1);
	*((unsigned *) 0x3F200028) = 0x200;		/* clear */

	return 0;
}
