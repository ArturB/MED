#include "Parser.h"


// konstruktor domyœlny
Parser::Parser() {
}


Parser::~Parser() {
}

// Pobiera z pliku informacje o rekordach - nazwa i typ
std::vector<DataHeader> Parser::getHeaderLines(std::istream& str)
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
		if (cell == " NUMERIC")
			types.push_back(NUMERIC);
		else
			types.push_back(DISCRETE);
	}

	for (int i = 0; i < result.size(); ++i) {
		headers.push_back(DataHeader(result[i], types[i]));
	}

	return headers;
}

// Pobiera kolejne dane z wiersza (odzielone ",")
std::pair<bool, std::vector<std::string>> Parser::getNextLineAndSplitIntoTokens(std::istream& str)
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


// Pobiera dane z pliku (nag³ówki i dane) i zwraca je jako obiekt ParsedData
ParsedData<std::string> Parser::loadData(std::string file) {

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

// Zwraca zparsowane dane z pliku o podanej nazwie 
ParsedData<std::string> Parser::parseData(std::string fileName) {

	std::cout << "Loading & Parsing: " << fileName << std::endl;
	ParsedData<std::string> parsedData = loadData(fileName);

	std::cout << "Data size: " << parsedData.getData().size() << std::endl;

	return parsedData;
}