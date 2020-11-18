#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <linux/unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include "acquisitionManager.h"
#include "msg.h"
#include "iSensor.h"
#include "mySoftware.h"
#include "iAcquisitionManager.h"
#include "debug.h"
#include <sys/types.h>
#include <sys/syscall.h>


//producer count storage
volatile unsigned int produceCount = 0;

pthread_t producers[4];

MSG_BLOCK Buffer[256]={0};

static void *produce(void *params);
int BufferIdWrite=0;
int BufferIdRead=0;

/**
* Semaphores and Mutex
*/

sem_t bufferLibreSemaphores;
sem_t bufferPrisSemaphores;

pthread_mutex_t produceCountMutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bufferMutex=PTHREAD_MUTEX_INITIALIZER;


//TODO

/*
* Creates the synchronization elements.
* @return ERROR_SUCCESS if the init is ok, ERROR_INIT otherwise
*/
static unsigned int createSynchronizationObjects(void);

/*
* Increments the produce count.
*/
static void incrementProducerCount(void);

static unsigned int createSynchronizationObjects(void)
{
	//Initialisation Semaphores
	if(sem_init(&bufferLibreSemaphores,0,0) != -1){
		if(sem_init(&bufferPrisSemaphores,0,256) != -1){
			printf("[acquisitionManager]Semaphore created\n");
			return ERROR_SUCCESS;
		}
	}
	return ERROR_INIT;
	
}

static void incrementProducerCount(void)
{
	pthread_mutex_lock(&produceCountMutex);
	produceCount++;
	pthread_mutex_unlock(&produceCountMutex);
}

unsigned int getProducerCount(void)
{
	unsigned int p = 0;
	pthread_mutex_lock(&produceCountMutex);
	p=produceCount;
	pthread_mutex_unlock(&produceCountMutex);
	return p;
}

//TODO create accessors to limit semaphore and mutex usage outside of this C module.

int BufferWriteId(){
	int p = 0;
	sem_wait(&bufferPrisSemaphores);
	sem_post(&bufferLibreSemaphores);
	pthread_mutex_lock(&produceCountMutex);
		p=BufferIdWrite;
		BufferIdWrite=(BufferIdWrite==255) ? 0 : BufferIdWrite+1;
	pthread_mutex_unlock(&produceCountMutex);
	return p;
}

int BufferReadId(){
	int p = 0;
	sem_wait(&bufferLibreSemaphores);
	sem_post(&bufferPrisSemaphores);
	pthread_mutex_lock(&produceCountMutex);
		p=BufferIdRead;
		BufferIdRead=(BufferIdRead==255) ? 0 : BufferIdRead+1;
	pthread_mutex_unlock(&produceCountMutex);
	return p;
}

void ReadMessage(MSG_BLOCK* msg){
	int BufId=-1;
	BufId=BufferReadId();
	if(BufId!=-1){
		printf("[acquisitionManager] Message %d lue\n", BufId);
		*msg=Buffer[BufId];
	}
}

unsigned int acquisitionManagerInit(void)
{
	unsigned int i;
	printf("[acquisitionManager]Synchronization initialization in progress...\n");
	fflush( stdout );
	if (createSynchronizationObjects() == ERROR_INIT)
		return ERROR_INIT;
	
	printf("[acquisitionManager]Synchronization initialization done.\n");

	for (i = 0; i < PRODUCER_COUNT; i++)
	{
		//Start the 4 thread
		pthread_create(&producers[i],NULL,&produce,i);
	}

	return ERROR_SUCCESS;
}

void acquisitionManagerJoin(void)
{
	unsigned int i;
	for (i = 0; i < PRODUCER_COUNT; i++)
	{
		//join all the 4 thread
		pthread_join(producers[i],NULL);
	}

	//TODO
	sem_destroy(&bufferLibreSemaphores);
	sem_destroy(&bufferPrisSemaphores);
	printf("[acquisitionManager]Semaphore cleaned\n");
}

void *produce(void* params)
{
	int BufId=-1;
	MSG_BLOCK tmpMsg;
	unsigned int produceId = syscall(SYS_gettid);
	printf("[acquisitionManager]Producer created with id %d \n", produceId);
	unsigned int i = 0;
	while (i < PRODUCER_LOOP_LIMIT)
	{
		i++;
		sleep(PRODUCER_SLEEP_TIME+(rand() % 5));
		//recuperer la valeur
		//Demander un id au Buffer
		BufId=BufferWriteId();
		if(BufId!=-1){
			getInput(1,&tmpMsg);
			if(messageCheck(&tmpMsg)){
				Buffer[BufId]=tmpMsg;
				printf("[acquisitionManager] %d a recu un message stocké a %d\n", produceId, BufId);
			}
		}
	}
	
	printf("[acquisitionManager] %d termination\n", produceId);
	//TODO
}
