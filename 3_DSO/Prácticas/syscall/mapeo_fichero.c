/* accedemos al fichero mapeandolo en memoria
   comprobar con strace las llamadas realizadas */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>


int main(int argc, char *argv[])
{
int fd;
int i, pagesize;
char *datos;
char buf[256];

/* abrimos el fichero */
fd=open("prueba.out", O_RDWR | O_CREAT , (mode_t)0600);
 
pagesize = getpagesize();
 
/* mapeamos la primera página del fichero en memoria */
/* si accedemos a espacio fuera del tamaño real del fichero -> SEG FAULT */
datos = mmap(NULL, pagesize /*tamaño*/, PROT_WRITE | PROT_READ, MAP_SHARED, fd , 0 /*offset*/);
															// Devuelve la dirección del mapeo en memoria

/* leemos los 5 primeros caracteres del fichero a traves del mapeo y los pintamos en pantalla */ 
for(i=0; i<5; i++) write(1,datos+i,1);

/* escribimos en memoria -> fichero */
sprintf(datos,"Hello");

/* leemos del fichero */
read(fd,buf,6);

/* pintamos la lectura del fichero */
printf("\n%s\n",buf);

/* libreamos el mapeo de memoria y cerramos el fichero */
munmap(datos,pagesize);
close(fd);	

exit(0);
}
