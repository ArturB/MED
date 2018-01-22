#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include "ParsedData.h"

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
	for (int i = 0; i < v.size; ++i) {
		std::cout << v[i] << ", ";
	}
	std::cout << std::endl;
}

int main(int argc, char** argv) {
	
	// Load adult data
	std::vector<std::vector<std::string>> adultData = loadData(dataType.getFileName(dataType.adult));
	std::vector<DataHeader> adultHeaders = dataType.getHeaders(dataType.adult);
	ParsedData<std::string> adultParsedData = ParsedData<std::string>(adultData, adultHeaders);

	std::vector<std::string> row = adultParsedData.getRow(1);
	row[0] = "eloelo";
	adultParsedData.setRow(1, row);

	std::vector<std::string> column = adultParsedData.getColumn(0);
	column[0] = "tralala";
	adultParsedData.setColumn(0, column);

	// Load flag data
	std::vector<std::vector<std::string>> flagData = loadData(dataType.getFileName(dataType.flag));
	std::vector<DataHeader> flagHeaders = dataType.getHeaders(dataType.flag);
	ParsedData<std::string> flagParsedData = ParsedData<std::string>(flagData, flagHeaders);

	for (int i = 1; i < 10; ++i) {
		print_row(adultParsedData.getRow(i));
	}


	return 0;
}



