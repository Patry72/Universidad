/*
 * File: c_std_io.c
 * Author: Sergio Romero Montiel
 * Date: 2019-10-03
 *
 * gcc -Wall c_std_io.c -o c_std_io
 */



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PIN "27"
#define MODE "out"
#define ON "1"
#define OFF "0"
#define EXPORT_FILE "/sys/class/gpio/export"
#define UNEXPORT_FILE "/sys/class/gpio/unexport"
#define DIRECTION_FILE "/sys/class/gpio/gpio"PIN"/direction"
#define VALUE_FILE "/sys/class/gpio/gpio"PIN"/value"

#define MAX_TRY 3

FILE* init_gpio()   // Configuraciones
{
	FILE *fp;
	int try=0;
	
	/* GPIO management files creation */
	if (NULL==(fp=fopen(EXPORT_FILE,"w"))) {   // abre fichero EXPORT_FILE
		perror(EXPORT_FILE);
		exit(1);
	}
	fwrite(PIN,1,sizeof(PIN),fp);fflush(fp);   // le escribe 27 (PIN)
	fclose(fp);
	
	/* set the direction to output */
	usleep(100000);   // IMPORTANTE porque el SO tarda un poco en responder al write de arriba
	while (NULL==(fp=fopen(DIRECTION_FILE,"w"))) {
		try++;
		perror(DIRECTION_FILE);
		if (try == MAX_TRY)	exit(1);
		usleep(100000);
	}
	if (try != 0) printf("SOLVED\n");
	fwrite(MODE,1,sizeof(MODE),fp);fflush(fp);
	fclose(fp);	
	
	/* open VALUE_FILE where to write "0" or "1" */
	if (NULL==(fp=fopen(VALUE_FILE,"w"))) {
		perror(VALUE_FILE);
		exit(1);
	}
	
	return fp;	// retorna el fp de VALUE
}

void remove_gpio()
{
	FILE *fp;
	
	/* GPIO management files deletion */
	if (NULL==(fp=fopen(UNEXPORT_FILE,"w"))) {
		perror(UNEXPORT_FILE);
		exit(1);
	}
	fwrite(PIN,1,sizeof(PIN),fp);fflush(fp);
	fclose(fp);
}


int main(int argc, char *argv[])
{
	/* initialize gpio pin returning the associated file */
	FILE *fp = init_gpio(); 
	
	/* perform output on the associated file (fp) */
	fwrite(ON,1,sizeof(ON),fp);fflush(fp);
	usleep(500000);
	fwrite(OFF,1,sizeof(OFF),fp);fflush(fp);
	
	/* close the file when no more output required */
	fclose(fp);	
	
	remove_gpio();
	
	return 0;
}
