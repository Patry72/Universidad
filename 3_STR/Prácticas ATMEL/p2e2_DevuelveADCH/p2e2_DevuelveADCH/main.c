/*
 * p2e2_DevuelveADCH.c
 *
 * Created: 11/03/2023 20:59:22
 * Author : Usuario
 */ 

#include <avr/io.h>

void initADC (void)
{
	// Configuramos ADMUX (la patilla AREF es para un voltaje de referencia externo)
	ADMUX = (1 << REFS0) + (1 << ADLAR) + (1 << MUX2) + (1 << MUX0);      // ADMUX = 0    1    1    0    0    1    0    1
	//						                                    ADcc en patilla AREF-^----^    ^         ^----^----^----^-Canal 5
	//											    Ajustado a izq-^
	
	// Configuramos ADCSRA
	ADCSRA = (1 << ADEN) + (1 << ADPS2);      // ADCSRA = 1    0    0    0    0    1    0    0
	//									   ADC habilitado-^                        ^----^----^-Divisor reloj en 16
}

unsigned char getADCH (void)
{
	
	initADC();
	
	// Iniciamos el conversor ADC
	ADCSRA |= (1 << ADSC);      // ADCSRA = 1    1    0    0    0    1    0    0
	//											 ^-inicia ADC
	
	// Esperamos a que acabe la conversión
	while (ADCSRA & (1 << ADSC));   // Saldrá cuando x0xxxxxx & 010000000 = 00000000, osea cuando ADSC se ponga a 0
	
	// Devolvemos el registro ADCH
	return ADCH;
}

int main(void)
{
    /* Replace with your application code */
	
	unsigned char adch = getADCH();
	
    while () 
    {
    }
}

