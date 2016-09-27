/*
 * Manual.cpp
 *
 *  Created on: 24 sep. 2016
 *      Author: jferrio
 */

#include "Manual.h"
#include <iostream>

using namespace std;

Manual::Manual() {
}

Manual::~Manual() {
}

void Manual::showInstructions() {

	cout << "Instrucciones " << endl;
	cout << "    Opciones                    Descripcion" << endl;
	cout << "    ---------------             -------------------" << endl;
	cout
			<< "    -i  --init=segundos         Inicia el restaurant en modo demonio "
			<< endl;
	cout
			<< "                                durante el tiempo en segundos establecido"
			<< endl;

}

