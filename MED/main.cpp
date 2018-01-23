#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include "SprintTree.h"

DataType dataType;

std::vector<std::string> getNextLineAndSplitIntoTokens(std::istream& str)
{
	std::vector<std::string> result;
	std::string line;
	std::getline(str, line);

	std::stringstream lineStream(line);
	std::string cell;

	while (std::getline(lineStream, cell, ',')) {
		result.push_back(cell);
	}
	if (!lineStream && cell.empty()) {
		result.push_back("");
	}
	return result;
}

std::vector<std::vector<std::string>> loadData(std::string file) {

	std::vector< std::vector<std::string> > result;
	std::ifstream is(file);

	while (!is.eof()) {
		result.push_back(getNextLineAndSplitIntoTokens(is));
	}
	return result;
}

template<typename T>
void print_row(const std::vector<T>& v) {
	for (int i = 0; i < v.size(); ++i) {
		std::cout << v[i] << ", ";
	}
	std::cout << std::endl;
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
	std::vector<std::vector<std::string>> flagLearnData = loadData("flag-learn.txt");
	std::vector<std::vector<std::string>> flagTestData = loadData("flag-test.txt");
	std::vector<DataHeader> flagHeaders = dataType.getHeaders(dataType.flag);
	ParsedData<std::string> flagLearnParsedData = ParsedData<std::string>(flagLearnData, flagHeaders);
	ParsedData<std::string> flagTestParsedData = ParsedData<std::string>(flagTestData, flagHeaders);

	int decision_attr = 1;
	
	SprintTree tree = SprintTree(flagLearnParsedData, decision_attr, 0.1);

	std::cout << "Nodes number " << tree.nodes_number();
	std::cout << "\nLeafs number: " << tree.leafs_number();
	std::cout << "\nAccuracy on test set: " << tree.accuracy(flagTestParsedData);



	return 0;
}



