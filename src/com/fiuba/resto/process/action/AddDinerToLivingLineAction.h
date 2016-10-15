/*
 * AddDinerToLivingLineAction.h
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#ifndef ADD_DINER_TO_LIVING_ACTION_H_
#define ADD_DINER_TO_LIVING_ACTION_H_

#include "../../utils/SharedMemory.h"
#include "../../utils/Semaphore.h"
#include "../../utils/Fifo.h"
#include "../../types/types.h"

#include <strings.h>

class AddDinerToLivingLineAction {
private:

	SharedMemory<restaurant_t> sharedMemory;
	Fifo* dinersInLivingFifo;

	Semaphore* memorySemaphore;

public:
	AddDinerToLivingLineAction();
	virtual ~AddDinerToLivingLineAction();
	void run(unsigned long dinerPid);
};

#endif /* ADD_DINER_TO_LIVING_ACTION_H_ */
