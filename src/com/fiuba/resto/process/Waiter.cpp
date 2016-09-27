/*
 * Waiter.cpp
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#include "Waiter.h"

#include <unistd.h>
#include <csignal>
#include <exception>
#include <sstream>

#include "../logger/Logger.h"
#include "../logger/Strings.h"
#include "../utils/Constant.h"
#include "../utils/signals/SignalHandler.h"
#include "../utils/signals/SIGINT_Handler.h"
#include "../process/action/SendOrderToCookAction.h"

using namespace std;

Waiter::Waiter() {
	this->ordersFifo = new Fifo(ORDERS);
	this->ordersLock = new LockFile(ORDERS_LOCK);
}

Waiter::~Waiter() {
	delete ordersFifo;
}

void Waiter::run() {

	SIGINT_Handler sigint_handler;
	SignalHandler::getInstance()->registerHandler(SIGINT, &sigint_handler);

	while (sigint_handler.getGracefulQuit() == 0) {

		try {
			order_t order = searchOrder();
			if (order.type == 'd') {
				requestOrder(order);
			} else {
				deliverOrder(order);
			}
		} catch (exception& e) {
			throw e;
		}

		sleep(10);
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

void Waiter::requestOrder(order_t order) {

	Logger::getInstance()->insert(KEY_WAITER, STRINGS_TAKE_ORDER, order.pid);
	sleep(TAKE_ORDER_TIME);

	__pid_t id = fork();

	if (id == 0) {
		SendOrderToCookAction action;
		action.send(order);
	}
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

	dinerFifo->cerrar();
	delete (dinerFifo);
}

