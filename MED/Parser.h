#pragma once
#include <string>
#include "ParsedData.h"

// klasa obiektów parsuj¹cych pliki w formacie CSV
class Parser
{
private:

	// Pobiera z pliku informacje o rekordach - nazwa i typ
	std::vector<DataHeader> getHeaderLines(std::istream& str);

	// Pobiera kolejne dane z wiersza (odzielone ",")
	std::pair<bool, std::vector<std::string>> getNextLineAndSplitIntoTokens(std::istream& str);

	// Pobiera dane z pliku (nag³ówki i dane) i zwraca je jako obiekt ParsedData
	ParsedData<std::string> loadData(std::string file);

public:

	// konstruktor domyœlny
	Parser();

	// Zwraca zparsowane dane z pliku o podanej nazwie 
	ParsedData<std::string> parseData(std::string fileName);

	~Parser();
};