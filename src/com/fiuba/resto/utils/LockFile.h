#ifndef LOCKFILE_H_
#define LOCKFILE_H_

#include <unistd.h>
#include <fcntl.h>
#include <string.h>

class LockFile {

private:

	struct flock fl;
	int fd;
	char name [ 255 ];

public:

	LockFile ( const char* name );
	virtual ~LockFile();

	int lock ();
	int unlock ();
	int _write ( char* buffer,int buffsize );
};

#endif /* LOCKFILE_H_ */
