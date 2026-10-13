/*
 * File: mapio.c
 * Author: Sergio Romero Montiel
 * Date: 2018-04-23
 * 
 * gcc -Wall -c mapio.c
 * ar rvs libmapio.a mapio.o
 */
#include <stdio.h>		/* perror, (printf on DEBUG) */
#include <stdlib.h>		/* exit */
#include <fcntl.h>		/* open */
#include <sys/mman.h>	/* mmap */
#include <unistd.h>		/* sysconf, geteuid*/

#define DEBUG

void* mmap_io(unsigned int physical)
{
	int fd;
	unsigned int *vaddr;
	long sz = sysconf(_SC_PAGE_SIZE);
	
#ifdef DEBUG
		printf("Page size: %ld (0x%lX)\n",sz,sz);
#endif	
	if (geteuid())
	{
		fprintf(stderr,"Should be root. Use sudo\n");
		exit(-1);
	}

	if ((fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0)
	{
		perror("open(\"/dev/mem\")");
		exit(-1);
	}
#ifdef DEBUG
		printf("Trying to map physical 0x%X\n",physical);
#endif
	vaddr = mmap(
		(void *)physical,		/* Try vaddr == physical			*/
		sz,						/* Mapping 1 page					*/
		PROT_WRITE|PROT_READ,	/* Read/Write in mapped page/s		*/
		MAP_SHARED,				/* Allow process sharing			*/
		fd,						/* File to map (/dev/mem)			*/
		physical				/* physical address we wish to map	*/
		);
		
	if (vaddr == MAP_FAILED)	/* Second try */
	{
#ifdef DEBUG
		printf("Can't set vaddr == physical\n");
#endif
		vaddr = mmap(NULL,		/* Let mmap choose virtual address	*/
			sz,	PROT_WRITE|PROT_READ, MAP_SHARED, fd, physical);
	} 
	
	close(fd); //No need to keep mem_fd open after mmap
 
	if (vaddr == MAP_FAILED)
	{
		perror("mmap");
		exit(-1);
	} 
#ifdef DEBUG
		printf("Virtual space used: 0x%X\n",(unsigned int)vaddr);
#endif

	return vaddr;
} /* mmap_io */
