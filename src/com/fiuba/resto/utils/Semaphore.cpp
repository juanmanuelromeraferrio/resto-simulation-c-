#include "Semaphore.h"

Semaphore::Semaphore(const char* name, char key, int initValue) {

	this->initValue = initValue;

	key_t clave = ftok(name, key);
	this->id = semget(clave, 1, 0666 | IPC_CREAT);

	this->init();
}

Semaphore::~Semaphore() {
}

void Semaphore::destroy() {
	semctl(this->id, 0, IPC_RMID);
}

int Semaphore::init() {

	union semnum {
		int val;
		struct semid_ds* buf;
		ushort* array;
	};

	semnum init;
	init.val = this->initValue;
	int resultado = semctl(this->id, 0, SETVAL, init);
	return resultado;
}

int Semaphore::p() {

	struct sembuf operacion;

	operacion.sem_num = 0;
	operacion.sem_op = -1;
	operacion.sem_flg = SEM_UNDO;

	int resultado = semop(this->id, &operacion, 1);
	return resultado;
}

int Semaphore::v() {

	struct sembuf operacion;

	operacion.sem_num = 0;
	operacion.sem_op = 1;
	operacion.sem_flg = SEM_UNDO;

	int resultado = semop(this->id, &operacion, 1);
	return resultado;
}
int Semaphore::wait() {
	return p();
}

int Semaphore::signal() {
	return v();
}
