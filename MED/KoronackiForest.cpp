#include "KoronackiForest.h"



KoronackiForest::KoronackiForest(ParsedData<std::string> data, ParsedData<std::string> trainingData, int decisionAttr) {
	this->data = data;
	this->trainingData = trainingData;
	this->decision_attr = decisionAttr;
}


KoronackiForest::~KoronackiForest()
{
}

std::vector<int> KoronackiForest::getCondAttrsIndexes(ParsedData<std::string>& dataSet, int decAttrIndex) {

	std::vector<int> condAttrs = std::vector<int>();

	for (int i = 0; i < dataSet.getRow(0).size(); ++i) {
		if (i != decAttrIndex) {
			condAttrs.push_back(i);
		}
	}

	return condAttrs;
}

std::vector<ParsedData<std::string>> KoronackiForest::drawDataSets(ParsedData<std::string>& data, int nrOfRecords) {

	std::vector<ParsedData<std::string>> drawedDataSets = std::vector<ParsedData<std::string>>();
	int dataSize = data.getData().size();

	for (int i = 0; i < nrOfRecords; ++i) {
		ParsedData<std::string> p = ParsedData<std::string>();
		p.setHeaders(data.getHeaders());
		drawedDataSets.push_back(p);
	}

	for (int i = 0; i < int(dataSize / nrOfRecords); ++i) {
		for (int j = 0; j < nrOfRecords; ++j) {
			drawedDataSets[j].addRow(data.getRow(rand() % dataSize));
		}
	}

	return drawedDataSets;
}

ParsedData<std::string> KoronackiForest::permutateDataSetAttr(ParsedData<std::string>& dataSet, int attrIndex) {

	std::vector<std::string> permutedColumn = dataSet.getColumn(attrIndex);
	ParsedData<std::string> permutedDataSet = ParsedData<std::string>();
	permutedDataSet.setHeaders(dataSet.getHeaders());

	int dataSize = dataSet.getData().size();

	for (int i = dataSize; i >= 2; --i) {
		std::iter_swap(permutedColumn.begin() + (rand() % i), permutedColumn.begin() + (i - 1));
	}

	dataSet.setColumn(attrIndex, permutedColumn);
	return dataSet;
}

std::map<int, double> KoronackiForest::getAttrsAccuracy(std::map<int, ParsedData<std::string>> attrsDataSets, ParsedData<std::string> trainingData) {

	std::map<int, double> attrsAccuracy = std::map<int, double>();

	for (auto& x : attrsDataSets) {
		SprintTree tree = SprintTree(x.second, decision_attr, gini_thr);
		attrsAccuracy[x.first] = tree.accuracy(trainingData);
	}

	return attrsAccuracy;
}

std::map<int, double> KoronackiForest::getAttrsWeight(std::map<int, double>& attrsAccuracy, std::map<int, double>& attrsAccuracyPermuted) {

	std::map<int, double> attrsWeight = std::map<int, double>();

	for (auto& x : attrsAccuracy) {
		attrsWeight[x.first] = x.second - attrsAccuracyPermuted[x.first];
	}

	return attrsWeight;
}

void KoronackiForest::printAttrsWeight(std::map<int, double> attrsWeight) {

	std::vector<DataHeader> headers = data.getHeaders();
	std::cout << "Wagi atrybutow:" << std::endl;
	
	for (auto const& x : attrsWeight) {
		std::cout << headers[x.first].header << " - " << x.second << std::endl;
	}
}

std::map<int, double> KoronackiForest::getAttrsWeight() {

	std::vector<int> condAttrsIndexes = getCondAttrsIndexes(data, decision_attr);
	std::vector<ParsedData<std::string>> drawedDataSets = drawDataSets(data, condAttrsIndexes.size());
	std::map<int, ParsedData<std::string>> drawedAttrsDataSets = std::map<int, ParsedData<std::string>>();
	std::map<int, ParsedData<std::string>> permutedAttrsDataSets = std::map<int, ParsedData<std::string>>();

	for (int i = 0; i < condAttrsIndexes.size(); ++i) {
		int condAttrIndex = condAttrsIndexes[i];
		drawedAttrsDataSets[condAttrIndex] = drawedDataSets[i];
		permutedAttrsDataSets[condAttrIndex] = permutateDataSetAttr(drawedDataSets[i], condAttrIndex);
	}

	std::map<int, double> drawedAttrsAccuracy = getAttrsAccuracy(drawedAttrsDataSets, trainingData);
	std::map<int, double> permutedAttrsAccuracy = getAttrsAccuracy(permutedAttrsDataSets, trainingData);
	std::map<int, double> attrsWeight = getAttrsWeight(drawedAttrsAccuracy, permutedAttrsAccuracy);
	printAttrsWeight(attrsWeight);

	return attrsWeight;
}