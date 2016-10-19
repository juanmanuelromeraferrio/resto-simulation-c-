/*
 * Diner.h
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#ifndef DINER_H_
#define DINER_H_

#include "../utils/Semaphore.h"
#include "../utils/Fifo.h"
#include "../utils/SharedMemory.h"
#include "../types/types.h"


#include <strings.h>

class Diner {
private:
	Fifo* dinerInDoorFifo;
	Fifo* dinerFifo;
	Fifo* ordersFifo;

	SharedMemory<restaurant_t> sharedMemory;
	Semaphore* memorySemaphore;
	Semaphore* tablesSemaphore;
	unsigned int toPay;

	void enterToRestaurant();
	bool waitToSeat();
	void order();
	void waitOrder();
	void eat();
	void pay();
	void leaveRestaurant();
	int repeatOrder() {
		return rand() % 3 + 1;
	}
	unsigned int menuPrice();

public:
	Diner();
	virtual ~Diner();
	void run();
};

#endif /* DINER_H_ */
