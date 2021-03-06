#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include "Parser.h"
#include "SprintTree.h"
#include "BorutaForest.h"

template<typename T>
void print_row(const std::vector<T>& v) {
	for (int i = 0; i < v.size(); ++i) {
		std::cout << v[i] << ", ";
	}
	std::cout << std::endl;
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

// Wypisuje odpowiednie informacje
void runUserTextInterface(std::string filename, int decision_attr, int algorithmNr) {
	std::cout << "           ------------ START ------------" << std::endl;
	std::cout << "------------ Koronacki / Boruta Algorithm ------------" << std::endl;
	Parser parser = Parser();
	ParsedData<std::string> data = parser.parseData(filename);
	std::cout << "Decision attribute: " << data.getHeaders()[decision_attr].header << std::endl;
	if (algorithmNr == 1)
		startKoronackiAlgorithm(data, decision_attr);
	else
		startBorutaAlgorithm(data, decision_attr);
	std::cout << "------------ Finished ------------" << std::endl;
}

// ENTRY POINT
int main(int argc, char** argv) {

	srand(time(NULL));
	std::srand(std::time(0));

	std::string filename;
	int decision_attr;
	int algorithmNr;

	// Jezeli brak parametrów wejściowych ustawianie ich na domyślne wartości
	if (argc < 4) {
		filename = "adult.txt";
		decision_attr = 1;
		algorithmNr = 1;
	}
	else {
		filename = argv[1];
		decision_attr = strtol(argv[2], NULL, 10);
		algorithmNr = strtol(argv[3], NULL, 10);
	}

	runUserTextInterface(filename, decision_attr, algorithmNr);

	return 0;
}



