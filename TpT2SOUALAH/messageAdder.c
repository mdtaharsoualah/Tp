#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h> 
#include <unistd.h>
#include <pthread.h>
#include "messageAdder.h"
#include "msg.h"
#include "iMessageAdder.h"
#include "mySoftware.h"
#include "iAcquisitionManager.h"
#include "debug.h"
#include <sys/types.h>
#include <sys/syscall.h>

//consumer thread
pthread_t consumer;
//Message computed
volatile MSG_BLOCK out;
//Consumer count storage
volatile unsigned int consumeCount = 0;

pthread_mutex_t consumeCountMutex, SumMutex;

/**
 * Increments the consume count.
 */

static void incrementConsumeCount(void)
{
	pthread_mutex_lock(&consumeCountMutex);
	consumeCount++;
	pthread_mutex_unlock(&consumeCountMutex);
}

unsigned int getConsumeCount(void)
{
	unsigned int p = 0;
	pthread_mutex_lock(&consumeCountMutex);
	p=consumeCount;
	pthread_mutex_unlock(&consumeCountMutex);
	return p;
}

/**
 * Consumer entry point.
 */
static void *sum( void *parameters );

//TODO create accessors to limit semaphore and mutex usage outside of this C module.

static void CalculSum(MSG_BLOCK *msg)
{
	pthread_mutex_lock(&SumMutex);
	messageAdd(&out, msg);
	pthread_mutex_unlock(&SumMutex);
}

void getSum(MSG_BLOCK *msg)
{
	MSG_BLOCK p;
	pthread_mutex_lock(&SumMutex);
	*msg=out;
	pthread_mutex_unlock(&SumMutex);
}

void messageAdderInit(void){
	pthread_mutex_init(&consumeCountMutex,NULL);
	pthread_create(&consumer,NULL,&sum,NULL);
	out.checksum = 0;
	for (size_t i = 0; i < DATA_SIZE; i++)
	{
		out.mData[i] = 0;
	}
}

void messageAdderJoin(void){
	//TODO
	pthread_join(consumer,NULL);
	pthread_mutex_destroy(&consumeCountMutex);
}

static void *sum( void *parameters )
{
	MSG_BLOCK tmpMsg;
	unsigned int produceId = syscall(SYS_gettid);
	int iddd;
	D(printf("[messageAdder]Thread created for sum with id %d\n", produceId));
	unsigned int i = 0;
	while(i<ADDER_LOOP_LIMIT){
		i++;
		sleep(ADDER_SLEEP_TIME);
		//TODO
		iddd=ReadAcquisMessage(&tmpMsg);
		CalculSum(&tmpMsg);
		printf("[messageAdder] Message %d lue\n", iddd);
	}

	printf("[messageAdder] %d termination\n", produceId);
	//TODO
}


