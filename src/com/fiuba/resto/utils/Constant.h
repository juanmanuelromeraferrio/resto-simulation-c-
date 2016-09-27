/*
 * Constantes.h
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#ifndef CONSTANTES_H_
#define CONSTANTES_H_

//FIFOS
#define	DINER_IN_DOOR	"/tmp/resto/diner_in_door"
#define	DINER_IN_LIVING "/tmp/resto/diner_in_living"
#define ORDERS  "/tmp/resto/orders"
#define ORDERS_TO_COOK "/tmp/resto/orders_to_cook"

//LOCK
#define DINER_IN_DOOR_LOCK "/tmp/resto/diner_in_door_lock"
#define ORDERS_LOCK "/tmp/resto/orders_lock"
#define DINER_IN_LIVING_LOCK "/tmp/resto/diner_in_living_lock"

//SEMAPHORES
#define	DINERS_FIFO	"/tmp/resto/diner_fifo_"

#define FILE_RESTAURANT "/tmp/resto/restaurant.tmp"
#define KEY_DINER_IN_DOOR 'd'
#define KEY_DINER_IN_LIVING 'l'
#define KEY_MEMORY 'm'

#define MOVE_TO_TABLE_TIME 0
#define MOVE_TO_LIVING_TIME 0

#define THINK_ORDER_TIME 0
#define EAT_TIME 5

#define TAKE_ORDER_TIME 0
#define DELIVER_ORDER_TIME 0

#define COOK_ORDER_TIME 2

#endif /* CONSTANTES_H_ */
