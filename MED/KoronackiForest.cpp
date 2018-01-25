#include "KoronackiForest.h"



KoronackiForest::KoronackiForest(double gini_thr) {
	this->gini_thr = gini_thr;
}


KoronackiForest::~KoronackiForest()
{
}

std::vector<int> KoronackiForest::getCondAttrsIndexes(ParsedData<std::string>& dataSet, int decAttrIndex) {

	std::vector<int> condAttrs = std::vector<int>();

	for (int i = 0; i < dataSet.getRow(0).size(); ++i) {
			condAttrs.push_back(i);
	}

	return condAttrs;
}

std::pair<std::vector<ParsedData<std::string>>, ParsedData<std::string>> KoronackiForest::drawDataSets(ParsedData<std::string>& data, int nrOfRecords) {

	std::vector<ParsedData<std::string>> drawedDataSets = std::vector<ParsedData<std::string>>();
	ParsedData<std::string> trainingData = ParsedData<std::string>(data.getData(), data.getHeaders());
	std::set<int> indexToRemove = std::set<int>();
	int dataSize = data.getData().size();

	for (int i = 0; i < nrOfRecords; ++i) {
		ParsedData<std::string> p = ParsedData<std::string>();
		p.setHeaders(data.getHeaders());
		drawedDataSets.push_back(p);
	}

	for (int i = 0; i < int(dataSize / nrOfRecords); ++i) {
		for (int j = 0; j < nrOfRecords; ++j) {
			int index = rand() % dataSize;
			std::vector<std::string> row = data.getRow(index);
			indexToRemove.insert(index);
			drawedDataSets[j].addRow(row);
		}
	}

	std::set<int>::reverse_iterator revIt = indexToRemove.rbegin();

	while (revIt != indexToRemove.rend()) {
		trainingData.removeRow(*revIt);
		revIt++;
	}

	std::pair<std::vector<ParsedData<std::string>>, ParsedData<std::string>> dataSets = std::pair<std::vector<ParsedData<std::string>>, ParsedData<std::string>>(drawedDataSets, trainingData);

	return dataSets;
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

std::map<int, double> KoronackiForest::getAttrsAccuracy(std::map<int, ParsedData<std::string>> attrsDataSets, ParsedData<std::string> trainingData, int decisionAttr) {

	std::map<int, double> attrsAccuracy = std::map<int, double>();

	int h = 0;
	for (auto& x : attrsDataSets) {
		++h;
		std::cout << "Koronacki, calculating tree " << h << "/" << attrsDataSets.size() << std::endl;
		SprintTree tree = SprintTree(x.second, decisionAttr, gini_thr);
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

void KoronackiForest::printAttrsWeight(std::map<int, double> attrsWeight, std::vector<DataHeader> headers) {

	std::cout << "Wagi atrybutow:" << std::endl;
	
	for (auto const& x : attrsWeight) {
		std::cout << headers[x.first].header << " - " << x.second << std::endl;
	}
}

std::map<int, double> KoronackiForest::calculateAttrsWeight(ParsedData<std::string> data, int decisionAttr) {

	std::vector<int> condAttrsIndexes = getCondAttrsIndexes(data, decisionAttr);
	std::pair<std::vector<ParsedData<std::string>>, ParsedData<std::string>> dataSets = drawDataSets(data, condAttrsIndexes.size());
	std::vector<ParsedData<std::string>> drawedDataSets = dataSets.first;
	ParsedData<std::string> trainingData = dataSets.second;
	std::map<int, ParsedData<std::string>> drawedAttrsDataSets = std::map<int, ParsedData<std::string>>();
	std::map<int, ParsedData<std::string>> permutedAttrsDataSets = std::map<int, ParsedData<std::string>>();

	for (int i = 0; i < condAttrsIndexes.size(); ++i) {
		int condAttrIndex = condAttrsIndexes[i];
		drawedAttrsDataSets[condAttrIndex] = drawedDataSets[i];
		permutedAttrsDataSets[condAttrIndex] = permutateDataSetAttr(drawedDataSets[i], condAttrIndex);
	}

	std::cout << "Trees with normal columns" << std::endl;
	std::map<int, double> drawedAttrsAccuracy = getAttrsAccuracy(drawedAttrsDataSets, trainingData, decisionAttr);
	std::cout << "Trees with permuted columns" << std::endl;
	std::map<int, double> permutedAttrsAccuracy = getAttrsAccuracy(permutedAttrsDataSets, trainingData, decisionAttr);
	std::map<int, double> attrsWeight = getAttrsWeight(drawedAttrsAccuracy, permutedAttrsAccuracy);
	//printAttrsWeight(attrsWeight, data.getHeaders());

	return attrsWeight;
}