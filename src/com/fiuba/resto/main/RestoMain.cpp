/*
 * RestoMain.cpp
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#include <getopt.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>

#include "../process/Diner.h"
#include "../process/Host.h"
#include "../process/Waiter.h"
#include "../process/Cook.h"
#include "../process/Attendant.h"
#include "../types/types.h"
#include "../utils/Constant.h"
#include "../logger/Logger.h"
#include "../logger/Strings.h"
#include "../utils/SharedMemory.h"
#include "../utils/LockFile.h"
#include "../utils/Fifo.h"

static const char *optString = "icd:q:sh?";

static const struct option longOpts[] = { { "init", no_argument, NULL, 'i' }, {
		"close", no_argument, NULL, 'c' }, { "diner", required_argument, NULL,
		'd' }, { "query", required_argument,
NULL, 'q' }, { "help", no_argument, NULL, 'h' }, { "simular",
no_argument, NULL, 's' }, { NULL, no_argument, NULL, 0 } };

void destroy(SharedMemory<restaurant_t>* sharedMemory);

bool initSharedMemory(SharedMemory<restaurant_t>* sharedMemory, bool exclusive);

bool wasSharedMemoryInit(SharedMemory<restaurant_t>* sharedMemory);

void initValues(SharedMemory<restaurant_t>* sharedMemory,
		restaurant_t *restaurant);

int main(int argc, char** argv) {

//	Manual manual;
//	manual.showInstructions();

	SharedMemory<restaurant_t> sharedMemory;
	restaurant_t restaurant;

	FILE* file = fopen(FILE_RESTAURANT, "w");
	fclose(file);

	int opt = 0;
	int longIndex;

	opt = getopt_long(argc, argv, optString, longOpts, &longIndex);

	while (opt != -1) {
		switch (opt) {
		case 'i':
			if (initSharedMemory(&sharedMemory, true)) {
				initValues(&sharedMemory, &restaurant);

				// Creo Hosts
				int hosts = HOSTS;
				int i = 0;
				__pid_t id = 0;
				for (; i < hosts; i++) {
					id = fork();
					if (id == 0) {
						break;
					}
				}

				if (id == 0) {
					Host host;
					host.run();
				} else {

					// Creo Waiters

					int waiters = WAITERS;
					int i = 0;
					__pid_t id = 0;
					for (; i < waiters; i++) {
						id = fork();
						if (id == 0) {
							break;
						}
					}
					if (id == 0) {
						Waiter waiter;
						waiter.run();
					} else {
						// Creo Cook
						id = fork();
						if (id == 0) {
							Cook cook;
							cook.run();

						} else {
							// Creo Attendant
							id = fork();
							if (id == 0) {
								Attendant attendant;
								attendant.run();
							} else {
								int childs = HOSTS + WAITERS + 1 + 1;
								for (int i = 0; i < childs; i++) {
									wait(NULL);
								}
							}

						}
					}
				}
			}

			break;

		case 'd':
			if (wasSharedMemoryInit(&sharedMemory)) {
				int diners = 0;

				if (optarg != NULL) {
					diners = atoi(optarg);
				}

				int i = 0;
				__pid_t id = 0;

				for (; i < diners; i++) {
					id = fork();
					if (id == 0) {
						break;
					}
				}

				if (id == 0) {
					Diner diner;
					diner.run();
				}

			} else {
				std::cout << "The system was not working" << std::endl;
			}
			break;

		case 'q':
			if (wasSharedMemoryInit(&sharedMemory)) {
				sharedMemory.read();
				switch (*optarg) {
				case 'c':
					Logger::getInstance()->insert(KEY_RESTO, STRINGS_CASH,
							restaurant.cash);
					break;
				}

				sharedMemory.free();

			} else {
				std::cout << "The system was not working" << std::endl;
			}
			break;
		}

		break;
	}

//	if (opt == 'i') {
//		std::cout << " Destroy shared memory " << std::endl;
//		destroy(&sharedMemory);
//	}

	unsigned long pid = getpid();
	Logger::getInstance()->insert(KEY_RESTO, STRINGS_EXIT, pid);
	return 0;
}

bool initSharedMemory(SharedMemory<restaurant_t>* sharedMemory,
		bool exclusive) {

	int state = sharedMemory->create(FILE_RESTAURANT, KEY_MEMORY, exclusive);
	bool result = false;

	switch (state) {
	case ERROR_FTOK:
		std::cout << "The File " << FILE_RESTAURANT << " is necessary"
				<< std::endl;
		break;
		std::cout << "The system has already been initiated" << std::endl;
		break;
	case SHM_OK:
		result = true;
		break;
	default:
		std::cout << "Unexpected error " << state << std::endl;
		break;
	}

	return result;
}

bool wasSharedMemoryInit(SharedMemory<restaurant_t>* sharedMemory) {
	int state = sharedMemory->create(FILE_RESTAURANT, KEY_MEMORY, true);
	if (state != SHM_OK) {
		return true;
	} else {
		sharedMemory->free();
		return false;
	}
}

void initValues(SharedMemory<restaurant_t>* sharedMemory,
		restaurant_t *restaurant) {

	restaurant->tables = TABLES;
	restaurant->busyTables = 0;
	restaurant->dinersInLiving = 0;
	restaurant->cash = 0;

	sharedMemory->write(*restaurant);

	new Semaphore(FILE_RESTAURANT,
	KEY_MEMORY, 1);

	new Semaphore(FILE_RESTAURANT,
	KEY_TABLES, 0);
}

void destroy(SharedMemory<restaurant_t>* sharedMemory) {

	unsigned long pid = getpid();
	Logger::getInstance()->insert(KEY_RESTO, STRINGS_DESTROY, pid);

	Fifo* fifo = new Fifo(DINER_IN_DOOR);
	fifo->cerrar();
	fifo->_destroy();

	fifo = new Fifo(DINER_IN_LIVING);
	fifo->cerrar();
	fifo->_destroy();

	fifo = new Fifo(ORDERS);
	fifo->cerrar();
	fifo->_destroy();

	fifo = new Fifo(ORDERS_TO_COOK);
	fifo->cerrar();
	fifo->_destroy();

	LockFile* lock = new LockFile(DINER_IN_DOOR_LOCK);
	lock->~LockFile();

	lock = new LockFile(DINER_IN_LIVING_LOCK);
	lock->~LockFile();

	lock = new LockFile(ORDERS_LOCK);
	lock->~LockFile();

	sharedMemory->free();
}

