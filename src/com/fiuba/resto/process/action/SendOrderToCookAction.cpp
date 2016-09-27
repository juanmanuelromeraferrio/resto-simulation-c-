/*
 * SendOrderToCookAction.cpp
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#include "SendOrderToCookAction.h"

#include <cstdlib>

#include "../../logger/Logger.h"
#include "../../logger/Strings.h"
#include "../../utils/Constant.h"

using namespace std;

SendOrderToCookAction::SendOrderToCookAction() {
	this->ordersToCookFifo = new Fifo(ORDERS_TO_COOK);
}

SendOrderToCookAction::~SendOrderToCookAction() {
	this->ordersToCookFifo->cerrar();
	delete (this->ordersToCookFifo);
}

void SendOrderToCookAction::send(order_t order) {
	Logger::getInstance()->insert(KEY_ORDER_TO_COOK_ACTION,
	STRINGS_SEND_ORDER_TO_COOK, order.pid);

	ordersToCookFifo->_write((char *) &order, sizeof(order_t));

	exit(0);
}

