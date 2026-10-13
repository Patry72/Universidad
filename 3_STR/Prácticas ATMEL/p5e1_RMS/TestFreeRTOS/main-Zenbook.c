/*
 * TestFreeRTOS.c
 *
 * Created: 01/07/2016 12:18:47
 * Author : Cipri
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "utils_planif.h"


 static void T1_func(void* pvParameters);
 static void T2_func(void* pvParameters);
 static void T3_func(void* pvParameters);

int main(void)
 {
	 
	 

	//Init HW, mainly the serial communication
	InitHW();

	//Tasks creation
	xTaskCreate
	  (	 T1_func,	 (char*)"T1",	 configMINIMAL_STACK_SIZE,	 NULL,	 tskIDLE_PRIORITY+taskPrio[0],	 NULL);

	
	xTaskCreate
	(	 T2_func,	 (char*)"T2",	 configMINIMAL_STACK_SIZE,	 NULL,	 tskIDLE_PRIORITY+taskPrio[1],	 NULL);

	xTaskCreate
	(	 T3_func,	 (char*)"T3",	 configMINIMAL_STACK_SIZE,	 NULL,	 tskIDLE_PRIORITY+taskPrio[2],	 NULL);

	// Start scheduler.
	  vTaskStartScheduler();
	
 }
 
 
 
/******************************************************************************
 * Private function definitions.
 ******************************************************************************/
 
static void T1_func(void* pvParameters)
{
	vTaskSetApplicationTaskTag( NULL, ( void * ) 1 );
	TickType_t xLastWakeTime;
	xLastWakeTime = 0;
	//delay to fulfill the first activation of the task. It can be neglected if it is 0
	vTaskDelayUntil( &xLastWakeTime, start[0] );
	for ( ;; )
	{
		consumeCPU(1,computationTime[0]);
		vTaskDelayUntil( &xLastWakeTime, taskPeriod[0] );
	}
} 


static void T2_func(void* pvParameters)
{
	vTaskSetApplicationTaskTag( NULL, ( void * ) 2 );
	TickType_t xLastWakeTime;
	xLastWakeTime = 0;
	
	//delay to fulfill the first activation of the task. It can be neglected if it is 0
	vTaskDelayUntil( &xLastWakeTime, start[1] );
	for ( ;; )
	{
		consumeCPU(2,computationTime[1]);
		vTaskDelayUntil( &xLastWakeTime, taskPeriod[1] );
	}
}

static void T3_func(void* pvParameters)
{
	vTaskSetApplicationTaskTag( NULL, ( void * ) 3 );
	TickType_t xLastWakeTime;
	xLastWakeTime = 0;
	//delay to fulfill the first activation of the task. It can be neglected if it is 0
	vTaskDelayUntil( &xLastWakeTime, start[2] );
	for ( ;; )
	{
		consumeCPU(3,computationTime[2]);
		vTaskDelayUntil( &xLastWakeTime, taskPeriod[2] );
	}
}
