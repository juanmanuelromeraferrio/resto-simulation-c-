#ifndef SHAREDMEMORY_H_
#define SHAREDMEMORY_H_

#define SHM_OK			0
#define	ERROR_FTOK		-1
#define ERROR_SHMGET	-2
#define	ERROR_SHMAT		-3

#include "../logger/Logger.h"

#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>
#include 	<stdlib.h>
#include 	<iostream>

template<class T> class SharedMemory {

private:

	int shmId;
	T* dataPointer;

	int cantidadProcesosAdosados();

public:

	SharedMemory();
	~SharedMemory();
	int create(const char *file, char key);
	int create(const char *file, char key, bool exclusive);
	void free();
	void write(T data);
	T read();

};

template<class T> SharedMemory<T>::SharedMemory() {
	this->shmId = 0;
	this->dataPointer = NULL;
}

template<class T> SharedMemory<T>::~SharedMemory() {
}
template<class T> int SharedMemory<T>::create(const char *file, char key) {
	return this->create(file, key, false);

}

template<class T> int SharedMemory<T>::create(const char *file, char key,
		bool exclusive) {
	// generacion de la clave
	key_t clave = ftok(file, key);
	if (clave == -1)
		return ERROR_FTOK;
	else {
		int flags = 0644 | IPC_CREAT;
		if (exclusive) {
			flags |= IPC_EXCL;
		}
		// creacion de la memoria compartida
		this->shmId = shmget(clave, sizeof(T), flags);

		if (this->shmId == -1)
			return ERROR_SHMGET;
		else {
			// attach del bloque de memoria al espacio de direcciones del proceso
			void* ptrTemporal = shmat(this->shmId, NULL, 0);

			if (ptrTemporal == (void *) -1) {
				return ERROR_SHMAT;
			} else {
				this->dataPointer = (T *) ptrTemporal;
//				std::stringstream log;
//				log << "Attach memoria compartida " << this->shmId;
//				Logger::getInstance()->insert(KEY_MEMORIA, log.str());
				return SHM_OK;
			}
		}
	}
}

template<class T> void SharedMemory<T>::free() {
	shmdt((void *) this->dataPointer);

	int procAdosados = this->cantidadProcesosAdosados();

	if (procAdosados == 0) {
		if (shmctl(this->shmId, IPC_RMID, NULL) == -1) {
		}

	}
}

template<class T> void SharedMemory<T>::write(T data) {
	*(this->dataPointer) = data;
}

template<class T> T SharedMemory<T>::read() {
	return (*(this->dataPointer));
}

template<class T> int SharedMemory<T>::cantidadProcesosAdosados() {
	shmid_ds estado;
	shmctl(this->shmId, IPC_STAT, &estado);
	return estado.shm_nattch;
}

#endif /* SHAREDMEMORY_H_ */
