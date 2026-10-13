# ejemplo de uso FUSE para DSO 2020
# hemos instalado el paquete de desarrollo de FUSE con:
# sudo apt-get install fuse-dev
# -D... es equivalente a poner un DEFINE en el código c

fuse_flags= -D_FILE_OFFSET_BITS=64 -lfuse -pthread

.PHONY : mount umount test

hello_fuse : hello_fuse.c
	gcc -o $@ $^ ${fuse_flags}
	mkdir -p hola
mount : 
	./hello_fuse hola
debug :
	./hello_fuse -d hola
umount :
	fusermount -u hola
test : 
	ls hola
	cat hola/hello

