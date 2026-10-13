/* diferencias en el buffering entre las llamadas al sistema y las llamadas a funciones de librería
   usar strace para comprobar las llamadas que se hacen realmente  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main()
{

FILE * f;    // Tipo fichero (stream) en C
int i, fd;
char *datos;   

/* Copiamos un string en datos */
datos=strdup("Hola ");   // Crea un string dinámicamente y devuelve un ptr de la copia
						 // Trabaja con memoria, no como strcopy()

/* Escribimos en un fichero 1000 veces 5 chars, en total 5000 bytes */
f=fopen("prueba.out","w+");

i= setvbuf(f,malloc(10000),_IOFBF,10000);   // Cambia a f el buffer a tipo full bufered (_IOFBF)
										    // El 2º arg es el nuevo buffer de tamaño >= que el 4º arg
										    // Devuelve 0 en éxito

if(i) perror("SETVBUF:");

for(i=0; i<1000; i++)
  fwrite(datos, sizeof(char), 5, f);

/*vamos al principio*/
fseek(f,0,0);  // Cambia el indicador de posición a la secuencia a la que apunta f con 
			   // 0 B de offset (2º arg) desde la pos indicada por el 3º arg

/*leemos de este fichero 3 veces 5 chars, en total 15 bytes */
fread(datos,sizeof(char),5,f);
fread(datos,sizeof(char),5,f);
fread(datos,sizeof(char),5,f);

fclose(f);

/*leemos otra vez pero con la llamada al sistema en vez de usar la libreria de c */
fd=open("prueba.out",O_RDONLY);

read(fd, datos, 5);
read(fd, datos, 5);
read(fd, datos, 5);

close(fd);

free(datos);
exit(0);

}
