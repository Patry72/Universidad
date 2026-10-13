#include <avr/io.h>
#define vel_max	...

unsigned float volatil velocidad;	// volatile para que el compilador no la quite (será modificada en la isr)

// El PINC0 es el que activa la interrupción
isr () {
	velocidad = PINB;
	// Envío pulso ACK
}

void main (void) {
	// Configuramos pines
	
	while (1) {
		if (velocidad > vel_max) {
			// Paramos todo
		}
	}
}