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

MSG_BLOCK ReadMessage();
unsigned int getProducerCount(void);

/**
* Waits that acquisitions terminate
*/
void acquisitionManagerJoin(void);

#endif