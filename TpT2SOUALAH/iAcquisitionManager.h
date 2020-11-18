#ifndef I_ACQUISITION_MANAGER_H
#define I_ACQUISITION_MANAGER_H

#include "msg.h"

//TODO create accessors prototype here.

int ReadAcquisMessage(MSG_BLOCK*);
unsigned int getProducerCount(void);

#endif