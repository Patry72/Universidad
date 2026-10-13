/*
 * File: test.s
 * Author: Sergio Romero Montiel
 * Date: 2018-04-23
 * Compile:
 *   as test.s -o test.o
 *   gcc test.o -o test -Llib -lmapio
 *   |
 *   +-- se enlaza con la libreria libmapio.a del directorio ./lib
 *   |
 *   +-- gcc introduce startfiles, stdlib, ..., como si fuera la funcion main()
 *       de un programa C.
 */
 
 .global	main
	.set	GPBASE,		0x3F200000
	.set	GPFSEL0,	0x00
	.set	GPSET0,		0x1c
	.set	GPCLR0,		0x28
	
.text
main:
	push {r4,lr}
/* Mapear las direcciones fisicas en la tabla de paginas del proceso */
	ldr		r0,	=GPBASE
	bl		mmap_io
	mov		r4, r0
	
/* Primero poner el gpio 9 en modo salida */	
	ldr		r1, [r4, #GPFSEL0]
/* guia bits 	   xx999888777666555444333222111000*/
	mov		r2,	#0b11000111111111111111111111111111
	mov 	r3,	#0b00001000000000000000000000000000
	and		r1, r1, r2
	orr		r1, r1, r3
	str		r1, [r4, #GPFSEL0]

/* Despues acceder y escribir un 1 en set para encender */
/* guia bits	   10987654321098765432109876543210 */
	mov		r1, #0b00000000000000000000001000000000
	str		r1, [r4, #GPSET0]
	
	mov		r0, #1
	bl		sleep
	
/* Despues acceder y escribir un 1 en clear para apagar */	
/* guia bits	   10987654321098765432109876543210 */
	mov		r1, #0b00000000000000000000001000000000
	str		r1, [r4, #GPCLR0]

	pop {r4,lr}
	bx lr
