#include <avr/io.h>

int main (void) {
	
	unsigned char b;
	
	// Configuramos pines
	DDRB |= 0;	// Puerto B como entrada
	DDRC |= 1;	// Puerto C como salida
	DDRD |= (1<<PIND0) | (1<<PIND1) | (1<<PIND2);	// Pines del puerto D como salidas
	
	while (1) {
		
		// Leemos valor del puerto B
		b = PINB;
		
		// Enviamos dato al puerto C
		PORTC = b;
		
		switch (b) {
			case 58:
				PORTD ^= (1<<PIN0);	// Hacemos toggle al pin 0 del puerto D
				break;
			case 69:
				PORTD ^= (1<<PIN1);
				break;
			case 70:
				PORTD ^= (1<<PIN2);
				break;
		}
		
	}
	
}