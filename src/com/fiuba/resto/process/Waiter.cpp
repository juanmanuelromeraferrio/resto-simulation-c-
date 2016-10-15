/*
 * Waiter.cpp
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#include "Waiter.h"

#include <sys/types.h>
#include <unistd.h>
#include <exception>
#include <sstream>

#include "../logger/Logger.h"
#include "../logger/Strings.h"
#include "../utils/Constant.h"
#include "action/SendOrderToCookAction.h"

using namespace std;

Waiter::Waiter() {

	sharedMemory.create(FILE_RESTAURANT, KEY_MEMORY);
	this->memorySemaphore = new Semaphore(FILE_RESTAURANT,
	KEY_MEMORY);

	this->ordersFifo = new Fifo(ORDERS);
	this->ordersLock = new LockFile(ORDERS_LOCK);
}

Waiter::~Waiter() {
	sharedMemory.free();
}

void Waiter::run() {

	while (true) {

		try {
			order_t order = searchOrder();
			if (order.type == 'd') {
				bool isMainProcess = requestOrder(order);
				if (!isMainProcess) {
					return;
				}
			} else if (order.type == 'p') {
				chargeOrder(order);
			} else {
				deliverOrder(order);
			}
		} catch (exception& e) {
			throw e;
		}
	}
}

order_t Waiter::searchOrder() {

	order_t order;

	ordersLock->lock();

	int result = ordersFifo->_read((char*) (&order), sizeof(order_t));

	if (result == -1) {
		ordersLock->unlock();
		exception e;
		throw e;
	}

	ordersLock->unlock();

	return order;
}

bool Waiter::requestOrder(order_t order) {

	Logger::getInstance()->insert(KEY_WAITER, STRINGS_TAKE_ORDER, order.pid);
	sleep(TAKE_ORDER_TIME);

	__pid_t id = fork();

	if (id == 0) {
		SendOrderToCookAction action;
		action.send(order);
		return false;
	}

	return true;
}

void Waiter::chargeOrder(order_t order) {
	memorySemaphore->wait();
	restaurant_t restaurant = sharedMemory.read();
	restaurant.cash += 1;
	Logger::getInstance()->insert(KEY_WAITER, STRINGS_MONEY_IN_CASH,
			restaurant.cash);
	sharedMemory.write(restaurant);
	memorySemaphore->signal();
}

void Waiter::deliverOrder(order_t order) {

	Logger::getInstance()->insert(KEY_WAITER, STRINGS_DISPATCH_ORDER,
			order.pid);

	sleep(DELIVER_ORDER_TIME);

	stringstream ssDinerFifoName;
	ssDinerFifoName << DINERS_FIFO << order.pid;

	char response = 1;
	Fifo* dinerFifo = new Fifo(ssDinerFifoName.str());
	dinerFifo->_write(&response, sizeof(char));
}

