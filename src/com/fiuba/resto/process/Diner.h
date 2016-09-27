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


#include <strings.h>

class Diner {
private:
	Fifo* dinerInDoorFifo;
	Fifo* dinerFifo;

	Fifo* ordersFifo;

	void enterToRestaurant();
	void waitToSeat();
	void order();
	void waitOrder();
	void eat();
	void leaveRestaurant();

public:
	Diner();
	virtual ~Diner();
	void run();
};

#endif /* DINER_H_ */
