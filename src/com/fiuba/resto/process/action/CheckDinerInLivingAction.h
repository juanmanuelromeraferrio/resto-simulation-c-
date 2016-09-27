/*
 * CheckDinerInLivingAction.h
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#ifndef CHECK_DINER_IN_LIVING_ACTION_H_
#define CHECK_DINER_IN_LIVING_ACTION_H_

#include "../../utils/SharedMemory.h"
#include "../../utils/Semaphore.h"
#include "../../utils/Fifo.h"
#include "../../utils/LockFile.h"
#include "../../types/types.h"

#include <strings.h>

class CheckDinerInLivingAction {
private:

	SharedMemory<restaurant_t> sharedMemory;
	Fifo* dinersInLivingFifo;

	Semaphore* memorySemaphore;
	LockFile* dinersInLivingLock;

	bool livingIsEmpty();
	void moveDinerToLiving();
	void updateTables();

public:
	CheckDinerInLivingAction();
	virtual ~CheckDinerInLivingAction();
	void run();
};

#endif /* CHECK_DINER_IN_LIVING_ACTION_H_ */
