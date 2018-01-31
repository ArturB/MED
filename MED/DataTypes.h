#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

// mo�liwe typy atrybut�w w danych: dyskretny lub numeryczny. 
enum DATA_TYPE { DISCRETE, NUMERIC };

// Nag��wek atrybutu w zbiorze danych. 
struct DataHeader {
	// nazwa atrybutu
	std::string header;

	// typ atrybutu
	DATA_TYPE type;

	// prosty konstruktor
	DataHeader(std::string h, DATA_TYPE t) : header(h), type(t) {}
};