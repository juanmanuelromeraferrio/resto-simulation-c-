/*
 * SendOrderToCookAction.h
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#ifndef SEND_ORDER_TO_COOK_ACTION_H_
#define SEND_ORDER_TO_COOK_ACTION_H_

#include "../../utils/Fifo.h"
#include "../../types/types.h"

#include <strings.h>

class SendOrderToCookAction {
private:
	Fifo* ordersToCookFifo;

public:
	SendOrderToCookAction();
	virtual ~SendOrderToCookAction();
	void send(order_t order);
};

#endif /* SEND_ORDER_TO_COOK_ACTION_H_ */
