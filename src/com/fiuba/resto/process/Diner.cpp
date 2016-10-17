/*
 * Diner.cpp
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#include "Diner.h"

#include <unistd.h>
#include <sstream>
#include <cstdlib>
#include <signal.h>

#include "../logger/Logger.h"
#include "../logger/Strings.h"
#include "../types/types.h"
#include "../utils/Constant.h"

using namespace std;

Diner::Diner() {
	std::stringstream ssDinerFifoName;
	ssDinerFifoName << DINERS_FIFO << getpid();

	this->dinerFifo = new Fifo(ssDinerFifoName.str());
	this->dinerInDoorFifo = new Fifo(DINER_IN_DOOR);
	this->ordersFifo = new Fifo(ORDERS);

	sharedMemory.create(FILE_RESTAURANT, KEY_MEMORY);

	this->memorySemaphore = new Semaphore(FILE_RESTAURANT,
	KEY_MEMORY);

	this->tablesSemaphore = new Semaphore(FILE_RESTAURANT,
	KEY_TABLES);

}

Diner::~Diner() {
	dinerFifo->cerrar();
	dinerFifo->_destroy();
	dinerInDoorFifo->cerrar();
	ordersFifo->cerrar();
	sharedMemory.free();
}

void Diner::run() {

	enterToRestaurant();

	bool hasPlace = waitToSeat();

	if (hasPlace) {
		order();
		waitOrder();
		eat();
		pay();
		leaveRestaurant();
	}
}

void Diner::enterToRestaurant() {
	unsigned long pid = getpid();
	Logger::getInstance()->insert(KEY_DINER, STRINGS_ENTER_RESTO);
	dinerInDoorFifo->_write((char *) &pid, sizeof(unsigned long));
}

bool Diner::waitToSeat() {
	Logger::getInstance()->insert(KEY_DINER, STRINGS_WAITING_FOR_A_TABLE);

	char wait;
	dinerFifo->_read((char*) &wait, sizeof(char));

	if (wait == 1) {
		Logger::getInstance()->insert(KEY_DINER, STRINGS_SEAT);
		return true;
	} else {
		return false;
	}
}

void Diner::order() {

	sleep(THINK_ORDER_TIME);

	unsigned long pid = getpid();
	Logger::getInstance()->insert(KEY_DINER, STRINGS_WAITING_TO_ORDER);

	order_t order;
	order.pid = pid;
	order.type = 'd';

	ordersFifo->_write((char *) &order, sizeof(order_t));
}

void Diner::waitOrder() {
	Logger::getInstance()->insert(KEY_DINER, STRINGS_WAITING_ORDER);

	char wait;
	dinerFifo->_read((char*) &wait, sizeof(char));
}

void Diner::eat() {
	Logger::getInstance()->insert(KEY_DINER, STRINGS_EATING);
	sleep(EAT_TIME);
}

void Diner::pay() {

	unsigned long pid = getpid();
	Logger::getInstance()->insert(KEY_DINER, STRINGS_WAITING_TO_PAY);

	order_t order;
	order.pid = pid;
	order.type = 'p';

	ordersFifo->_write((char *) &order, sizeof(order_t));
}

void Diner::leaveRestaurant() {
	Logger::getInstance()->insert(KEY_DINER, STRINGS_LEAVING);

	this->memorySemaphore->wait();
	restaurant_t restaurant = this->sharedMemory.read();

	restaurant.dinersInRestaurant--;

	if (restaurant.dinersInLiving > 0) {
		tablesSemaphore->signal();
	} else {
		restaurant.busyTables--;
		Logger::getInstance()->insert(KEY_DINER,
		STRINGS_UPDATE_TABLE, restaurant.busyTables);

		if (restaurant.diners == DINERS_TOTAL && restaurant.dinersInRestaurant == 0) {
			Logger::getInstance()->insert(KEY_DINER,
			STRINGS_LAST_DINER);
			kill(restaurant.main_pid, SIGQUIT);
		}
	}

	this->sharedMemory.write(restaurant);

	this->memorySemaphore->signal();

}

