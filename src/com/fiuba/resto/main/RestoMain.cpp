#include <getopt.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "../logger/Logger.h"
#include "../logger/Strings.h"
#include "../process/Attendant.h"
#include "../process/Cook.h"
#include "../process/Diner.h"
#include "../process/Host.h"
#include "../process/Waiter.h"
#include "../types/types.h"
#include "../utils/Constant.h"
#include "../utils/Fifo.h"
#include "../utils/LockFile.h"
#include "../utils/Manual.h"
#include "../utils/MemoriaCompartida2.h"
#include "../utils/signals/SignalHandler.h"
#include "../utils/signals/SIGINT_Handler.h"
#include "../utils/signals/SIGQUIT_Handler.h"
#include "../utils/Semaphore.h"

static const char *optString = "icd:q:p:sh?";

static const struct option longOpts[] = { { "init", no_argument, NULL, 'i' }, {
		"close", no_argument, NULL, 'c' }, { "diner", required_argument, NULL,
		'd' }, { "query", required_argument, NULL, 'q' }, { "power",
required_argument, NULL, 'p' }, { "help", no_argument,
NULL, 'h' }, { "simular", no_argument, NULL, 's' }, { NULL, no_argument,
NULL, 0 } };

void destroy(MemoriaCompartida2<restaurant_t>* sharedMemory);

void showFinalInfo(MemoriaCompartida2<restaurant_t>* sharedMemory);

void initValues(MemoriaCompartida2<restaurant_t>* sharedMemory,
		restaurant_t *restaurant);

void sigterm_handler(int sig) {
	Logger::getInstance()->insert(KEY_RESTO,
			"Finalizando Restaurant en 4 Segundos ....");
	sleep(2);
	kill(0, SIGINT);
	sleep(2);
	kill(0, SIGINT);
}

int main(int argc, char** argv) {
	restaurant_t restaurant;

	FILE* file = fopen(FILE_RESTAURANT, "w");
	fclose(file);

	int opt = 0;
	int longIndex;

	unsigned long resto_pid = getpid();
	Logger::getInstance()->insert(KEY_RESTO, "Initing Resto ", resto_pid);

	opt = getopt_long(argc, argv, optString, longOpts, &longIndex);

	while (opt != -1) {
		switch (opt) {
		case 'i':
			try {
				MemoriaCompartida2<restaurant_t> sharedMemory_i(
				FILE_RESTAURANT, KEY_MEMORY);
				initValues(&sharedMemory_i, &restaurant);

				signal(SIGTERM, sigterm_handler);

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

								SIGINT_Handler sigint_handler;
								SIGQUIT_Handler sigquit_handler;
								SignalHandler::getInstance()->registerHandler(
								SIGINT, &sigint_handler);
								SignalHandler::getInstance()->registerHandler(
								SIGQUIT, &sigquit_handler);

								int childs = HOSTS + WAITERS + 1 + 1; //1 cook y 1 attendant
								for (int i = 0; i < childs; i++) {
									wait(NULL);
								}
							}
						}
					}
				}

				unsigned long pid = getpid();
				if (pid == resto_pid) {
					showFinalInfo(&sharedMemory_i);
					destroy(&sharedMemory_i);
				}

			} catch (std::string& mensaje) {
				std::cerr << mensaje << std::endl;
			}
			break;

		case 'd':
			try {
				MemoriaCompartida2<restaurant_t> sharedMemory_d(
				FILE_RESTAURANT, KEY_MEMORY);

				restaurant_t restaurant = sharedMemory_d.leer();

				if (restaurant.isOpen) {
					restaurant.diners_main_pid = getpid();
					sharedMemory_d.escribir(restaurant);
				}

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
				} else {

					//Si no se corto la luz, espero a que terminen los diners
					for (int i = 0; i < diners; i++) {
						wait(NULL);
					}
				}
			} catch (std::string& mensaje) {
				std::cerr << mensaje << std::endl;
			}
			break;

		case 'q':
			try {
				MemoriaCompartida2<restaurant_t> buffer_consulta(
				FILE_RESTAURANT, KEY_MEMORY);
				restaurant_t consulta_restaurant = buffer_consulta.leer();
				if (consulta_restaurant.isOpen) {
					int opcion = 0;
					if (optarg != NULL)
						opcion = atoi(optarg);
					switch (opcion) {
					case 1:
						Logger::getInstance()->insert(KEY_RESTO, STRINGS_CASH,
								consulta_restaurant.cash);
						break;
					case 2:
						Logger::getInstance()->insert(KEY_RESTO,
						STRINGS_DINERS_IN_LIVING,
								consulta_restaurant.dinersInLiving);
						break;
					case 3:
						Logger::getInstance()->insert(KEY_RESTO,
						STRINGS_CASH_LOST,
								consulta_restaurant.money_not_cashed);
						break;
					case 4:
						Logger::getInstance()->insert(KEY_RESTO, STRINGS_CASH,
								consulta_restaurant.cash);
						Logger::getInstance()->insert(KEY_RESTO,
						STRINGS_DINERS_IN_LIVING,
								consulta_restaurant.dinersInLiving);
						Logger::getInstance()->insert(KEY_RESTO,
						STRINGS_CASH_LOST,
								consulta_restaurant.money_not_cashed);
						break;
					default:
						Logger::getInstance()->insert(KEY_RESTO,
								"Consultar: \n 1) Caja \n 2) Gente en living \n 3) Perdidas\n 4) Todo");
						break;
					}
				} else {
					Logger::getInstance()->insert(KEY_RESTO,
							"Restaurant cerrado - No puede hacer consultas");
				}
			} catch (std::string& mensaje) {
				std::cerr << mensaje << std::endl;
			}
			break;
		case 'p':
			try {
				MemoriaCompartida2<restaurant_t> memoria(
				FILE_RESTAURANT, KEY_MEMORY);

				restaurant_t restaurant = memoria.leer();
				if (restaurant.isOpen) {
					int option = 0;

					if (optarg != NULL) {
						option = atoi(optarg);
					}

					switch (option) {
					case 0:
						Logger::getInstance()->insert(KEY_RESTO,
								"Cortando la Luz ");
						killpg(restaurant.main_pid, SIGQUIT);
						killpg(restaurant.diners_main_pid, SIGQUIT);
						break;
					case 1:
						Logger::getInstance()->insert(KEY_RESTO,
								"Vuelve la Luz ");
						killpg(restaurant.main_pid, SIGQUIT);
						break;
					}

				} else {
					Logger::getInstance()->insert(KEY_RESTO,
							"Restaurant cerrado - No puedo simular corte de luz");
				}
			} catch (std::string& mensaje) {
				std::cerr << mensaje << std::endl;
			}
			break;
		case 'h':
			try {
				Manual manual;
				manual.showInstructions();
			} catch (std::string& mensaje) {
				std::cerr << mensaje << std::endl;
			}
			break;
		}

		break;
	}

	return 0;
}

void initValues(MemoriaCompartida2<restaurant_t>* sharedMemory,
		restaurant_t *restaurant) {
	restaurant->main_pid = getpid();
	restaurant->diners_main_pid = 0;
	restaurant->tables = TABLES;
	restaurant->busyTables = 0;
	restaurant->dinersInLiving = 0;
	restaurant->cash = 0;
	restaurant->diners = 0;
	restaurant->dinersInRestaurant = 0;
	restaurant->money_not_cashed = 0;
	restaurant->isOpen = true;

	sharedMemory->escribir(*restaurant);

	new Semaphore(FILE_RESTAURANT, KEY_MEMORY, 1);
	new Semaphore(FILE_RESTAURANT, KEY_TABLES, 0);
}

void showFinalInfo(MemoriaCompartida2<restaurant_t>* sharedMemory) {

	restaurant_t consulta_restaurant = sharedMemory->leer();

	Logger::getInstance()->insert(KEY_RESTO, "");
	Logger::getInstance()->insert(KEY_RESTO, "***** INFOMACION FINAL *****");
	Logger::getInstance()->insert(KEY_RESTO, STRINGS_CASH,
			consulta_restaurant.cash);
	Logger::getInstance()->insert(KEY_RESTO,
	STRINGS_DINERS_IN_LIVING, consulta_restaurant.dinersInLiving);
	Logger::getInstance()->insert(KEY_RESTO,
	STRINGS_CASH_LOST, consulta_restaurant.money_not_cashed);
	Logger::getInstance()->insert(KEY_RESTO, "");

}
void destroy(MemoriaCompartida2<restaurant_t>* sharedMemory) {
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

	Semaphore* semaphore = new Semaphore(FILE_RESTAURANT, KEY_MEMORY);
	semaphore->destroy();

	semaphore = new Semaphore(FILE_RESTAURANT, KEY_TABLES);
	semaphore->destroy();

	Logger::getInstance()->insert(KEY_RESTO, STRINGS_FINISHED);
}
