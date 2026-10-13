#include "libreria.h"
#include <stdlib.h>

int main(int argc, char**argv)
{
	if(argc>1) 
		funcion_mensaje(argv[1]);
	else
		funcion_mensaje("No hay argumentos");
	
	exit(0);
}
