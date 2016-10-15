/*
 * Cook.cpp
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#include "Cook.h"

#include <unistd.h>
#include <csignal>
#include <exception>

#include "../logger/Logger.h"
#include "../logger/Strings.h"
#include "../utils/Constant.h"
#include "../utils/signals/SignalHandler.h"
#include "../utils/signals/SIGINT_Handler.h"

using namespace std;

Cook::Cook() {
	this->ordersFifo = new Fifo(ORDERS);
	this->ordersToCookFifo = new Fifo(ORDERS_TO_COOK);

}

Cook::~Cook() {
	delete ordersFifo;
	delete ordersToCookFifo;
}

void Cook::run() {

	while (true) {

		try {
			order_t order = searchOrder();
			cookOrder(order);
			sendOrder(order);
		} catch (exception& e) {
			throw e;
		}
	}
}

order_t Cook::searchOrder() {

	order_t order;
	int result = ordersToCookFifo->_read((char*) (&order), sizeof(order_t));

	if (result == -1) {
		exception e;
		throw e;
	}

	return order;
}

void Cook::cookOrder(order_t order) {
	Logger::getInstance()->insert(KEY_COOK, STRINGS_COOKING_ORDER, order.pid);
	sleep(COOK_ORDER_TIME);
}

void Cook::sendOrder(order_t order) {
	Logger::getInstance()->insert(KEY_COOK, STRINGS_FINISH_ORDER, order.pid);
	order.type = 'c';

	ordersFifo->_write((char *) &order, sizeof(order_t));
}

