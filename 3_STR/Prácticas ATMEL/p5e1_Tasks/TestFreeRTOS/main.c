#include "FreeRTOS.h"
#include "task.h"
#include <avr/io.h>
void taskLedON(void* arguments)
{
	// CONFIGURE PORT B, PIN 5 AS OUTPUT (my LED is here)
	DDRB |= (1<<PB5);
	// ENTER TASK'S LOOP
	while(1)
	{
		PORTB |= (1 << PB5); // Turn LED on.
		vTaskDelay(500); // Wait 500ms
	}
}
void taskLedOFF(void* arguments)
{
	// CONFIGURE PORT B, PIN 5 AS OUTPUT (my LED is here)
	// Note: we have to do it here again just in case the other
	// function did not run before this one does
	DDRB |= (1<<PB5);
	// ENTER TASK'S LOOP
	while(1)
	{
		PORTB &= ~(1 << PB5); // Turn LED off.
		vTaskDelay(250); // Wait 250ms
	}
}
int main(void)
{
	// CREATE ON TASK
	xTaskCreate(taskLedON,
	"ON",
	configMINIMAL_STACK_SIZE,
	NULL,
	tskIDLE_PRIORITY+1,
	NULL);
	// CREATE OFF TASK
	xTaskCreate(taskLedOFF,
	"OFF",
	configMINIMAL_STACK_SIZE,
	NULL,
	tskIDLE_PRIORITY+2,
	NULL);
	// START SCHEDULER
	vTaskStartScheduler();
	return 0;
}
void vApplicationIdleHook(void)
{
	// THIS RUNS WHILE NO OTHER TASK RUNS
}