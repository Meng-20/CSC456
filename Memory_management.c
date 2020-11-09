#include <stdio.h>
#include <stdlib.h>
/**
**Author: Mengling Ding
**Date:11/8/2020
**to have hands©\on experience of building a paging-based memory management system
** 
*/ 

int freeFrameList[];
int processList[];

void memoryManager(int memSize, int frameSize);
int allocate(int allocSize, int pid);


int main()
{
    memoryManager(int memSize, int frameSize);
	printf("Hello world!\n");
    return 0;
}


void memoryManager(int memSize, int frameSize){
	frameSize = 1;
	freeFrameList[memSize];
}

int allocate(int allocSize, int pid){
	allocSize = 1;
	
	
}
