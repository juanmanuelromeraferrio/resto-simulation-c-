#include "LockFile.h"

LockFile::LockFile(const char* name) {

	strcpy(this->name, name);

	this->fl.l_type = F_WRLCK;
	this->fl.l_whence = SEEK_SET;
	this->fl.l_start = 0;
	this->fl.l_len = 0;
	this->fl.l_pid = getpid();
	this->fd = open(this->name, O_CREAT | O_WRONLY, 0777);
}

int LockFile::lock() {
	this->fl.l_type = F_WRLCK;
	int resultado = fcntl(this->fd, F_SETLKW, &(this->fl));
	return resultado;
}

int LockFile::unlock() {
	this->fl.l_type = F_UNLCK;
	int resultado = fcntl(this->fd, F_SETLK, &(this->fl));
	return resultado;
}

int LockFile::_write(char* buffer, int buffsize) {
	lseek(this->fd, 0, SEEK_END);
	int resultado = write(this->fd, buffer, buffsize);
	return resultado;
}

LockFile::~LockFile() {
	close(this->fd);
}
