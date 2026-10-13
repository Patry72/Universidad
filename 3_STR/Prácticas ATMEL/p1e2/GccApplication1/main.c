/*
 * GccApplication1.c
 *
 * Created: 09/03/2023 12:06:45
 * Author : Usuario
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	
	// (0x01<<2)  =>  Desplazamos 00000001 2 bits a la izq -> 00000100 (PB.2 LED0)
	
	DDRB = (0x01<<2);   // PB.2 como salida
	PORTB = (0x01<<2);  // Escribimos un 1 en PB.2
	
	while (1)
	{
		// No hacer nada, sólo queremos que se quede encendido
	}
	
	return 0;
}

