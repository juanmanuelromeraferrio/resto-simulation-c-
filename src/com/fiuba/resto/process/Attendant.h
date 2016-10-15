/*
 * Attendant.h
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#ifndef ATTENDANT_H_
#define ATTENDANT_H_

#include "../utils/Semaphore.h"
#include "../utils/SharedMemory.h"
#include "../types/types.h"

#include <strings.h>
#include "../utils/Fifo.h"

class Attendant {
private:

	Fifo* dinersInLivingFifo;
	Semaphore* freeTableSemaphore;

	SharedMemory<restaurant_t> sharedMemory;
	Semaphore* memorySemaphore;

	void asignTable();

public:
	Attendant();
	virtual ~Attendant();
	void run();
};

#endif /* ATTENDANT_H_ */
