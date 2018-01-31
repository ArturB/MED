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
	// pr�g wsp. Giniego poni�ej kt�rego tworzony jest li�� drzewa decyzjnego. 
	double gini_thr;

	//Zwraca wektor index�w atrybut�w z podanych danych
	std::vector<int> getCondAttrsIndexes(ParsedData<std::string>& dataSet, int decAttrIndex);

	//Zwraca A grup wierszy po T/A wierszy w ka�dej grupie. Wiersze s� losowona ze zwracaniem z podanych danyc.
	//Zwraca r�wnie� grup� niewylosowanych ani razu wierszy. Grupa ta to dane testuj�ce.
	//(T - liczba wierszy w danych, A - liczba atrybtut�w)
	std::pair<std::vector<ParsedData<std::string>>, ParsedData<std::string>> drawDataSets(ParsedData<std::string>& data, int nrOfRecords);

	//Losowa permutacja warto�ci w wierszach podanych anych dla podanego atrybutu 
	ParsedData<std::string> permutateDataSetAttr(ParsedData<std::string>& dataSet, int attrIndex);

	//Zwraca mapy z: klucz - index atrybutu, warto�� - dok�adno�� klasyfikacji dla danych testuj�cych i zpermutownaych danych testuj�cych
	//Parametry wej�ciowe: dane wej�ciowe, dane testuj�ce, atrybut decyzyjny.
	std::pair<std::map<int, double>, std::map<int, double>> getAttrsAccuracy(std::map<int, ParsedData<std::string>> attrsDataSets, ParsedData<std::string> trainingData, int decisionAttr);

public:
	// prosty konstruktor przyjmuj�cy pr�g wsp. Giniego, poni�ej kt�rego tworzony jest li�� drzewa decyzyjnego. 
	KoronackiForest(double gini_thr);

	//Wypisuje dla kolejnych atrybut�w ich dok�adno�ci klasyfikacyjne (zwy�e dane i zpermutowane) oraz ich wagi
	void printAttrsWeight(std::map<int, double> attrsWeight, std::map<int, double> attrsAcc, std::map<int, double> attrsPAcc, std::vector<DataHeader> headers);

	//Zwraca wagi kolejnych atrybut�w.
	//Waga jest liczona jako r�nica dok�adno�ci klasyfikacji dla danych i dok�adno�ci klsyfikacji dla danych im odpowiadaj�cym ze zpermutowanymi warto�ciami
	std::map<int, double> getAttrsWeight(std::map<int, double>& attrsAccuracy, std::map<int, double>& attrsAccuracyPermuted);

	// Zwraca wagi atrybut�w dla podanych danych i parameru decyzyjnego policzonych przy u�yciu algorytmu Koronackiego
	std::map<int, double> calculateAttrsWeight(ParsedData<std::string> data, int decisionAttr);

	~KoronackiForest();
};