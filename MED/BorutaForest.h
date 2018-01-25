#pragma once
#include "KoronackiForest.h"

class BorutaForest
{
private:
	double gini_thr;
	KoronackiForest koroneckiForest;

	ParsedData<std::string>& replicateDataAttr(ParsedData<std::string>& data);

public:
	BorutaForest(double gini_thr);
	void printAttrsWeight(std::map<int, double> attrsWeight, std::vector<DataHeader> headers);
	std::map<int, double> getAttrsWeight(ParsedData<std::string> data, ParsedData<std::string> trainingData, int decisionAttr);

	~BorutaForest();
};

