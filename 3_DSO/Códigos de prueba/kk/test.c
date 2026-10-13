#include <stdio.h>

int main() 
{
	FILE *fp;
	char data[1000];
	int n;
	
	fp = fopen("mifichero.txt", "r");
	do {
		n = fscanf(fp, "%S\n", data);
		printf("%s\n", data);
	} while(n);
	
}
