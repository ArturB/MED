#pragma once
#include <string>
#include <numeric>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include "ParsedData.h"
#include "SprintTree.h"

class KoronackiForest
{
private:
	const double gini_thr = 0.1;
	int decision_attr;
	ParsedData<std::string> data;
	ParsedData<std::string> trainingData;

	std::vector<int> getCondAttrsIndexes(ParsedData<std::string>& dataSet, int decAttrIndex);
	std::vector<ParsedData<std::string>> drawDataSets(ParsedData<std::string>& data, int nrOfRecords);
	ParsedData<std::string> permutateDataSetAttr(ParsedData<std::string>& dataSet, int attrIndex);
	std::map<int, double> getAttrsAccuracy(std::map<int, ParsedData<std::string>> attrsDataSets, ParsedData<std::string> trainingData);
	std::map<int, double> getAttrsWeight(std::map<int, double>& attrsAccuracy, std::map<int, double>& attrsAccuracyPermuted);

public:
	KoronackiForest(ParsedData<std::string> data, ParsedData<std::string> trainingData, int decisionAttr);
	void printAttrsWeight(std::map<int, double> attrsWeight);
	std::map<int, double> getAttrsWeight();

	~KoronackiForest();
};