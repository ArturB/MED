#pragma once
#include <string>
#include <numeric>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <set>
#include "ParsedData.h"
#include "SprintTree.h"

// Las losowy tworzony algorytmem prof. Koronackiego. 
class KoronackiForest
{
private:
	// próg wsp. Giniego poni¿ej którego tworzony jest liœæ drzewa decyzjnego. 
	double gini_thr;

	//Zwraca wektor indexów atrybutów z podanych danych
	std::vector<int> getCondAttrsIndexes(ParsedData<std::string>& dataSet, int decAttrIndex);

	//Zwraca A grup wierszy po T/A wierszy w ka¿dej grupie. Wiersze s¹ losowona ze zwracaniem z podanych danyc.
	//Zwraca równie¿ grupê niewylosowanych ani razu wierszy. Grupa ta to dane testuj¹ce.
	//(T - liczba wierszy w danych, A - liczba atrybtutów)
	std::pair<std::vector<ParsedData<std::string>>, ParsedData<std::string>> drawDataSets(ParsedData<std::string>& data, int nrOfRecords);

	//Losowa permutacja wartoœci w wierszach podanych anych dla podanego atrybutu 
	ParsedData<std::string> permutateDataSetAttr(ParsedData<std::string>& dataSet, int attrIndex);

	//Zwraca mapy z: klucz - index atrybutu, wartoœæ - dok³adnoœæ klasyfikacji dla danych testuj¹cych i zpermutownaych danych testuj¹cych
	//Parametry wejœciowe: dane wejœciowe, dane testuj¹ce, atrybut decyzyjny.
	std::pair<std::map<int, double>, std::map<int, double>> getAttrsAccuracy(std::map<int, ParsedData<std::string>> attrsDataSets, ParsedData<std::string> trainingData, int decisionAttr);

public:
	// prosty konstruktor przyjmuj¹cy próg wsp. Giniego, poni¿ej którego tworzony jest liœæ drzewa decyzyjnego. 
	KoronackiForest(double gini_thr);

	//Wypisuje dla kolejnych atrybutów ich dok³adnoœci klasyfikacyjne (zwy³e dane i zpermutowane) oraz ich wagi
	void printAttrsWeight(std::map<int, double> attrsWeight, std::map<int, double> attrsAcc, std::map<int, double> attrsPAcc, std::vector<DataHeader> headers);

	//Zwraca wagi kolejnych atrybutów.
	//Waga jest liczona jako ró¿nica dok³adnoœci klasyfikacji dla danych i dok³adnoœci klsyfikacji dla danych im odpowiadaj¹cym ze zpermutowanymi wartoœciami
	std::map<int, double> getAttrsWeight(std::map<int, double>& attrsAccuracy, std::map<int, double>& attrsAccuracyPermuted);

	// Zwraca wagi atrybutów dla podanych danych i parameru decyzyjnego policzonych przy u¿yciu algorytmu Koronackiego
	std::map<int, double> calculateAttrsWeight(ParsedData<std::string> data, int decisionAttr);

	~KoronackiForest();
};