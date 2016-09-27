/*
 * CheckDinerInLivingAction.cpp
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#include "CheckDinerInLivingAction.h"

#include <cstdlib>
#include <exception>
#include <sstream>

#include "../../logger/Logger.h"
#include "../../logger/Strings.h"
#include "../../utils/Constant.h"
#include "../../utils/LockFile.h"

using namespace std;

CheckDinerInLivingAction::CheckDinerInLivingAction() {

	sharedMemory.create(FILE_RESTAURANT, KEY_MEMORY);
	this->dinersInLivingFifo = new Fifo(DINER_IN_LIVING);

	this->memorySemaphore = new Semaphore(FILE_RESTAURANT,
	KEY_MEMORY, 1);
	this->dinersInLivingLock = new LockFile(DINER_IN_LIVING_LOCK);
}

CheckDinerInLivingAction::~CheckDinerInLivingAction() {
	sharedMemory.free();

	this->dinersInLivingFifo->cerrar();
	delete (this->dinersInLivingFifo);
}

void CheckDinerInLivingAction::run() {
	dinersInLivingLock->lock();

	bool isEmpty = livingIsEmpty();

	if (!isEmpty) {
		try {
			moveDinerToLiving();
		} catch (exception& e) {
			dinersInLivingLock->unlock();
			throw e;
		}
	} else {
		updateTables();
	}

	dinersInLivingLock->unlock();

	exit(0);
}

bool CheckDinerInLivingAction::livingIsEmpty() {
	Logger::getInstance()->insert(KEY_DINER_IN_LIVING_ACTION,
	STRINGS_CHECK_DINER_IN_LIVING);

	this->memorySemaphore->wait();
	restaurant_t restaurant = this->sharedMemory.read();
	this->memorySemaphore->signal();

	if (restaurant.dinersInLiving > 0) {
		return false;
	}

	return true;

}

void CheckDinerInLivingAction::moveDinerToLiving() {

	unsigned int dinerPid;

	int result = dinersInLivingFifo->_read((char*) (&dinerPid),
			sizeof(unsigned long));

	if (result == -1) {
		exception e;
		throw e;
	}

	// Actualizo Lista de Comensales
	memorySemaphore->wait();
	restaurant_t restaurant = sharedMemory.read();
	restaurant.dinersInLiving--;
	sharedMemory.write(restaurant);
	memorySemaphore->signal();

	//Enivo Mensaje a Comensal
	Logger::getInstance()->insert(KEY_DINER_IN_LIVING_ACTION,
	STRINGS_ASSIGN_TABLE, dinerPid);

	stringstream ssDinerFifoName;
	ssDinerFifoName << DINERS_FIFO << dinerPid;

	char response = 1;
	Fifo* dinerFifo = new Fifo(ssDinerFifoName.str());
	dinerFifo->_write(&response, sizeof(char));
	dinerFifo->cerrar();
	delete (dinerFifo);

}

void CheckDinerInLivingAction::updateTables() {

	this->memorySemaphore->wait();
	restaurant_t restaurant = this->sharedMemory.read();
	restaurant.busyTables--;

	Logger::getInstance()->insert(KEY_DINER_IN_LIVING_ACTION,
	STRINGS_UPDATE_TABLE, restaurant.busyTables);

	this->sharedMemory.write(restaurant);
	this->memorySemaphore->signal();

}

