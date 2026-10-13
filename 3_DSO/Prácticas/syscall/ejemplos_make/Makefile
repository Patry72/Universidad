# DSO 2015, ejemplo de compilación con librería

CC=gcc

bin_dir=./bin
lib_dir=./lib
source_lib=./mi_libreria

ejecutable= programa
fuentes= main.c

CFLAGS= -O3 -I $(source_lib)
LDFLAGS= -L $(lib_dir) 


.PHONY: shared static build_lib_sh build_lib_st clean # estos no son ficheros

shared: build_lib_sh $(bin_dir)/$(ejecutable)_sh

static: build_lib_st $(bin_dir)/$(ejecutable)_st


build_lib_sh:
	make -C $(source_lib) shared
	@echo "\nNo olvides incluir el path de la librería dinámica en LD_LIBRARY_PATH\n"

# se puede configurar  /etc/ld.so.conf y ejecutar ldconfig para añadir un path definitivamente
	
build_lib_st:
	make -C $(source_lib) static

$(bin_dir)/$(ejecutable)_sh: $(fuentes) $(lib_dir)/libmi_libreria.so
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS) -lmi_libreria
	

$(bin_dir)/$(ejecutable)_st: $(fuentes) $(lib_dir)/libmi_libreria_static.a
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS) -lmi_libreria_static -static

clean: 
	make -C $(source_lib) clean
	@echo "objetos librería borrados" 
	rm -f $(bin_dir)/$(ejecutable)_?? 
	@echo "Programas borrados"   # la arroba para que no se pinte el comando

