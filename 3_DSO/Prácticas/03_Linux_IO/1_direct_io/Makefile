#-----------------------------------------------------------------------
# File: Makefile
#
# Usage:
# $ make 								// same as $make all
# $ make all		// compiles every source code file to diferent execs
# $ make <source_file_w/o_extension>	// compiles one single program
#
# Author: Sergio Romero Montiel
#
# Created on April 25th, 2018
#-----------------------------------------------------------------------

#-----------------------FILES-------------------------------------------
# Lista de fuentes
SOURCES_S := ${wildcard *.s}
SOURCES_C := ${wildcard *.c}
INCDIR := ./include
LIBDIR := ./lib
# Ejecutables
EXECS_S := ${SOURCES_S:.s=}
EXECS_C := ${SOURCES_C:.c=}
LIB := libmapio.a
#-----------------------TOOLS-------------------------------------------
# Compiladores y Enlazadores (no modificar, usamos los por defecto)
#CC = gcc
# Opciones para el compilador
CFLAGS = -Wall -I$(INCDIR)
# ----------------------RULES-------------------------------------------
# Targets y sufijos
.PHONY: all clean
# regla para obtener todos los ejecutables
all: $(EXECS_S) $(EXECS_C)
$(EXECS_S): $(LIBDIR)/$(LIB)
$(EXECS_C): $(LIBDIR)/$(LIB)
$(LIBDIR)/libmapio.a: mapio/mapio.o
	ar rvs $(LIBDIR)/libmapio.a mapio/mapio.o
mapio/mapio.o: mapio/mapio.c
clean:
	-rm -fv $(EXECS_S) $(EXECS_C) mapio/mapio.o
#-----------------------------------------------------------------------
