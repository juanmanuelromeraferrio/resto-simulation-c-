#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define KEY_HOST "Host"
#define KEY_DINER "Diner"
#define KEY_WAITER "Waiter"
#define KEY_FIFO "Fifo"
#define KEY_DINER_TO_LIVING_ACTION "Diner To Living Action"
#define KEY_DINER_IN_LIVING_ACTION "Diner In Living Action"
#define KEY_ORDER_TO_COOK_ACTION "Order To Cook Action"
#define KEY_COOK "Cook"

class Logger {

private:

	static Logger* logger;
	std::fstream ioFile;
	Logger();
	std::string getTime();

public:
	static Logger* getInstance();
	void debug(std::string message);
	void info(std::string message);
	void warn(std::string message);
	void error(std::string message);
	void insert(std::string key, std::string value, unsigned int param);
	void insert(std::string key, std::string value);
	virtual ~Logger();
};

#endif /** LOGGER_H */
