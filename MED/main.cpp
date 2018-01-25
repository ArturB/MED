#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include "SprintTree.h"
#include "BorutaForest.h"

DataType dataType;

std::pair<bool, std::vector<std::string>> getNextLineAndSplitIntoTokens(std::istream& str)
{
	std::pair<bool, std::vector<std::string>> nextLine;
	nextLine.first = true;
	std::vector<std::string> result;
	std::string line;
	std::getline(str, line);

	std::stringstream lineStream(line);
	std::string cell;

	while (std::getline(lineStream, cell, ',')) {
		result.push_back(cell);
		if (cell == " ?") {
			nextLine.first = false;
			break;
		}
	}
	if (!lineStream && cell.empty()) {
		result.push_back("");
		nextLine.first = false;
	}
	nextLine.second = result;
	return nextLine;
}

std::vector<std::vector<std::string>> loadData(std::string file) {

	std::vector< std::vector<std::string> > result;
	std::ifstream is(file);

	while (!is.eof()) {
		std::pair<bool, std::vector<std::string>> newLine = getNextLineAndSplitIntoTokens(is);
		if (newLine.first) {
			result.push_back(newLine.second);
		}
	}
	is.close();
	return result;
}

template<typename T>
void print_row(const std::vector<T>& v) {
	for (int i = 0; i < v.size(); ++i) {
		std::cout << v[i] << ", ";
	}
	std::cout << std::endl;
}

ParsedData<std::string> parseData(int typeNr) {

	DataType::dataType dType;
	if (typeNr == 1) {
		dType = dataType.adult;
	}
	else {
		dType = dataType.flag;
	}

	std::cout << "Loading " << dataType.getFileName(dType) << std::endl;
	std::vector<std::vector<std::string>> data = loadData(dataType.getFileName(dType));
	std::vector<DataHeader> headers = dataType.getHeaders(dType);
	std::cout << "Parsing " << dataType.getFileName(dType) << std::endl;
	ParsedData<std::string> parsedData = ParsedData<std::string>(data, headers);

	std::cout << "Data size: " << parsedData.getData().size() << std::endl;

	return parsedData;
}

void startKoronackiAlgorithm(ParsedData<std::string>& data, int decisionAttr) {
	KoronackiForest koronackiForest = KoronackiForest(0.1);
	std::map<int, double> attrsWeight = koronackiForest.calculateAttrsWeight(data, decisionAttr);
	std::cout << "-----Koronacki-----" << std::endl;
}

void startBorutaAlgorithm(ParsedData<std::string>& data, int decisionAttr) {
	BorutaForest borutaForest = BorutaForest(0.1);
	std::cout << "-----Boruta-----" << std::endl;
	borutaForest.getAttrsWeight(data, decisionAttr);
}

void runUserTextInterface() {
	std::cout << "           ------------ START ------------" << std::endl;
	while (true) {
		std::cout << "------------ Koronacki / Boruta Algorithm ------------" << std::endl;
		std::cout << "Please choose data type to load (number): " << std::endl;
		std::cout << "1. Adults" << std::endl;
		std::cout << "2. Flags" << std::endl;
		int typeNr;
		std::cin >> typeNr;
		ParsedData<std::string> data = parseData(typeNr);
		std::cout << "Please enter index of decision attribute (number from 0): " << std::endl;
		int decision_attr;
		std::cin >> decision_attr;
		std::cout << "Please choose algorithm type to start (number): " << std::endl;
		std::cout << "1. Koronacki algorithm" << std::endl;
		std::cout << "2. Boruta algorithm" << std::endl;
		int algorithmNr;
		std::cin >> algorithmNr;
		if (algorithmNr == 1)
			startKoronackiAlgorithm(data, decision_attr);
		else
			startBorutaAlgorithm(data, decision_attr);
		std::cout << "------------ Finished ------------" << std::endl;
	}
}

int main(int argc, char** argv) {
	
	// Load adult data
	/*std::vector<std::vector<std::string>> adultData = loadData(dataType.getFileName(dataType.adult));
	std::vector<DataHeader> adultHeaders = dataType.getHeaders(dataType.adult);
	ParsedData<std::string> adultParsedData = ParsedData<std::string>(adultData, adultHeaders);

	std::vector<std::string> row = adultParsedData.getRow(1);
	row[0] = "eloelo";
	adultParsedData.setRow(1, row);

	std::vector<std::string> column = adultParsedData.getColumn(0);
	column[0] = "tralala";
	adultParsedData.setColumn(0, column);*/

	// Load flag data - learn and test
	/*
	std::vector<std::vector<std::string>> flagLearnData = loadData("flag-learn.txt");
	std::vector<std::vector<std::string>> flagTestData = loadData("flag-test.txt");
	std::vector<DataHeader> flagHeaders = dataType.getHeaders(dataType.flag);
	ParsedData<std::string> flagLearnParsedData = ParsedData<std::string>(flagLearnData, flagHeaders);
	ParsedData<std::string> flagTestParsedData = ParsedData<std::string>(flagTestData, flagHeaders);

	int decision_attr = 6;
	*/

	/*
	SprintTree tree = SprintTree(flagLearnParsedData, decision_attr, 0.1);

	std::cout << "Nodes number " << tree.nodes_number();
	std::cout << "\nLeafs number: " << tree.leafs_number();
	std::cout << "\nLet's classify first row of test data. Got class: " << tree.classify(flagTestParsedData.getRow(1)) << " while expected " << flagTestParsedData.getRow(1)[decision_attr];
	std::cout << "\nAccuracy on test set: " << tree.accuracy(flagTestParsedData);
	*/

	/*
	KoronackiForest koronackiForest = KoronackiForest(0.1);
	std::map<int, double> attrsWeight = koronackiForest.calculateAttrsWeight(flagLearnParsedData, flagTestParsedData, decision_attr);
	std::cout << "-----Koronacki-----" << std::endl;
	koronackiForest.printAttrsWeight(attrsWeight, flagLearnParsedData.getHeaders());

	BorutaForest borutaForest = BorutaForest(0.1);
	std::cout << "-----Boruta-----" << std::endl;
	borutaForest.getAttrsWeight(flagLearnParsedData, flagTestParsedData, decision_attr);
	*/

	// Load adult data - learn and test
	//int decision_attr = 13;
	//std::vector<std::vector<std::string>> adultLearnData = loadData("adult-learn.txt");
	//std::vector<std::vector<std::string>> adultTestData = loadData("adult-test.txt");
	//std::vector<DataHeader> adultHeaders = dataType.getHeaders(dataType.adult);
	//ParsedData<std::string> adultLearnParsedData = ParsedData<std::string>(adultLearnData, adultHeaders);
	//ParsedData<std::string> adultTestParsedData = ParsedData<std::string>(adultTestData, adultHeaders);

	// Load flags data - learn and test
	/*
	int decision_attr = 14;
	std::cout << "Loading adult" << std::endl;
	std::vector<std::vector<std::string>> adultData = loadData("adult.txt");
	std::vector<DataHeader> adultHeaders = dataType.getHeaders(dataType.adult);
	std::cout << "Parsing adult-learn" << std::endl;
	ParsedData<std::string> adultParsedData = ParsedData<std::string>(adultData, adultHeaders);

	std::cout << "Data size: " << adultParsedData.getData().size() << std::endl;
	
	KoronackiForest koronackiForest = KoronackiForest(0.1);
	std::map<int, double> attrsWeight = koronackiForest.calculateAttrsWeight(adultParsedData, decision_attr);
	std::cout << "-----Koronacki-----" << std::endl;
	koronackiForest.printAttrsWeight(attrsWeight, adultParsedData.getHeaders());

	BorutaForest borutaForest = BorutaForest(0.1);
	std::cout << "-----Boruta-----" << std::endl;
	borutaForest.getAttrsWeight(adultParsedData, decision_attr);
	*/
	runUserTextInterface();

	return 0;
}



