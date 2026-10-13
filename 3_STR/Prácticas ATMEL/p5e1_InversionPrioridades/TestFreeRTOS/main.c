#include <avr/io.h>
#include <avr/interrupt.h>
#include "utils_planif.h"
#include "semphr.h"

// Ahora modificamos las prioridades

/******************************************************************************
 * Problem data.
 ******************************************************************************/
// Num tasks
unsigned char num_tasks=3;

// Tasks periods for each task
TickType_t taskPeriod[]={150,100,90};

//Computation times for each task 
unsigned int computationTime[]={100,80,60};
	 
//priorities for each task, the highest the more priority. Starting from 1.
unsigned char taskPrio[]={3,2,1};
	 
//first_activations (phase) for each task, the highest the more priority. Starting from 1.
unsigned int start[]={50,70,0};
	
SemaphoreHandle_t xSemaphore;
	 
 static void T1_func(void* pvParameters);
 static void T2_func(void* pvParameters);
 static void T3_func(void* pvParameters);

int main(void)
 {

	//Init HW, mainly the serial communication
	InitHW();
	
	//Semaphore creation
	xSemaphore = xSemaphoreCreateBinary();	xSemaphoreGive(xSemaphore);

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

// Tarea que usa semáforo para adquirir y soltar recurso
static void T1_func(void* pvParameters)
{
	vTaskSetApplicationTaskTag( NULL, ( void * ) 1 );
	TickType_t xLastWakeTime;
	xLastWakeTime = 0;
	for ( ;; )
	{
		// Primer tiempo de cómputo antes de coger el recurso (1 es el indicador de la tarea, 20 los ms de cómputo)
		consumeCPU(1,20);
		if( xSemaphore != NULL )
		{
			// La tarea 1 se bloquea por no conseguir el recurso (3), 0 es el recurso
			my_trace(xTaskGetTickCount(),1,3,0);
			if( xSemaphoreTake( xSemaphore, portMAX_DELAY))
			{
				//trace_data: tick, tag, userdata=1 for taking the resource
				my_trace(xTaskGetTickCount(),1,1,0);
				//use tag 0 for the rest
				consumeCPU(0,40);
				//userdata=2 when releasing the resource
				my_trace(xTaskGetTickCount(),1,2,0);
				xSemaphoreGive( xSemaphore );
				
				consumeCPU(0,40);   // DUDA
			}
			//userdata=3 eagering for the resource
		}
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

// Tarea que usa semáforo para adquirir y soltar recurso   *** ACABAR ***
static void T3_func(void* pvParameters)
{
	vTaskSetApplicationTaskTag( NULL, ( void * ) 3 );
	TickType_t xLastWakeTime;
	xLastWakeTime = 0;
	for ( ;; )
	{
		// Primer tiempo de cómputo (3 es el indicador de la tarea, 10 los ms de cómputo)
		consumeCPU(3, 10);
		if( xSemaphore != NULL )
		{
			// La tarea 3 accede al recurso (1), 0 es el recurso
			my_trace(xTaskGetTickCount(),3,1,0);
			if( xSemaphoreTake( xSemaphore, portMAX_DELAY))
			{
				//trace_data: tick, tag, userdata=1 for taking the resource
				my_trace(xTaskGetTickCount(),3,1,0);
				//use tag 0 for the rest
				consumeCPU(0,30);
				//userdata=2 when releasing the resource
				my_trace(xTaskGetTickCount(),3,2,0);
				xSemaphoreGive( xSemaphore );
			}
			//userdata=3 eagering for the resource
		}
		vTaskDelayUntil( &xLastWakeTime, taskPeriod[2] );
	}
}
