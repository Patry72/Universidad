#include <avr/io.h>

// Definir palabras de cuenta con defines 

volatile unsigned char estado;
volatile unsigned char dir;

void main (void)
{
	cli();
	estado = 0
	// Inicializar
	sei();
	while(1);
}

ISR (TIMER0_COMPA_vect)
{
	if (estado == 0 || estado = 3) {
		dir++;
		PORTC = dir;
	}
}

ISR (TIMER1_COMPA_vect) {
	if (estado == 2) {
		// Cerrar puertas
		// Reconfigurar Timer1 porque lo usamos también para esperar 3 seg
		
		OCR1A = 8000;
		
		// Activar haz
		
		estado = 3;
	} else {
		// ERROR (aunque no se suele hacer)
	}
}

ISR (PCINT0_vect) { //(*1)
	// Aquí dentro metemos ISR_abierto e ISR_cerrado
	if (PINB & (1<<PINB3)) {
		if (estado == 1) {
			// Activar 3"
			// Parar motor y activar haz
			
			 estado = 2;
		}
	} else if (PINB & (1<<PINB4)) {
		if (estado == 3) {
			// Parar motor y activar haz
			
			esatdo = 0;
		}
	}
}

ISR (INT0_vect) {
	if (estado == 0 || estado == 3) {
		OCR1A = 14000;
		
		// Activar haz
		
		estado = 1;
	} else if (estado == 2) {
		// Reactivar 3"
	}
}

// Los siguientes métodos son de forma esquemática (se hace como en (*1))
void ISR_abierto () {
	if (estado == 1) {
		// Activar 3"
		// Parar motor y activar haz
		
		 estado = 2;
	}
}

void ISR_cerrado () {
	if (estado == 3) {
		// Parar motor y activar haz
		
		esatdo = 0;
	}
}