#include <avr/io.h>
#define vel_max	...

int main() {
	// Configuramos pines
	DDRB = 0x00;	// Input
	DDRC = (1 << PINC1) | (1 << PINC2) | (1 << PINC3);	// input / output
	
	PORTC = (1 << PINC2) | (1 << PINC3);
	
	while (1) {
		float velocidad;
		unisgned char d;
		
		d = PINC & 0x01;	// v
		
		if (d) {
			velocidad = PINB;
			
			// Generamos un pulso (ponemos a 1 y luego a 0)
			PORTC |= (1 << PINC1);	// ACK
			PORTC &= ~(1 << PINC1);
			
			// Si supera la velocidad máxima desactivamos enable y start
			if (velocidad > vel_max) {
				PORTC &= ~(1 << PINC2);
				PORTC &= ~(1 << PINC3);
			}
		}
	}
}