#pragma once

#include <string>
#include "ParsedData.h"

enum PART_TYPE { NO, IN, LE, LEAF};

std::string printPartType(PART_TYPE type);

class SprintPartition
{
public:

	PART_TYPE part_type;
	int part_col;
	std::string thr;
	//ParsedData<std::string>* data1;
	//ParsedData<std::string>* data2;
	std::vector<int> set1;
	std::vector<int> set2;
	double gini;

	SprintPartition();
	~SprintPartition();
};

