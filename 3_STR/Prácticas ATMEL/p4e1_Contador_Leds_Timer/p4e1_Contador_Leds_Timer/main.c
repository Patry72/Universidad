/*
 * p4e1_Contador_Leds_Timer.c
 *
 * Created: 27/04/2023 10:28:27
 * Author : patri
 */  

#include <avr/io.h>
#include <avr/interrupt.h>

#define WAIT_250ms 62500

uint8_t contador = 0x01;

void initLeds (void)
{
	DDRB = 0x07;
	PORTB = 0x00;
	DDRC = 0x18;
	PORTC = 0x00;
	DDRD = 0xB0;
	PORTD = 0x00;
}

void digitalWrite (uint8_t data)
{
	/* Inicialmente apagamos los leds para que no se queden encendidos */
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	int i;
	unsigned char f;
	for (i = 0; i < 8; i++) {
		// Vamos desplazando una máscara bit a bit
		f = data & (0x01 << i);
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

/* Configuración del Timer 1 en modo CTC */
void configTimer1 (void)
{
	/* Configuramos en modo CTC con preescalado de 64 */
	TCCR1A	= 0x00;			  //   0    0    0    0    0    0    0
	//                                                      ^----^-Modo CTC
	TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);   //   0    0    1    0    0    1    1
	//										                Modo normal-^----^    ^----^----^-Preescalado 64
	
	/* Configuramos palabra de cuenta (OCR1A) */
	OCR1A = WAIT_250ms;
	
	/* Eliminamos cualquier flag de irq anterior y activamos interrupción para cada vez que se llegue al valor de comparación */
	TIFR1 = 0x00;
	TIMSK1 = 1 << OCIE1A;   // OCIE1A => comparación en OCR1A
}

/* Rutina de tratamiento para comparación de OCR1A del Timer 1 */
ISR (TIMER1_COMPA_vect)
{
	/* 
	 * Mientras el contador no sea el máximo vamos mostrando e incrementando 
	 * Si llegamos al máximo, mostramos y reiniciamos contador a 1
	 */
	
	if (contador < 0xFF) {
		digitalWrite(contador++);
	} else if (contador == 0xFF) {
		digitalWrite(contador);
		contador = 0x01;
	}
}

int main (void)
{
    /* Replace with your application code */
	
	/* Desahilitamos interrupciones para que no afecten a la configuración */
	cli();
	
	/* Configuramos Leds y Timer 1 (16 bits) */
	initLeds();
	configTimer1();
	
	/* Habilitamos interrupciones */
	sei();
	
    while (1) 
    {
		// No hacemos nada
    }
}

