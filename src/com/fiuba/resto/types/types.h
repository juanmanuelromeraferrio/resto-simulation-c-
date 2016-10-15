/*
 * types.h
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#ifndef TYPES_H_
#define TYPES_H_

struct restaurant_t {
	unsigned int tables;
	unsigned int busyTables;
	unsigned int dinersInLiving;
	unsigned int cash;

};

struct order_t{
	char type;
	pid_t pid;
};

#endif /* TYPES_H_ */
