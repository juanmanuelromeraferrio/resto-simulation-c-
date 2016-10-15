/*
 * Waiter.h
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#ifndef WAITER_H_
#define WAITER_H_

#include "../utils/Semaphore.h"
#include "../utils/LockFile.h"
#include "../types/types.h"
#include "../utils/SharedMemory.h"

#include <strings.h>
#include "../utils/Fifo.h"

class Waiter {
private:

	SharedMemory<restaurant_t> sharedMemory;
	Semaphore* memorySemaphore;

	Fifo* ordersFifo;
	LockFile* ordersLock;

	order_t searchOrder();
	bool requestOrder(order_t order);
	void chargeOrder(order_t order);
	void deliverOrder(order_t order);

public:
	Waiter();
	virtual ~Waiter();
	void run();
};

#endif /* WAITER_H_ */
