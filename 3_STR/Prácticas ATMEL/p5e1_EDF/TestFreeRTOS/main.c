// Cipriano Galindo Andrades
// Sistemas de Tiempo Real
// Universidad de Malaga

// AVR includes
#include <avr/io.h>
#include <avr/interrupt.h>
// FreeRTOS and scheduling stuff
#include "utils_planif.h"

//Current implementation of EDF does not allow more than 3 tasks
#define min_TASK_PRIORITY   (tskIDLE_PRIORITY+1)
#define max_TASK_PRIORITY   (tskIDLE_PRIORITY+4)

static void T1_func(void* pvParameters);
static void T2_func(void* pvParameters);
static void T3_func(void* pvParameters);

//Task 4 is in charge of computing the online priorities. Naively implemented yet...
static void T4_func(void* pvParameters);

TaskHandle_t * tasklist[3];

unsigned char num_tasks=3;
TickType_t taskPeriod[]={p1,p2,p3};
unsigned int computationTime[]={c1,c2,c3};
unsigned int start[]={0,0,0};
//there is not a priority lists. Tasks have the same priority at the begining 
//and it is changed acording to their dealines

//edf period. Every 50 ms it checks tasks' priorities
unsigned char edfPeriod=50;



int main(void)
 {
	 // Initialize HW

	InitHW();
	
	//The taks handles are neccessary to change their priorities on runtime	
	TaskHandle_t t1,t2,t3;	

	  xTaskCreate
	  (	 T1_func,	 (char*)"T1",	 configMINIMAL_STACK_SIZE,	 NULL,	 min_TASK_PRIORITY,&t1);
	  
	  xTaskCreate
	  (
	  T2_func, (char*)"T2", configMINIMAL_STACK_SIZE,        NULL,        min_TASK_PRIORITY,&t2);
	
	 xTaskCreate
	  (
	  T3_func, (char*)"T3", configMINIMAL_STACK_SIZE,        NULL,        min_TASK_PRIORITY,&t3);

	xTaskCreate
	(
	T4_func, (char*)"T4", configMINIMAL_STACK_SIZE,        NULL,        max_TASK_PRIORITY,NULL);

	  //array of task handles
	  tasklist[0]=t1;
	  tasklist[1]=t2;
	  tasklist[2]=t3;
	  
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
		consumeCPU(2,computationTime[1]); //models the computation time of the task
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

//////Tasks dealing with EDF. DO Not touch
//////Yep, it nos efficient, but it works for a few tasks.
static void T4_func(void* pvParameters)
{
	TickType_t xLastWakeTime;
	TickType_t min;
	TickType_t t;
	TickType_t aux[3];
	TickType_t temp;
	TickType_t now;

	
	unsigned char temp_order;
	
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = 0;
	vTaskSetApplicationTaskTag( NULL, ( void * ) 4 );

	for( ; ;)
	{
		now= xTaskGetTickCount();
		
		unsigned char pos=0;
		min=0xFFFF;
		//reset priorities and compute the current deadline for each task.
		for (unsigned char k=0;k<num_tasks;k++)
		{
			
			vTaskPrioritySet(tasklist[k],min_TASK_PRIORITY);
			t=(int)((double)now/(double)taskPeriod[k])+1;
			aux[k]=(t*taskPeriod[k])-now;
		}
		
		//sorting deadlines
		unsigned char order[3]={0,1,2};
		for (unsigned char i=0;i<num_tasks-1;i++)
		{
			for (unsigned char j=0;j<num_tasks-1-i;j++)
			{
				if (aux[j]>aux[j+1])
				{
					temp=aux[j+1];
					aux[j+1]=aux[j];
					aux[j]=temp;
					
					temp_order=order[j+1];
					order[j+1]=order[j];
					order[j]=temp_order;
				}
			}
		}
		
		//assigining priorities in the same order...
		
		for (unsigned char k=0;k<num_tasks;k++)
		{
			
			vTaskPrioritySet(tasklist[order[k]],max_TASK_PRIORITY-k-1);
			
		}
		
		vTaskDelayUntil( &xLastWakeTime, edfPeriod);
	}//for;;



}

