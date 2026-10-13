/*
 * p4_Parking.c
 *
 * Created: 17/04/2023 13:04:08
 * Author : Patricia Cárdenas Fernández
 */ 

/*
 * Led 7 -> semáforo de entrada  Led 6 -> semáforo de salida  
 * Leds 2, 3, 4 -> contador (binario de coches)
 * Led 0 -> parpadeo barrera
 */

#include <avr/io.h>

volatile unsigned char estado;
static unsigned int direccion;   // 0 -> coche entra;  1 -> coche sale

void ISR_sensor_entrada (void);
// void ISR_sensor_entrada2 (void);
void ISR_sensor_salida (void);
// void ISR_sensor_salida2 (void);
void ISR_barerra_abierta (void);
void ISR_barrera_cerrada (void);

void ISR_sensor_entrada (void)
{
	if (estado == 0 && direccion == 0) {
		// motor_on_subiendo
		// parpadeo_on
		// sem_salida_off
		// estado = 1
	} else if (estado == 0 && direccion == 1) {
		// motor_on_subiendo
		// parpadeo_on
		// sem_entrada_off
		// estado = 2
	}
}

void ISR_sensor_salida (void)
{
	if (estado == 0 && direccion == 0) {
		// motor_on_bajando
		// parpadeo_on
		// sem_salida_on
		// estado = 3
	} else if (estado == 0 && direccion == 1) {
		// motor_on_bajando
		// parpadeo_on
		// sem_entrada_on
		// estado == 3
	}
}

void ISR_barerra_abierta (void) 
{
	if (estado == 1)
}

void ISR_barrera_cerrada (void) 
{
	
}

int main(void)
{
    /* Replace with your application code */
	
	/*
	 * - irqs_disable.
	 * - estado = 0.
	 * - motor_off.
	 * - parpadeo_off.
	 * - sem_entrada_on.
	 * - sem_salida_on.
	 * - irqs_enable.
	 */
	
    while (1) 
    {
    }
}

