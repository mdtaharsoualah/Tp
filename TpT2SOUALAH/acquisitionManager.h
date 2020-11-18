#ifndef ACQUISITION_MANAGER_H
#define ACQUISITION_MANAGER_H
#include "msg.h"
/**
* Initializes the acquisitions
*/
unsigned int acquisitionManagerInit(void);

/**
* Read Message
*/

void ReadMessage(MSG_BLOCK* msg);
unsigned int getProducerCount(void);

/**
* Waits that acquisitions terminate
*/
void acquisitionManagerJoin(void);

#endif