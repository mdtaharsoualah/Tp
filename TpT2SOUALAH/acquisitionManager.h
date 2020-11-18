#ifndef ACQUISITION_MANAGER_H
#define ACQUISITION_MANAGER_H

/**
* Initializes the acquisitions
*/
unsigned int acquisitionManagerInit(void);

/**
* Read Message
*/

MSG_BLOCK ReadMessage();

/**
* Waits that acquisitions terminate
*/
void acquisitionManagerJoin(void);

#endif