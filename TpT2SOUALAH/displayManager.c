#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "displayManager.h"
#include "iDisplay.h"
#include "iAcquisitionManager.h"
#include "iMessageAdder.h"
#include "msg.h"
#include "mySoftware.h"
#include "debug.h"
#include <sys/types.h>
#include <sys/syscall.h>

// DisplayManager thread.
pthread_t displayThread;

/**
 * Display manager entry point.
 * */
static void *display( void *parameters );


void displayManagerInit(void){
	pthread_create(&displayThread,NULL,&display,NULL);
}

void displayManagerJoin(void){
	pthread_join(displayThread,NULL);
} 

static void *display( void *parameters )
{
	MSG_BLOCK tmpOut;
	unsigned int ConsumeCount=0, ProducerCount=0;
	unsigned int displayId = syscall(SYS_gettid);
	D(printf("[displayManager]Thread created for display with id %d\n", displayId));
	unsigned int diffCount = 0;
	int difFlag = 0;
	while(diffCount > 0 || !difFlag){
		sleep(DISPLAY_SLEEP_TIME);
		getSum(&tmpOut);
		ConsumeCount=getConsumeCount();
		ProducerCount= getProducerCount();
		diffCount=ProducerCount-ConsumeCount;
		difFlag=(diffCount==0) ? 1 : 0;
		messageDisplay(&tmpOut);
		printf("Messages recu : %4d  Messages somme : %4d Messages Restant : %4d \n", ProducerCount, ConsumeCount, diffCount);

		//TODO
	}
	printf("[displayManager] %d termination\n", displayId);
   //TODO
}