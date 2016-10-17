/*
 * types.h
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#ifndef TYPES_H_
#define TYPES_H_

struct restaurant_t {
	unsigned long main_pid;
	unsigned int tables;
	unsigned int busyTables;
	unsigned int dinersInLiving;
	unsigned int cash;
	unsigned int diners;
	unsigned int dinersInRestaurant;

};

struct order_t {
	char type;
	pid_t pid;
};

#endif /* TYPES_H_ */
