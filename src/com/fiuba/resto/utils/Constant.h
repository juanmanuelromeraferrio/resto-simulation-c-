/*
 * Constant.h
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#ifndef CONSTANTES_H_
#define CONSTANTES_H_

#include "../parser/Parser.h"

//FIFOS
#define	DINER_IN_DOOR	"/tmp/resto/diner_in_door"
#define	DINER_IN_LIVING "/tmp/resto/diner_in_living"
#define ORDERS  "/tmp/resto/orders"
#define ORDERS_TO_COOK "/tmp/resto/orders_to_cook"
#define	DINERS_FIFO	"/tmp/resto/diner_fifo_"

//LOCK
#define DINER_IN_DOOR_LOCK "/tmp/resto/diner_in_door_lock"
#define ORDERS_LOCK "/tmp/resto/orders_lock"
#define DINER_IN_LIVING_LOCK "/tmp/resto/diner_in_living_lock"

//SEMAPHORES
#define FILE_RESTAURANT "/tmp/resto/restaurant.tmp"
#define KEY_DINER_IN_DOOR 'd'
#define KEY_DINER_IN_LIVING 'l'
#define KEY_MEMORY 'm'
#define KEY_TABLES 't'

#define MOVE_TO_TABLE_TIME 0
#define MOVE_TO_LIVING_TIME 0

#define THINK_ORDER_TIME 0
#define EAT_TIME 2

#define TAKE_ORDER_TIME 0
#define DELIVER_ORDER_TIME 0

#define COOK_ORDER_TIME 2


//CONFIGURATION
const int HOSTS = Parser::getInstance()->getIntValue("hosts");
const int WAITERS = Parser::getInstance()->getIntValue("waiters");
const int TABLES = Parser::getInstance()->getIntValue("tables");
const int DINERS_TOTAL = Parser::getInstance()->getIntValue("diners_total");

#define FILE_CONFIG "src/com/fiuba/resto/config/config.json"

#endif /* CONSTANTES_H_ */
