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
 * I/O address: 0x20000000 for RPi1, 0x3F000000 for RPi2 & RPi3
 */
 
.global	main
	
.text
main:
	push {lr}
/* Mapear las direcciones fisicas en la tabla de paginas del proceso */
	ldr		r0,	=0x3F200000   // dirección física que hay que traducir a virtual
	bl		mmap_io   // r0 también es el retorno de mmap_io
	
/* Primero poner el gpio 9 en modo salida */	
	ldr		r0,	=0x3F200000
	ldr		r1, [r0, #0x00]
/* guia bits 	   xx999888777666555444333222111000*/
	mov		r2,	#0b11000111111111111111111111111111
	mov 	r3,	#0b00001000000000000000000000000000
	and		r1, r1, r2
	orr		r1, r1, r3
	str		r1, [r0, #0x00]

/* Despues acceder y escribir un 1 en set para encender */
/* guia bits	   10987654321098765432109876543210 */
	mov		r1, #0b00000000000000000000001000000000
	ldr		r0,	=0x3F200000
	str		r1, [r0, #0x1c]
	
	mov		r0, #1
	bl		sleep
	
/* Despues acceder y escribir un 1 en clear para apagar */	
/* guia bits	   10987654321098765432109876543210 */
	mov		r1, #0b00000000000000000000001000000000   // donde haya un 1 es un 0 en la patilla
	ldr		r0,	=0x3F200000
	str		r1, [r0, #0x28]

	pop {lr}
	bx lr
