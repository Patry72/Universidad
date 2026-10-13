#include <stdio.h>

int main() 
{
	FILE *fp;
	int data[10] = {0,1,2,3,4,5,6,7,8,9};
	int n;
	
	fp = fopen("mifichero.txt", "r");
	
	for(n=0;n<10;n++){
		fscanf(fp, "%d", &data[n]);
		printf("%d", data[n]);
	}
}
