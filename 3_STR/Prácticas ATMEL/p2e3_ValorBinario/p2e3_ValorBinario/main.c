/*
 * p2e3_ValorBinario.c
 *
 * Created: 10/03/2023 13:10:57
 * Author : Patricia Cárdenas Fernández
 */ 

#include <avr/io.h>

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

int main(void)
{
    /* Replace with your application code */
	
	// Configuramos GPIOs e inicializamos todos apagados
	DDRB = 0x07;
	PORTB = 0x00;
	DDRC = 0x18;
	PORTC = 0x00;
	DDRD = 0xB0;
	PORTD = 0x00;
	
    while (1) 
    {
		digitalWrite(0xFF);
    }
}

