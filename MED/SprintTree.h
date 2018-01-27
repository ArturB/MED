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
	//void generateTree(ParsedData<std::string>& data, std::vector<int>& set, int decision_attr_, double gini_thr);
	

public:

	static SprintTree create(ParsedData<std::string>& data, int decision_attr_, double gini_thr);

	std::string biggest_class(ParsedData<std::string>& data, std::vector<int>);
	SprintPartition best_attr_numeric_partition(ParsedData<std::string>& data, std::vector<int>& set, int col_num);
	SprintPartition best_attr_discrete_partition(ParsedData<std::string>& data, std::vector<int>& set, int col_num);
	double gini(ParsedData<std::string>&, std::vector<int>&, std::vector<int>&);

	SprintTree(int decision_attr_);
	SprintTree(ParsedData<std::string>& data, std::vector<int>, int decision_attr_, double gini_thr);

	std::string classify(std::vector<std::string>& row);
	double accuracy(ParsedData<std::string>& data);

	int nodes_number();
	int leafs_number();

	~SprintTree();
};


