/*
 * AddDinerToLivingLineAction.cpp
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#include "AddDinerToLivingLineAction.h"

#include <unistd.h>
#include <csignal>
#include <exception>
#include <sstream>

#include "../../logger/Logger.h"
#include "../../logger/Strings.h"
#include "../../utils/Constant.h"

using namespace std;

AddDinerToLivingLineAction::AddDinerToLivingLineAction() {

	sharedMemory.create(FILE_RESTAURANT, KEY_MEMORY);
	this->dinersInLivingFifo = new Fifo(DINER_IN_LIVING);

	this->memorySemaphore = new Semaphore(FILE_RESTAURANT,
	KEY_MEMORY, 1);
}

AddDinerToLivingLineAction::~AddDinerToLivingLineAction() {
	sharedMemory.free();

	this->dinersInLivingFifo->cerrar();
	delete (this->dinersInLivingFifo);
}

void AddDinerToLivingLineAction::run(unsigned int dinerPid) {
	Logger::getInstance()->insert(KEY_DINER_TO_LIVING_ACTION,
	STRINGS_ADD_DINER_TO_LIVING, dinerPid);

	this->memorySemaphore->wait();

	restaurant_t restaurant = this->sharedMemory.read();
	restaurant.dinersInLiving++;
	this->sharedMemory.write(restaurant);

	this->memorySemaphore->signal();

	dinersInLivingFifo->_write((char *) &dinerPid, sizeof(unsigned long));

	exit(0);

}

