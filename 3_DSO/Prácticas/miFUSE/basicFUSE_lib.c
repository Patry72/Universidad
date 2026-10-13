/*
 * FUSE: Filesystem in Userspace
 * DSO 2014
 * Ejemplo para montar un libro de poesía como sistema de ficheros
 * Cada capítulo del libro será un fichero diferente
 * 
 * Compilarlo con make
 *  
 * uso:  basicFUSE [opciones FUSE] fichero_inicial punto_de_montaje
 * 
 *  ./basicFUSE proverbiosycatares.txt punto_montaje
 * 
*/

#include "basicFUSE_lib.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/***********************************
 * Busca un fichero path en mi estructura de datos
 * */
int buscar_fichero(const char *path, struct structura_mis_datos *mis_datos)   // DUDA: ¿cuándo se llama a buscar_fichero?
{
	/*
	 * Busca en mis_datos -> contenido_ficheros[] el capítulo path + 1 (/"capitulo"), si existe, devuelve su
	 * índice. Si no existe el capítulo, devuelve -1.
	 */
	 
	int i;
	
	for(i = 0; i < mis_datos -> numero_ficheros; i++)
	{
		/* Se comprueba si existe el capitulo path + 1 en path que coincida con el path /"capitulo" */
		if(strcmp(mis_datos -> nombre_ficheros[i], path + 1) == 0)
		{
			return i;
		}
	}
	return -1;
}



/***********************************
 * */
void error_parametros()
{
    fprintf(stderr, "uso:  basicFUSE [opciones FUSE] fichero_inicial punto_de_montaje\n");
    exit(-1);
}

void leer_fichero(struct structura_mis_datos *mis_datos)   // *** DUDA *** //
{
	/*
	 * Abre el fichero que contiene los capítulos y va guardando en mis_datos -> nombre_ficheros[] el nombre de
	 * cada uno de los capítulos conforme los lee. También va guardando en mis_datos -> contenido_ficheros[] el
	 * contenido de cada capítulo conforme los lee.
	 */
	
	FILE *f;
	int i = 0;
	char nombre[MAX_NOMBRE];
	char contenido[MAX_CONTENIDOS];
	struct stat fileStat;
	
	/* Abre el fichero que contiene los capítulos */
	f = fopen(mis_datos -> fichero_inicial, "rc");
	if (f == NULL) error_parametros();
	
	nombre[0] = 'X';
	
	/* Comprueba que no sea el final del fichero */
	while (!feof(f))
	{
		/* Leemos del fichero con el formato especificado (?) y lo almacenamos en la siguiente pos de nombre */
		if(fscanf(f, " X%[LXVI]s ", nombre + 1))
		{
			/* Vamos inicializando nombre_ficheros con los capítulos, y contenido_ficheros con el contenido de cada uno */
			if(strcmp(nombre, "XX") == 0) 
				mis_datos -> nombre_ficheros[i] = strdup("largo");
			else
				mis_datos -> nombre_ficheros[i] = strdup(nombre);
			fscanf(f, " %[^X]s ", contenido);
			mis_datos -> contenido_ficheros[i] = strdup(contenido);
			i++;
		}
	}
	
	/* Actualizamos datos de la estructura */
	mis_datos -> numero_ficheros = i;
	
	fstat(fileno(f), &fileStat);   // fstat obtiene la info asociada al descriptor de f (devuelto por fileno(f)) y la almacena en fileStat
	
	// También podríamos crear un campo tipo stat en mis_datos y copiar la estructura fileStat
	
	mis_datos -> st_uid = fileStat.st_uid;
	mis_datos -> st_gid = fileStat.st_gid;
	
	mis_datos -> st_atime = fileStat.st_atime;
	mis_datos -> st_ctime = fileStat.st_ctime;
	mis_datos -> st_mtime = fileStat.st_mtime;
		      
	fclose(f);
}
