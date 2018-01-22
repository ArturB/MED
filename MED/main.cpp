#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include "ParsedData.h"
#include "DataTypes.h"

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

int main(int argc, char** argv) {
	
	// Load adult data
	std::vector<std::vector<std::string>> adultData = loadData(dataType.getFileName(dataType.adult));
	std::vector<std::string> adultHeaders = dataType.getHeaders(dataType.adult);
	ParsedData<std::string> adultParsedData = ParsedData<std::string>(adultData, adultHeaders);

	// Load flag data
	std::vector<std::vector<std::string>> flagData = loadData(dataType.getFileName(dataType.flag));
	std::vector<std::string> flagHeaders = dataType.getHeaders(dataType.flag);
	ParsedData<std::string> flagParsedData = ParsedData<std::string>(flagData, flagHeaders);

	return 0;
}



