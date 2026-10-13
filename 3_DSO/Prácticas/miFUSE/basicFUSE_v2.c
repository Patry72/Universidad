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
#define FUSE_USE_VERSION 26

#include "basicFUSE_lib.h"

#include <stdlib.h>
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

/*
 *  Para usar los datos pasados a FUSE usar en las funciones:
 * 
 *  struct structura_mis_datos *mis_datos= (struct structura_mis_datos *) fuse_get_context()->private_data;
 * 
 */

/**************************
 ---- GETATTR FUNCTION ----
 **************************/
 
/* stbuf: ptr a info del fichero */
static int mi_getattr(const char *path, struct stat *stbuf)
{
	/*
	 * El sistema llama a esta función antes de nada para obtener atributos como permisos, quién es el usuario...
	 */
	 
	printf("Welcome to mi_getattr function \\^o^/ !\n");
	
	printf("mi_getattr - Path: %s\n", path);
	
	struct structura_mis_datos *mis_datos= (struct structura_mis_datos *) fuse_get_context() -> private_data;
	
	int i;
	
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	
	/* Comprobamos si el path es el raíz / o si es un fichero: /XXVI , /XXVII , /XXVIII ... */
	if (strcmp(path, "/") == 0) {
		
		/* Inicializamos atributos para / */
		stbuf -> st_mode = S_IFDIR | 0755;   // Directorio de lectura y ejecución para group y other, y todo para user
		stbuf -> st_nlink = 2;
		stbuf -> st_uid = mis_datos -> st_uid;
		stbuf -> st_gid = mis_datos -> st_gid;
		
		stbuf -> st_atime = mis_datos -> st_atime;
		stbuf -> st_mtime = mis_datos -> st_mtime;
		stbuf -> st_ctime = mis_datos -> st_ctime;
		stbuf -> st_size = 1024;
		stbuf -> st_blocks = 2;
		
	} else if ((i = buscar_fichero(path, mis_datos)) >= 0) {
		
		/* Inicializamos atributos para el fichero encontrado */
		stbuf -> st_mode = S_IFREG | 0444;   // Fichero regular de solo lectura
		stbuf -> st_nlink = 1;
		
		stbuf -> st_uid = mis_datos -> st_uid;
		stbuf -> st_gid = mis_datos -> st_gid;
		
		stbuf -> st_atime = mis_datos -> st_atime;
		stbuf -> st_mtime = mis_datos -> st_mtime;
		stbuf -> st_ctime = mis_datos -> st_ctime;
		
		stbuf -> st_size = strlen(mis_datos -> contenido_ficheros[i]);
		stbuf -> st_blocks = stbuf -> st_size/512 + (stbuf -> st_size % 512) ? 1 : 0;
	} else {
		printf("mi_getattr - No entry... :[\n");
		res = -ENOENT;
	}
	
	printf("Attributes set successfully :D !\n");
	
	return res;
}

/**************************
 ---- READDIR FUNCTION ----
 **************************/

/* path: path relativo a nuestro punto de monataje, no desde raíz del PC sino desde raíz del punto de montaje */
static int mi_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	/*
	 * El sistema llama a esta función cuando queremos leer el directorio (cuando hacemos ls).
	 */
	
	printf("Welcome to mi_readdir function \\^o^/ !\n");
	
	struct structura_mis_datos *mis_datos = (struct structura_mis_datos *) fuse_get_context() -> private_data;
	
	int i;

	if (strcmp(path, "/") != 0) {
		printf("mi_readdir - No entry... :[\n");
		return -ENOENT;
	}
	
	/* filler le dice al resto del sistema que existen tales entradas */
	if(filler(buf, "." , NULL, 0) != 0) {
		return -ENOMEM;   
	}
	
	if(filler(buf, "..", NULL, 0) != 0) {
		return -ENOMEM;
	}
	
	/* Hacemos lo mismo para cada entrada (fichero) */
	for (i = 0; i < mis_datos -> numero_ficheros; i++) {
		if (filler(buf, mis_datos -> nombre_ficheros[i], NULL, 0) != 0) {
            return -ENOMEM;
		}
	}
	
	printf("Directory read successfully :D !\n");
	
	return 0;
}

/***********************
 ---- OPEN FUNCTION ----
 ***********************/

/* fi: ptr a info del fichero */
static int mi_open(const char *path, struct fuse_file_info *fi)
{
	/*
	 * El sistema llama a esta función cuando se abre un fichero (para lectura o escritura),
	 * en este caso, lectura.
	 */
	
	/* completar */
	
	printf("Welcome to mi_open function \\^o^/ !\n");
	
	struct structura_mis_datos *mis_datos = (struct structura_mis_datos *) fuse_get_context() -> private_data;
	
	int i;
	
	/* Comprobamos si existe el fichero direccionado por path */
	if ((i = buscar_fichero(path, mis_datos)) >= 0) {
		/* Guardamos su posición para no tener que buscarlo en read */
		fi -> fh = i;
		printf("fi -> fh: %lu\n", fi -> fh);
	} else {
		printf("mi_open - No entry... :[\n");
		return -ENOENT;
	}

	// Comprobamos si tenemos acceso a lectura */
	if ((fi -> flags & 3) != O_RDONLY) {   // los 3 lsb son los modos de open (append, read only,...)
		return -EACCES;
	}
	
	printf("File opened successfully :D !\n");
	
	return 0;
}


/***********************
 ---- READ FUNCTION ----
 ***********************/

/* size: tamaño del dato, offset: por dónde va leyendo */
static int mi_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	/*
	 * El sistema llama a esta función cuando queremos leer el contenido de un fichero (cuando hacemos cat "fichero").
	 * Como en la función mi_open guardamos el índice del fichero, no hace falta volver a buscarlo, basta con 
	 * obtener el índice en fi -> fh.
	 */
	
	/* completar */
	
	printf("Welcome to mi_read function \\^o^/ !\n");
	
	struct structura_mis_datos *mis_datos = (struct structura_mis_datos *) fuse_get_context() -> private_data;
	
	int len;
	uint64_t fd = fi -> fh;   // Obtenemos descriptor (índice) del fichero abierto en la función mi_open
	printf("fd: %lu\n", fd);
	len = strlen(mis_datos -> contenido_ficheros[fd]);
	
	if (offset < len) {
		if (offset + size > len) {
			size = len - offset;
		}
		
		memcpy(buf, (mis_datos -> contenido_ficheros[fd]) + offset, size);   // estamos a nvl de aplicación, no usamos copy_to_user
	} else {
		size = 0;
	}
	
	return size;
}

/*************************
 ---- FUSE OPERATIONS ----
 *************************/

static struct fuse_operations basic_oper = {
	.getattr	= mi_getattr,   
	.readdir	= mi_readdir,
	.open		= mi_open,
	.read		= mi_read,
};


/**********************
 ---- MAIN PROGRAM ----
 **********************/
 
/* El main funciona como polling o servicio */
int main(int argc, char *argv[])   // *** DUDA FUNCIONAMIENTO *** //
{
	printf("*************************\n");
	printf("Hello from main function! :D\n");
	printf("*************************\n");
	
	struct structura_mis_datos *mis_datos;
	
	mis_datos = malloc(sizeof(struct structura_mis_datos));
	
	// análisis parámetros de entrada
	if ((argc < 3) || (argv[argc-2][0] == '-') || (argv[argc-1][0] == '-')) error_parametros();

	mis_datos -> fichero_inicial = strdup(argv[argc-2]); // fichero donde están los capítulos
    argv[argc-2] = argv[argc-1];
    argv[argc-1] = NULL;
    argc--;
    
    /* Leemos el fichero que contiene los capítulos e inicializamos mis_datos */
    leer_fichero(mis_datos);
    
    int i;
    for(i = 0; i < mis_datos -> numero_ficheros; i++)
    {
		printf("----->  %s\n", mis_datos->nombre_ficheros[i]);
		printf("%s",mis_datos->contenido_ficheros[i]);
	}
	// exit(0);
	
	return fuse_main(argc, argv, &basic_oper, mis_datos);
}
