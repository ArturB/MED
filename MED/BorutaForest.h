#pragma once
#include "KoronackiForest.h"

// Las losowy tworzony algorytmem Boruta. 
class BorutaForest
{
private:
	// próg wsp. Giniego poni¿ej którego tworzymy liœæ drzewa decyzyjnego
	double gini_thr;

	// zagnie¿d¿ony las losowy
	KoronackiForest koroneckiForest;

	//Zwraca dane z zrepliokwanymi kolumnami ka¿dego z atrybutów
	ParsedData<std::string>& replicateDataAttr(ParsedData<std::string>& data);

public:
	// prosty konstruktor przyjmuj¹cy próg wsp. Giniego, poni¿ej którego tworzony jest liœæ drzewa decyzyjnego. 
	BorutaForest(double gini_thr);

	//Wypisuje ostateczne wagi policzone dla ka¿dego z atrybutów
	void printAttrsWeight(std::map<int, double> attrsWeight, std::vector<DataHeader> headers);

	// Zwraca wagi atrybutów dla podanych danych i parameru decyzyjnego policzonych przy u¿yciu algorytmu Boruta
	std::map<int, double> getAttrsWeight(ParsedData<std::string> data, int decisionAttr);

	~BorutaForest();
};

