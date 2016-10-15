/*
 * Host.h
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#ifndef HOST_H_
#define HOST_H_

#include "../utils/Semaphore.h"
#include "../utils/LockFile.h"
#include "../utils/SharedMemory.h"
#include "../types/types.h"

#include <strings.h>
#include "../utils/Fifo.h"

class Host {
private:
	SharedMemory<restaurant_t> sharedMemory;

	Fifo* dinerInDoorFifo;
	Fifo* dinerInLivingFifo;

	LockFile* dinerInDoorLock;
	Semaphore* memorySemaphore;

	unsigned long searchDinerInDoor();
	bool existFreeTable();
	void moveDinerToTable(unsigned long dinerPid);
	bool moveDinerToLiving(unsigned long dinerPid);

public:
	Host();
	virtual ~Host();
	void run();
};

#endif /* HOST_H_ */
