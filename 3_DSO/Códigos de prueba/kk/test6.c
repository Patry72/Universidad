// DA EEROR

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mmap.h>
#include <strings.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() 
{
	
	int no;
	int data[10] = {10,11,12,13,14,15,16,17,18,19};
	int n;
	int *p;
	
	no = open("mifichero.txt", 0664);
	
	ftruncate(no+1, sizeof(int)*10);
	
	p = mmap(NULL, sizeof(int)*10, PROT_SHARED, 0664, no, 0);
	
	bcopy(data, p, sizeof(int)*10);
	
	close(no);
}
