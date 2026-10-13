/*
 * p2e1_InicializacionADC.c
 *
 * Created: 11/03/2023 20:50:53
 * Author : Usuario
 */ 

#include <avr/io.h>

void initADC (void)
{
	// Configuramos ADMUX (la patilla AREF es para un voltaje de referencia externo)
	ADMUX = (1 << REFS0) + (1 << ADLAR);      // ADMUX = 0    1    1    0    0    0    0    0
	//						        ADcc en patilla AREF-^----^    ^         ^----^----^----^-Canal 0
	//											    Ajustado a izq-^
	
	// Configuramos ADCSRA
	ADCSRA = (1 << ADEN) + (1 << ADPS2);      // ADCSRA = 1    0    0    0    0    1    0    0
	//									   ADC habilitado-^                        ^----^----^-Divisor reloj en 16
}

int main(void)
{
    /* Replace with your application code */
	
	initADC();
	
    while (1) 
    {
    }
}

