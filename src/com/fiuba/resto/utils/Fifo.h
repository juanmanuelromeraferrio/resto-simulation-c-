#ifndef FIFO_H_
#define FIFO_H_

#include <unistd.h>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>

class Fifo {
private:
	std::string nombre;
	int fileDes;

public:
	Fifo ( std::string nombre );
	virtual ~Fifo();

	int _write ( char* dato,int datoSize );
	int _read ( char* buffer,int buffSize );
	void cerrar ();
	void _destroy ();
};

#endif /* FIFO_H_ */
