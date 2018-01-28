#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include "SprintTree.h"
#include "BorutaForest.h"

DataType dataType;

std::vector<DataHeader> getHeaderLines(std::istream& str)
{
	std::vector<DataHeader> headers;
	std::vector<std::string> result;
	std::vector<DATA_TYPE> types;
	std::string line;
	std::getline(str, line);

	std::stringstream lineStream(line);
	std::string cell;

	while (std::getline(lineStream, cell, ',')) {
		result.push_back(cell);
	}

	std::getline(str, line);
	std::stringstream lineTypeStream(line);

	while (std::getline(lineTypeStream, cell, ',')) {
		if (cell == "NUMERIC")
			types.push_back(NUMERIC);
		else
			types.push_back(DISCRETE);
	}

	for (int i = 0; i < result.size(); ++i) {
		headers.push_back(DataHeader(result[i], types[i]));
	}

	return headers;
}

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
		if ((cell == " ?") || (cell == "")) {
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

ParsedData<std::string> loadData(std::string file) {

	std::vector< std::vector<std::string> > result;
	std::vector<DataHeader> headers;
	ParsedData<std::string> data;
	std::ifstream is(file);

	if (!is.eof()) {
		headers = getHeaderLines(is);
	}

	while (!is.eof()) {
		std::pair<bool, std::vector<std::string>> newLine = getNextLineAndSplitIntoTokens(is);
		if (newLine.first) {
			result.push_back(newLine.second);
		}
	}
	is.close();

	data = ParsedData<std::string>(result, headers);

	return data;
}

template<typename T>
void print_row(const std::vector<T>& v) {
	for (int i = 0; i < v.size(); ++i) {
		std::cout << v[i] << ", ";
	}
	std::cout << std::endl;
}

ParsedData<std::string> parseData(std::string fileName) {

	std::cout << "Loading & Parsing: " << fileName << std::endl;
	ParsedData<std::string> parsedData = loadData(fileName);

	std::cout << "Data size: " << parsedData.getData().size() << std::endl;

	return parsedData;
}

void startKoronackiAlgorithm(ParsedData<std::string>& data, int decisionAttr) {
	KoronackiForest koronackiForest = KoronackiForest(0.1);
	std::cout << "-----Koronacki algorithm-----" << std::endl;
	std::map<int, double> attrsWeight = koronackiForest.calculateAttrsWeight(data, decisionAttr);
}

void startBorutaAlgorithm(ParsedData<std::string>& data, int decisionAttr) {
	BorutaForest borutaForest = BorutaForest(0.1);
	std::cout << "-----Boruta algorithm-----" << std::endl;
	borutaForest.getAttrsWeight(data, decisionAttr);
}

void runUserTextInterface(std::string filename, int decision_attr, int algorithmNr) {
	std::cout << "           ------------ START ------------" << std::endl;
	std::cout << "------------ Koronacki / Boruta Algorithm ------------" << std::endl;
	ParsedData<std::string> data = parseData(filename);
	std::cout << "Decision attribute: " << data.getHeaders()[decision_attr].header << std::endl;
	if (algorithmNr == 1)
		startKoronackiAlgorithm(data, decision_attr);
	else
		startBorutaAlgorithm(data, decision_attr);
	std::cout << "------------ Finished ------------" << std::endl;
}

int main(int argc, char** argv) {
	std::string filename;
	int decision_attr;
	int algorithmNr;

	filename = argv[1];
	decision_attr = strtol(argv[2], NULL, 10);
	algorithmNr = strtol(argv[3], NULL, 10);

	runUserTextInterface(filename, decision_attr, algorithmNr);

	return 0;
}



