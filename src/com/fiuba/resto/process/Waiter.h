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
#include "../utils/SharedMemory.h"
#include "../types/types.h"

#include <strings.h>
#include "../utils/Fifo.h"

class Waiter {
private:

	Fifo* ordersFifo;

	LockFile* ordersLock;

	order_t searchOrder();
	void requestOrder(order_t order);
	void deliverOrder(order_t order);

public:
	Waiter();
	virtual ~Waiter();
	void run();
};

#endif /* WAITER_H_ */
