#pragma once

#include <string>
#include "ParsedData.h"

enum PART_TYPE { IN, LE, LEAF };

class SprintPartition
{
public:

	PART_TYPE part_type;
	int part_col;
	std::string thr;
	ParsedData<std::string>* data1;
	ParsedData<std::string>* data2;
	double gini;

	SprintPartition();
	~SprintPartition();
};

