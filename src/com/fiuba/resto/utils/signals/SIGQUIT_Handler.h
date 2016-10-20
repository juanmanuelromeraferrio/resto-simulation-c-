#ifndef SIGQUIT_HANDLER_H_
#define SIGQUIT_HANDLER_H_

#include <sys/wait.h>
#include <cassert>

#include "EventHandler.h"

class SIGQUIT_Handler: public EventHandler {

private:
	sig_atomic_t powerOutage;

public:

	SIGQUIT_Handler() {
		this->powerOutage = 0;
	}

	~SIGQUIT_Handler() {
	}

	virtual int handleSignal(int signum) {
		assert(signum == SIGQUIT);

		if (this->powerOutage == 0) {
			this->powerOutage = 1;
		} else {
			this->powerOutage = 0;
		}

		return 0;
	}

	sig_atomic_t getPowerOutage() {
		return this->powerOutage;
	}

};

#endif /* SIGQUIT_HANDLER_H_ */
