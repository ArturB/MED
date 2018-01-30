#include "BorutaForest.h"



BorutaForest::BorutaForest(double gini_thr):koroneckiForest(gini_thr) {
	this->gini_thr = gini_thr;
}


BorutaForest::~BorutaForest()
{
}


//Zwraca dane z zrepliokwanymi kolumnami ka¿dego z atrybutów
ParsedData<std::string>& BorutaForest::replicateDataAttr(ParsedData<std::string>& data) {

	std::vector<DataHeader> headers = data.getHeaders();
	int rowSize = headers.size();
	int columnSize = data.getData().size();

	for (int i = 0; i < rowSize; ++i) {
		std::vector<std::string> column = data.getColumn(i);
		for (int j = columnSize; j >= 2; --j) {
			std::iter_swap(column.begin() + (rand() % j), column.begin() + (j - 1));
		}
		data.addColumn(column, headers[i]);
	}

	return data;
}

//Wypisuje ostateczne wagi policzone dla ka¿dego z atrybutów
void BorutaForest::printAttrsWeight(std::map<int, double> attrsWeight, std::vector<DataHeader> headers) {
	std::cout << "Attributes final weights:" << std::endl;

	for (auto const& x : attrsWeight) {
		std::cout << headers[x.first].header << " - " << x.second << std::endl;
	}
}

// Zwraca wagi atrybutów dla podanych danych i parameru decyzyjnego policzonych przy u¿yciu algorytmu Boruta
std::map<int, double> BorutaForest::getAttrsWeight(ParsedData<std::string> data, int decisionAttr) {

	int headerSize = data.getHeaders().size();
	ParsedData<std::string> replicatedData = replicateDataAttr(data);
	std::map<int, double> attrsWeightKor = koroneckiForest.calculateAttrsWeight(data, decisionAttr);
	std::map<int, double> attrsWeightNor = std::map<int, double>();
	std::map<int, double> attrsWeightRep = std::map<int, double>();
	for (auto const& x : attrsWeightKor) {
		
		if (x.first < headerSize) {
			attrsWeightNor[x.first] = x.second;
			attrsWeightRep[x.first + headerSize] = attrsWeightKor[x.first + headerSize];
		}
		else {
			break;
		}
	}

	std::map<int, double> attrsWeight = koroneckiForest.getAttrsWeight(attrsWeightNor, attrsWeightRep);
	printAttrsWeight(attrsWeight, data.getHeaders());

	return attrsWeight;
}
