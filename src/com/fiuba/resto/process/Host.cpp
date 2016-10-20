/*
 * Host.cpp
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#include "Host.h"

#include <unistd.h>
#include <csignal>
#include <exception>
#include <sstream>

#include "../logger/Logger.h"
#include "../logger/Strings.h"
#include "../utils/Constant.h"
#include "../utils/signals/SignalHandler.h"
#include "../utils/signals/SIGINT_Handler.h"
#include "../process/action/AddDinerToLivingLineAction.h"

using namespace std;

Host::Host() {

	sharedMemory.create(FILE_RESTAURANT, KEY_MEMORY);

	this->dinerInDoorFifo = new Fifo(DINER_IN_DOOR);
	this->dinerInLivingFifo = new Fifo(DINER_IN_LIVING);

	this->dinerInDoorLock = new LockFile(DINER_IN_DOOR_LOCK);

	this->memorySemaphore = new Semaphore(FILE_RESTAURANT,
	KEY_MEMORY);

	this->sigquit_handler;
}

Host::~Host() {
	sharedMemory.free();
}

void Host::run() {

	SIGINT_Handler sigint_handler;
	SignalHandler::getInstance()->registerHandler(SIGINT, &sigint_handler);
	SignalHandler::getInstance()->registerHandler(SIGQUIT, &sigquit_handler);

	while (sigint_handler.getGracefulQuit() == 0) {
		try {

			unsigned long dinerPid = searchDinerInDoor();
			bool enter = dinerCanEnter();
			if (enter) {
				bool freeTable = existFreeTable();
				if (freeTable) {
					moveDinerToTable(dinerPid);
				} else {
					bool isMainProcess = moveDinerToLiving(dinerPid);
					if (!isMainProcess) {
						return;
					}
				}
			} else {
				sendOutDiner(dinerPid);
			}

		} catch (exception& e) {
		}
	}

	SignalHandler::destroy();
}

unsigned long Host::searchDinerInDoor() {

	unsigned long dinerPid = 0;

	dinerInDoorLock->lock();

	int result = dinerInDoorFifo->_read((char*) (&dinerPid),
			sizeof(unsigned long));

	if (result == -1) {
		dinerInDoorLock->unlock();
		exception e;
		throw e;
	}

	dinerInDoorLock->unlock();

	Logger::getInstance()->insert(KEY_HOST, STRINGS_SERVE_DINER, dinerPid);

	return dinerPid;
}

bool Host::dinerCanEnter() {
	bool canEnter = false;

	if (sigquit_handler.getPowerOutage() == 1) {
		return false;
	}

	memorySemaphore->wait();

	restaurant_t restaurant = sharedMemory.read();

	if (restaurant.diners < DINERS_TOTAL) {
		restaurant.diners++;
		restaurant.dinersInRestaurant++;
		sharedMemory.write(restaurant);
		canEnter = true;
	}

	memorySemaphore->signal();

	return canEnter;
}

bool Host::existFreeTable() {

	bool existFreeTable = false;

	memorySemaphore->wait();

	restaurant_t restaurant = sharedMemory.read();

	if (restaurant.tables > restaurant.busyTables) {
		restaurant.busyTables++;
		sharedMemory.write(restaurant);
		existFreeTable = true;
	}

	memorySemaphore->signal();

	return existFreeTable;

}

void Host::moveDinerToTable(unsigned long dinerPid) {

	Logger::getInstance()->insert(KEY_HOST, STRINGS_ASSIGN_TABLE, dinerPid);
	sleep(MOVE_TO_TABLE_TIME);

	stringstream ssDinerFifoName;
	ssDinerFifoName << DINERS_FIFO << dinerPid;

	char response = 1;
	Fifo* dinerFifo = new Fifo(ssDinerFifoName.str());
	dinerFifo->_write(&response, sizeof(char));
}

bool Host::moveDinerToLiving(unsigned long dinerPid) {

	Logger::getInstance()->insert(KEY_HOST, STRINGS_MOVE_DINER_TO_LIVING,
			dinerPid);
	sleep(MOVE_TO_LIVING_TIME);

	__pid_t id = fork();

	if (id == 0) {
		AddDinerToLivingLineAction action;
		action.run(dinerPid);
		return false;
	}

	return true;
}

void Host::sendOutDiner(unsigned long dinerPid) {

	Logger::getInstance()->insert(KEY_HOST, STRINGS_SEND_OUT, dinerPid);

	stringstream ssDinerFifoName;
	ssDinerFifoName << DINERS_FIFO << dinerPid;

	char response = 0;
	Fifo* dinerFifo = new Fifo(ssDinerFifoName.str());
	dinerFifo->_write(&response, sizeof(char));
}
