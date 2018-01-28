#pragma once
#include <string>
#include "ParsedData.h"

class Parser
{
private:
	std::vector<DataHeader> getHeaderLines(std::istream& str);
	std::pair<bool, std::vector<std::string>> getNextLineAndSplitIntoTokens(std::istream& str);
	ParsedData<std::string> loadData(std::string file);

public:
	Parser();
	ParsedData<std::string> parseData(std::string fileName);

	~Parser();
};