#include <avr/io.h>

void initADC (void) {
	
	// Configuramos
	ADMUX = (1 << REFS0) | (1 << ADLAR);
	ADCSRA = (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); 
	
}

// Para temporizar la conversión podemos usar un led (encendemos al inicio y apagamos al acabar)
unsigned char ReadADC (unsigned char canal) {
	ADMUX |= (canal & 0x0F);
	ADCSRA |= (1 << ADSC);
	
	// Encendemos led contador
	PORTB |= (1 << PINB5);
	
	while (ADCSRA & (1 << ADSC));
	
	// Apagamos led contador
	PORTB &= ~(1 << PINB5);
	
	return ADCH;
}

void main (void) {
	
	// Inicializamos ADC
	initADC();
	
	// Configuramos pines
	DDRB = (1 << PINB5);
	DDRC = 0x00;
	
	unsigned char b;
	
	while (1) {
		b = ReadADC(5);
		
	}
	
}