#include "iDisplay.h"
#include <stdlib.h>
#include <stdio.h>
#include "debug.h"

void messageDisplay(volatile MSG_BLOCK* mBlock){
	unsigned int i; 
	messageCheck(mBlock);
	printf("Message\n");
	printf("[");
	for(i=0;i < DATA_SIZE;i++)
		printf("%u ",mBlock->mData[i]);
	printf("]\n");
}