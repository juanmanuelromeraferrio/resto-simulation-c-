/*
 * Cook.h
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#ifndef COOK_H_
#define COOK_H_

#include "../utils/Semaphore.h"
#include "../utils/LockFile.h"
#include "../utils/SharedMemory.h"
#include "../types/types.h"

#include <strings.h>
#include "../utils/Fifo.h"

class Cook {
private:

	Fifo* ordersFifo;
	Fifo* ordersToCookFifo;

	order_t searchOrder();
	void cookOrder(order_t order);
	void sendOrder(order_t order);

public:
	Cook();
	virtual ~Cook();
	void run();
};

#endif /* COOK_H_ */
