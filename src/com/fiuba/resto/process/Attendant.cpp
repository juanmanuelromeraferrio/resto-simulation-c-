/*
 * Attendant.cpp
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#include "Attendant.h"

#include <unistd.h>
#include <csignal>
#include <exception>

#include "../logger/Logger.h"
#include "../logger/Strings.h"
#include "../utils/Constant.h"
#include "../utils/signals/SignalHandler.h"
#include "../utils/signals/SIGINT_Handler.h"

using namespace std;

Attendant::Attendant() {

	sharedMemory.create(FILE_RESTAURANT, KEY_MEMORY);

	this->dinersInLivingFifo = new Fifo(DINER_IN_LIVING);
	this->freeTableSemaphore = new Semaphore(FILE_RESTAURANT,
	KEY_TABLES);
	this->memorySemaphore = new Semaphore(FILE_RESTAURANT,
	KEY_MEMORY);
}

Attendant::~Attendant() {
	sharedMemory.free();
}

void Attendant::run() {

	SIGINT_Handler sigint_handler;
	SignalHandler::getInstance()->registerHandler(SIGINT, &sigint_handler);

	while (sigint_handler.getGracefulQuit() == 0) {
		try {
			asignTable();
		} catch (exception& e) {
			if (sigint_handler.getGracefulQuit() == 0) {
				throw e;
			}
		}
	}

	SignalHandler::destroy();
}

void Attendant::asignTable() {
	int dinerPid;
	int result = dinersInLivingFifo->_read((char*) (&dinerPid),
			sizeof(unsigned long));

	if (result == -1) {
		exception e;
		throw e;
	}

	freeTableSemaphore->wait();

	// Actualizo Lista de Comensales
	memorySemaphore->wait();
	restaurant_t restaurant = sharedMemory.read();
	restaurant.dinersInLiving--;
	sharedMemory.write(restaurant);
	memorySemaphore->signal();

	//Enivo Mensaje a Comensal
	Logger::getInstance()->insert(KEY_ATTENDANT,
	STRINGS_ASSIGN_TABLE, dinerPid);

	stringstream ssDinerFifoName;
	ssDinerFifoName << DINERS_FIFO << dinerPid;

	char response = 1;
	Fifo* dinerFifo = new Fifo(ssDinerFifoName.str());
	dinerFifo->_write(&response, sizeof(char));
	dinerFifo->cerrar();
	delete (dinerFifo);
}
