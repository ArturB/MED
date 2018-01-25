#pragma once
#include <string>
#include <numeric>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <set>
#include "ParsedData.h"
#include "SprintTree.h"

class KoronackiForest
{
private:
	double gini_thr;

	std::vector<int> getCondAttrsIndexes(ParsedData<std::string>& dataSet, int decAttrIndex);
	std::pair<std::vector<ParsedData<std::string>>, ParsedData<std::string>> drawDataSets(ParsedData<std::string>& data, int nrOfRecords);
	ParsedData<std::string> permutateDataSetAttr(ParsedData<std::string>& dataSet, int attrIndex);
	std::map<int, double> getAttrsAccuracy(std::map<int, ParsedData<std::string>> attrsDataSets, ParsedData<std::string> trainingData, int decisionAttr);

public:
	KoronackiForest(double gini_thr);
	void printAttrsWeight(std::map<int, double> attrsWeight, std::vector<DataHeader> headers);
	std::map<int, double> getAttrsWeight(std::map<int, double>& attrsAccuracy, std::map<int, double>& attrsAccuracyPermuted);
	std::map<int, double> calculateAttrsWeight(ParsedData<std::string> data, int decisionAttr);

	~KoronackiForest();
};