#include <stdio.h>
#include <string.h>

int main (void)
{
	char buffer1[10] = "";
	
	printf("Buffer1 al inicio: %s\n", buffer1);
	
	strcat(buffer1, "1");
	
	printf("Buffer1 al concatenar: %s\n", buffer1);
	
	buffer1[1] = 'a';
	
	printf("Buffer1 al asignar una letra a posición: %s\n", buffer1);
}
