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

#include "../logger/Logger.h"
#include "../logger/Strings.h"
#include "../types/types.h"
#include "../utils/Constant.h"
#include "../process/action/CheckDinerInLivingAction.h"

using namespace std;

Diner::Diner() {
	std::stringstream ssDinerFifoName;
	ssDinerFifoName << DINERS_FIFO << getpid();

	this->dinerFifo = new Fifo(ssDinerFifoName.str());
	this->dinerInDoorFifo = new Fifo(DINER_IN_DOOR);
	this->ordersFifo = new Fifo(ORDERS);

}

Diner::~Diner() {
	delete dinerInDoorFifo;
	delete ordersFifo;

	dinerFifo->cerrar();
	dinerFifo->_destroy();
	delete dinerFifo;
}

void Diner::run() {
	enterToRestaurant();
	waitToSeat();
	order();
	waitOrder();
	eat();
	leaveRestaurant();
}

void Diner::enterToRestaurant() {
	unsigned long pid = getpid();
	Logger::getInstance()->insert(KEY_DINER, STRINGS_ENTER_RESTO);
	dinerInDoorFifo->_write((char *) &pid, sizeof(unsigned long));
}

void Diner::waitToSeat() {
	Logger::getInstance()->insert(KEY_DINER, STRINGS_WAITING_FOR_A_TABLE);

	char wait;
	dinerFifo->_read((char*) &wait, sizeof(char));

	Logger::getInstance()->insert(KEY_DINER, STRINGS_SEAT);
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

void Diner::leaveRestaurant() {
	Logger::getInstance()->insert(KEY_DINER, STRINGS_LEAVING);

	__pid_t id = fork();

	if (id == 0) {
		CheckDinerInLivingAction action;
		action.run();
	} else {
		exit(0);
	}

}

