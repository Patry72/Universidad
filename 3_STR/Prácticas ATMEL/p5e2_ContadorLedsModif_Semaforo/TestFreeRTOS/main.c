#include "FreeRTOS.h"
#include "task.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "semphr.h"

// GLOBAL VARS
unsigned char contador = 0;
SemaphoreHandle_t mi_semaforo;

void initLeds (void)
{
	DDRB = 0x07;
	PORTB = 0x00;
	DDRC = 0x18;
	PORTC = 0x00;
	DDRD = 0xB0;
	PORTD = 0x00;
}

unsigned char decimalBinario (int n)
{
	unsigned char bin = 0x00;
	int decimal = n;
	int resto = decimal % 2;   // nos quedamos con el resto
	decimal = decimal / 2;   // nos quedamos con el cociente
	int i = 0;
	
	while (i < 8) {
		if (resto == 1) {
			bin = bin | (0x01 << i);
		}
		
		++i;
		resto = decimal % 2;
		decimal = decimal / 2;
	}
	
	return bin;
}

void digitalWrite (int n)
{
	/* Inicialmente apagamos los leds para que no se queden encendidos */
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	
	int i;
	unsigned char data = decimalBinario(n);
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
	
	// Añadimos retraso
	vTaskDelay();
}

void incrementarContador (void* parameter)
{
	while(mi_semaforo == NULL); // Wait for semaphore to be ready
	for (;;)
	{
		// See if we can obtain the semaphore. If the semaphore is not
		// available wait 10 ticks to see if it becomes free.
		if(xSemaphoreTake(mi_semaforo, ( TickType_t ) 10 ) == pdTRUE )
		{
			// WE GOT THE SEMAPHORE
			contador++;
			digitalWrite(contador);
			} else {
			// WE COULD NOT GET THE SEMAPHORE, WAIT AGAIN
		}
	}
}

// MAIN PROGRAM
int main (void)
{
	// INITIALIZE LEDS
	initLeds();
	
	// CONFIGURE BUTTONS & IRQ
	DDRD = 0x00;
	EIMSK = 0x01;   // Habilitamos INT0
	EICRA = 0x02;   // Activación por flanco de bajada
	EIFR = 0x00;    // Borramos flag register (opcional)
	// PCICR = 0x01;
	// PCMSK0 = 0x10;
	
	sei();
	
	// CREATE BLINKER TASK AND ITS SEMPAHORE
	mi_semaforo = xSemaphoreCreateBinary();
	xTaskCreate(incrementarContador, "mitask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	
	// START SCHELUDER
	vTaskStartScheduler();
	
	return 0;
}

// IDLE TASK
void vApplicationIdleHook (void)
{
	// THIS RUNS WHILE NO OTHER TASK RUNS
}

ISR (INT0_vect)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(mi_semaforo, &xHigherPriorityTaskWoken);
}