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
	ordersToCookFifo->cerrar();
}

void SendOrderToCookAction::send(order_t order) {
	ordersToCookFifo->_write((char *) &order, sizeof(order_t));
}

