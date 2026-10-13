/* poker de formas de llamar al sistema desde C 
   comprobar con strace las llamadas que se realizan */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */

int main()
{
int res;
char * mensaje1="Salida write con int 80h desde asm\n";  /* tamaño 35 caracteres */
char * mensaje2="Salida write con SWI/SVC desde asm\n";  /* tamaño 35 caracteres */

/*Pinta por pantalla usando la librería de C stdio.h*/
printf("Salida printf() desde C\n");

/* para hacer flush:  fflush(stdout); */

/* pinta por pantalla usando el envoltorio de la llamada al sistema en unistd.h */
write(STDOUT_FILENO, "Salida write() desde C\n", 23);   /* STDOUT_FILENO = 1 */

/* ahora pinta usando la funcion de C en unistd.h que llama al sistema de forma genérica*/
res=syscall(SYS_write, STDOUT_FILENO, "Salida syscall() desde C\n", 25);  // SYS_write = 4

#if defined( __x86_64__ ) || defined( __i386__ )  // Intel x86 & x64
// usa ensamblador para hacer la llamada al sistema con la interrupción 80 hex
asm("mov $4  , %%eax \n"
    "mov $1  , %%ebx \n" 
    "mov %k0 , %%ecx \n" 
    "mov $35 , %%edx \n"
    "int $0x80       \n" 
 	: // sin salida
	: "r" (mensaje1)  // variables de entrada
	: "%eax", "%ebx", "%ecx", "%edx"   // registros usados para que el compilador lo sepa 
   );

#elif defined( __arm__ )  // ARM
// usa ensamblador para hacer la llamada al sistema con la interrupción  swi/svc 
asm( "mov r7 , #4  \n"   
     "mov r0 , #1  \n"   
     "mov r1 , %0  \n"   
     "mov r2 , #35 \n"   
     "svc 0        \n"
         : // sin salida
         : "r" (mensaje2)
         : "r0", "r1", "r2", "r7"
   );

#else
printf("Arquitectura ISA no soportada\n"); 

#endif

exit(0);

}
