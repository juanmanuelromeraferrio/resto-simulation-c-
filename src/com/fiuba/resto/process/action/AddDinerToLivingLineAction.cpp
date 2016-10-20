/*
 * AddDinerToLivingLineAction.cpp
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#include "AddDinerToLivingLineAction.h"

#include "../../logger/Logger.h"
#include "../../logger/Strings.h"
#include "../../utils/Constant.h"

using namespace std;

AddDinerToLivingLineAction::AddDinerToLivingLineAction() {

	sharedMemory.create(FILE_RESTAURANT, KEY_MEMORY);
	this->dinersInLivingFifo = new Fifo(DINER_IN_LIVING);

	this->memorySemaphore = new Semaphore(FILE_RESTAURANT,
	KEY_MEMORY);
}

AddDinerToLivingLineAction::~AddDinerToLivingLineAction() {
	sharedMemory.free();
	dinersInLivingFifo->cerrar();
}

void AddDinerToLivingLineAction::run(unsigned long dinerPid) {

	this->memorySemaphore->wait();

	restaurant_t restaurant = this->sharedMemory.read();
	restaurant.dinersInLiving++;
	this->sharedMemory.write(restaurant);

	this->memorySemaphore->signal();

	dinersInLivingFifo->_write((char *) &dinerPid, sizeof(unsigned long));
}

