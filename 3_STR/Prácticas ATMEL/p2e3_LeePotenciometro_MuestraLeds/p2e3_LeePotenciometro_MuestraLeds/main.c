/*
 * p2e3_LeePotenciometro_MuestraLeds.c
 *
 * Created: 12/03/2023 20:18:56
 * Author : Patricia Cárdenas Fernández
 */ 

#include <avr/io.h>

void initGPIOS ()
{
	DDRB = 0x07;
	PORTB = 0x00;
	DDRC = 0x18;
	PORTC = 0x00;
	DDRD = 0xB0;
	PORTD = 0x00;
}

void digitalWrite (unsigned char data)
{
	int i;
	for (i = 0; i < 8; i++) {
		// Vamos desplazando una máscara bit a bit
		unsigned char f = data & (0x01 << i);
		if (f != 0x00) {   // si el bit i está a 1 encender
			switch (i) {
				case 0:    // bit 0 = led 0 -> PB2
				PORTB |= 0x04;
				break;
				case 1:   // bit 1 = led 1 -> PB1
				PORTB |= 0x02;
				break;
				case 2:   // bit 2 = led 2 -> PB0
				PORTB |= 0x01;
				break;
				case 3:   // bit 3 = led 3 -> PD7
				PORTD |= 0x80;
				break;
				case 4:   // bit 4 = led 4 -> PD5
				PORTD |= 0x20;
				break;
				case 5:   // bit 5 = led 5 -> PD4
				PORTD |= 0x10;
				break;
				case 6:   // bit 6 = led 6 -> PC4
				PORTC |= 0x10;
				break;
				case 7:   // bit 7 = led 7 -> PC3
				PORTC |= 0x08;
				break;
				default:
				break;
			}
		}
	}
}

void initADC (void)
{
	// Configuramos ADMUX (la patilla AREF es para un voltaje de referencia externo)
	ADMUX = (1 << REFS0) + (1 << ADLAR) + (1 << MUX2) + (1 << MUX0);      // ADMUX = 0    1    1    0    0    1    0    1
	//						                                    ADcc en patilla AREF-^----^    ^         ^----^----^----^-Canal 5
	//																			Ajustado a izq-^
	
	// Configuramos ADCSRA
	ADCSRA = (1 << ADEN) + (1 << ADPS2);      // ADCSRA = 1    0    0    0    0    1    0    0
	//									   ADC habilitado-^                        ^----^----^-Divisor reloj en 16
}

unsigned char getADCH (void)
{
	// Inicializamos ADC
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
	
	initGPIOS();
	
	unsigned char adch;
	
    while (1) 
    {
		// Obtenemos el valor binario de ADCH
		adch = getADCH();
		
		// Mostramos en los leds
		digitalWrite(adch);
		initGPIOS();
    }
}

