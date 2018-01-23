#pragma once

#include <string>
#include <numeric>
#include <map>
#include "ParsedData.h"
#include "SprintPartition.h"


class SprintTree
{
private:
	int decision_attr;
	std::string decision_class;
	SprintPartition decision;
	SprintTree* Left;
	SprintTree* Right;

	std::string biggest_class(ParsedData<std::string>& data);
	SprintPartition best_attr_numeric_partition(ParsedData<std::string>& data, int col_num);
	SprintPartition best_attr_discrete_partition(ParsedData<std::string>& data, int col_num);
	double gini(ParsedData<std::string>& set1, ParsedData<std::string>& set2);

public:
	SprintTree(int decision_attr_);
	SprintTree(ParsedData<std::string>& data, int decision_attr_, double gini_thr);

	std::string classify(std::vector<std::string>& row);
	double accuracy(ParsedData<std::string>& data);

	int nodes_number();
	int leafs_number();

	~SprintTree();
};


