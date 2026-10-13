/*
 * p2e2_LedCualquiera.c
 *
 * Created: 10/03/2023 18:21:18
 * Author : Usuario
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	
    // (0x01<<2)  =>  Desplazamos 00000001 2 bits a la izq -> 00000100 (PB.2 LED0)
	
	// USANDO ARITMÉTICA
    /*
    * DDRB = (0x01) + (0x01 << 2);   // PB.0 como salida
    * PORTB = (0x01) + (0x01 << 2);  // Escribimos un 1 en PB.2
	*/
	
	// USANDO DIRECTAMENTE LA POSICIÓN DEL PUERTO
	DDRB = (0x07);
	PORTB = (0x07);
    
    while (1)
    {
	    // No hacer nada, sólo queremos que se quede encendido
    }
    
    return 0;
}

