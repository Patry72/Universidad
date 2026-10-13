/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

  gcc -Wall hello.c `pkg-config fuse --cflags --libs` -o hello
*/

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

static const char *hello_str = "Hello World!\n";
static const char *hello_path = "hello";

/* stbuf: devuelve info del fichero */
static int hello_getattr(const char *path, struct stat *stbuf)
{
	printf ("función getattr\n");
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;   // directorio de lectura y ejecución para group y other, y todo para user
		stbuf->st_nlink = 2;
	} else if (strcmp(path+1, hello_path) == 0) {
		stbuf->st_mode = S_IFREG | 0444;   // regular file de solo lectura
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen(hello_str);
	} else
		res = -ENOENT;

	return res;
}

/* path: path relativo a nuestro fichero (punto de monataje), no desde raíz del PC sino /hello */
static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
	printf ("función readdir\n");
	(void) offset;
	(void) fi;

	if (strcmp(path, "/") != 0)
		return -ENOENT;
		
	/* filler le dice al resto del sistema que existen tales entradas */
	if(filler(buf, "." , NULL, 0)!=0) return -ENOMEM;
	if(filler(buf, "..", NULL, 0)!=0) return -ENOMEM;
	if(filler(buf, hello_path, NULL, 0)!=0) return -ENOMEM;

	return 0;
}

/* fi: ptr a info del fichero */
static int hello_open(const char *path, struct fuse_file_info *fi)
{
	printf ("función open\n");
	if (strcmp(path+1, hello_path) != 0)
		return -ENOENT;

	if ((fi->flags & 3) != O_RDONLY)   // los 3 lsb son los modos de open (append, read only,...)
		return -EACCES;

	return 0;
}

/* size: tamaño del dato, offset: por dónde va leyendo */
static int hello_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	printf ("función read\n");
	size_t len;
	(void) fi;
	
	len = strlen(hello_str);
	if (offset < len) {
		if (offset + size > len) size = len - offset;
		memcpy(buf, hello_str + offset, size);   // estamos a nvl de aplicación, no usamos copy_to_user
	} else
		size = 0;

	return size;
}

static struct fuse_operations hello_oper = {
	.getattr	= hello_getattr,   // El sistema llama a esta función antes de nada para obtener atributos como permisos, quién es el usuario...
	.readdir	= hello_readdir,
	.open		= hello_open,
	.read		= hello_read,
};

/* El main funciona como un servicio o polling */
int main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &hello_oper, NULL);   // Si recibe --help muestra un mesaje genérico
}
