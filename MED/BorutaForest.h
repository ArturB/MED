#pragma once
#include "KoronackiForest.h"

// Las losowy tworzony algorytmem Boruta. 
class BorutaForest
{
private:
	// pr�g wsp. Giniego poni�ej kt�rego tworzymy li�� drzewa decyzyjnego
	double gini_thr;

	// zagnie�d�ony las losowy
	KoronackiForest koroneckiForest;

	//Zwraca dane z zrepliokwanymi kolumnami ka�dego z atrybut�w
	ParsedData<std::string>& replicateDataAttr(ParsedData<std::string>& data);

public:
	// prosty konstruktor przyjmuj�cy pr�g wsp. Giniego, poni�ej kt�rego tworzony jest li�� drzewa decyzyjnego. 
	BorutaForest(double gini_thr);

	//Wypisuje ostateczne wagi policzone dla ka�dego z atrybut�w
	void printAttrsWeight(std::map<int, double> attrsWeight, std::vector<DataHeader> headers);

	// Zwraca wagi atrybut�w dla podanych danych i parameru decyzyjnego policzonych przy u�yciu algorytmu Boruta
	std::map<int, double> getAttrsWeight(ParsedData<std::string> data, int decisionAttr);

	~BorutaForest();
};

