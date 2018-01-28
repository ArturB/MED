#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

enum DATA_TYPE { DISCRETE, NUMERIC };

struct DataHeader {
	std::string header;
	DATA_TYPE type;
	DataHeader(std::string h, DATA_TYPE t) : header(h), type(t) {}
};