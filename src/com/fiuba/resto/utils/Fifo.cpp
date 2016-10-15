#include "Fifo.h"

#include "../logger/Logger.h"

#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

Fifo::Fifo(std::string nombre) {

	this->nombre = nombre;
	// se crea el fifo
	mknod(nombre.c_str(), S_IFIFO | 0666, 0);
	// se inicializa el descriptor en -1
	this->fileDes = -1;
}

Fifo::~Fifo() {
	if (this->fileDes != -1) {
		close(this->fileDes);
	}
}

void Fifo::_open(int __oflag) {
	if (this->fileDes == -1) {
		this->fileDes = open(this->nombre.c_str(), __oflag);
	}
}

int Fifo::_write(char* dato, int datoSize) {
	// se abre el file descriptor para escritura
	if (this->fileDes == -1) {
		this->fileDes = open(this->nombre.c_str(), O_WRONLY);
	}

	// se escriben los datos en el fifo
	int resultado = write(this->fileDes, (const void *) dato, datoSize);

	return resultado;
}

int Fifo::_read(char* buffer, int buffSize) {
	int resultado = 0;

	while (resultado == 0) {
		if (this->fileDes == -1) {
			this->fileDes = open(this->nombre.c_str(), O_RDONLY);
		}
		resultado = read(this->fileDes, (void *) buffer, buffSize);
		if (resultado == 0) {
			cerrar();
		}
	}

	return resultado;
}

void Fifo::cerrar() {
	close(this->fileDes);
	this->fileDes = -1;
}

void Fifo::_destroy() {
	unlink(this->nombre.c_str());
}
