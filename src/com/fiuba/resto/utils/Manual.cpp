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
	cout << "    Opciones					Descripcion" << endl;
	cout << "    ---------------			-------------------" << endl;
	cout << "    -i  --init				Inicia el restaurant " << endl;
	cout
			<< "    -d  --diner=cantidad		Ingresa la catidad de diners establecidos al restaurant"
			<< endl;
	cout
			<< "    -q  --query=TIPO            Devuelve la información correspondiente al "
			<< endl;
	cout << "                                TIPO: 1, Caja " << endl;
	cout << "                                      2, Gente en el Living "
			<< endl;
	cout << "                                      3, Perdidas " << endl;
	cout << "                                      4, Todo " << endl;
	cout
			<< "    -p  --power=TIPO               Simula el corte o vuelta de la luz "
			<< endl;
	cout << "                                TIPO: 0, Corte de Luz " << endl;
	cout << "                                TIPO: 1, Vuelve la Luz " << endl;
	cout << "    -h  --help                  Muestra esta leyenda" << endl;

}

