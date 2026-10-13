/*
 * p3e1_LeePotenciometro_MuestraLeds_Int.c
 *
 * Created: 22/03/2023 20:28:19
 * Author : Patricia Cárdenas Fernández
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void initLeds (void);
void digitalWrite (unsigned char);
void initADC (void);
ISR (ADC_vect);

int main(void)
{
    /* Replace with your application code */
	
    // Desactivamos interrupciones para no interferir en la configuración
    cli();
    
    initLeds();
    initADC();
    
    // Activamos interrupciones
    sei();
	
	while (1) 
    {
		// No hacemos nada, esperamios a que ocurran las interrupciones
    }
}

// Rutina de tratamiento de interrupción por conversión de ADC
ISR (ADC_vect)
{
	// Mostramos leds según el daro guardado en ADCH
	digitalWrite(ADCH);
	
	// Volvemos a inicializar leds y a iniciar la conversión
	initLeds();
	ADCSRA |= (1 << ADSC);
}

void initADC (void)
{
	// Configuramos ADMUX (la patilla AREF es para un voltaje de referencia externo)
	ADMUX = (1 << REFS0) + (1 << ADLAR) + (1 << MUX2) + (1 << MUX0);      // ADMUX = 0    1    1    0    0    1    0    1
	//						                                    ADcc en patilla AREF-^----^    ^         ^----^----^----^-Canal 5
	//																			Ajustado a izq-^
	
	// Configuramos ADCSRA
	ADCSRA = (1 << ADEN) + (1 << ADIE) + (1 << ADPS2);      // ADCSRA = 1    0    0    0    1    1    0    0
	//													 ADC habilitado-^                   ^    ^----^----^-Divisor reloj en 16
	//																			            ^-Interr habilitadas
	
	// Iniciamos la conversión
	ADCSRA |= (1 << ADSC);
}

void initLeds (void)
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

