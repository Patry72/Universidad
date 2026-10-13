#include <stdio.h>

int main() 
{
	FILE *fp;
	int data[10] = {0,1,2,3,4,5,6,7,8,9};
	int n;
	
	fp = fopen("mifichero.txt", "r");
	
	fwrite(data, sizeof(int), 10, fp);
	
	fclose(fp);
}
