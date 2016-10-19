#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class Parser {

private:

	static Parser* parser;
	std::fstream fs;
	nlohmann::json js;
	Parser();

public:
	static Parser* getInstance();
	virtual ~Parser();
	int getIntValue(std::string value);
};

#endif /** PARSER_H */
