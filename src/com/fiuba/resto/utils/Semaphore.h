#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

class Semaphore {

private:
	int id;
	int initValue;

	int init();

public:
	Semaphore(const char* name, char key, int initValue);
	virtual ~Semaphore();

	int p();
	int wait();

	int v();
	int signal();

	void destroy();
};

#endif /* SEMAPHORE_H_ */
