# DSO 2015, ejemplo de compilación de librería

CC=gcc
AR=ar

lib_dir=../lib

CFLAGS= -O3 -fPIC


.PHONY: shared static clean      # estos no son ficheros

shared: $(lib_dir)/libmi_libreria.so

static: $(lib_dir)/libmi_libreria_static.a

# lib dinámica
$(lib_dir)/libmi_libreria.so: mi_libreria.o
	$(CC) -shared -o $@ $< $(CFLAGS)

# lib estática
$(lib_dir)/libmi_libreria_static.a: mi_libreria.o
	$(AR) rcs $@ $<

# objeto de la lib
mi_libreria.o: libreria.c libreria.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f mi_libreria.o
