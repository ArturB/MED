#include "KoronackiForest.h"


// prosty konstruktor przyjmuj¹cy próg wsp. Giniego, poni¿ej którego tworzony jest liœæ drzewa decyzyjnego. 
KoronackiForest::KoronackiForest(double gini_thr) {
	this->gini_thr = gini_thr;
}


KoronackiForest::~KoronackiForest()
{
}

//Zwraca wektor indexów atrybutów z podanych danych
std::vector<int> KoronackiForest::getCondAttrsIndexes(ParsedData<std::string>& dataSet, int decAttrIndex) {

	std::vector<int> condAttrs = std::vector<int>();

	for (int i = 0; i < dataSet.getRow(0).size(); ++i) {
			condAttrs.push_back(i);
	}

	return condAttrs;
}


//Zwraca A grup wierszy po T/A wierszy w ka¿dej grupie. Wiersze s¹ losowona ze zwracaniem z podanych danyc.
//Zwraca równie¿ grupê niewylosowanych ani razu wierszy. Grupa ta to dane testuj¹ce.
//(T - liczba wierszy w danych, A - liczba atrybtutów)
std::pair<std::vector<ParsedData<std::string>>, ParsedData<std::string>> KoronackiForest::drawDataSets(ParsedData<std::string>& data, int nrOfRecords) {

	std::vector<ParsedData<std::string>> drawedDataSets = std::vector<ParsedData<std::string>>();
	ParsedData<std::string> trainingData = ParsedData<std::string>(data.getData(), data.getHeaders());
	std::set<int> indexToRemove = std::set<int>();
	int dataSize = data.getData().size();

	for (int i = 0; i < nrOfRecords; ++i) {
		ParsedData<std::string> p = ParsedData<std::string>();
		p.setHeaders(data.getHeaders());
		drawedDataSets.push_back(p);
	}

	for (int i = 0; i < int(dataSize / nrOfRecords); ++i) {
		for (int j = 0; j < nrOfRecords; ++j) {
			int index = rand() % dataSize;
			std::vector<std::string> row = data.getRow(index);
			indexToRemove.insert(index);
			drawedDataSets[j].addRow(row);
		}
	}

	std::set<int>::reverse_iterator revIt = indexToRemove.rbegin();

	while (revIt != indexToRemove.rend()) {
		trainingData.removeRow(*revIt);
		revIt++;
	}

	std::pair<std::vector<ParsedData<std::string>>, ParsedData<std::string>> dataSets = std::pair<std::vector<ParsedData<std::string>>, ParsedData<std::string>>(drawedDataSets, trainingData);

	return dataSets;
}

//Losowa permutacja wartoœci w wierszach podanych anych dla podanego atrybutu 
ParsedData<std::string> KoronackiForest::permutateDataSetAttr(ParsedData<std::string>& dataSet, int attrIndex) {

	ParsedData<std::string> permutedDataSet = ParsedData<std::string>();
	permutedDataSet.setHeaders(dataSet.getHeaders());

	for (int i = 0; i < dataSet.getData().size(); ++i) {
		permutedDataSet.getData().push_back(dataSet.getData()[i]);
	}

	std::vector<std::string> permutedColumn = permutedDataSet.getColumn(attrIndex);
	int dataSize = permutedDataSet.getData().size();

	for (int i = dataSize; i >= 2; --i) {
		std::iter_swap(permutedColumn.begin() + (rand() % i), permutedColumn.begin() + (i - 1));
	}

	permutedDataSet.setColumn(attrIndex, permutedColumn);

	return permutedDataSet;
}


//Zwraca mapy z: klucz - index atrybutu, wartoœæ - dok³adnoœæ klasyfikacji dla danych testuj¹cych i zpermutownaych danych testuj¹cych
//Parametry wejœciowe: dane wejœciowe, dane testuj¹ce, atrybut decyzyjny.
std::pair<std::map<int, double>, std::map<int, double>> KoronackiForest::getAttrsAccuracy(std::map<int, ParsedData<std::string>> attrsDataSets, ParsedData<std::string> trainingData, int decisionAttr) {
	
	std::map<int, double> attrsAccuracy = std::map<int, double>();
	std::map<int, double> permAttrsAccuracy = std::map<int, double>();

	int h = 0;
	for (auto& x : attrsDataSets) {
		++h;
		std::cout << "Koronacki, calculating tree " << h << "/" << attrsDataSets.size() << std::endl;
		SprintTree tree = SprintTree::create(x.second, decisionAttr, gini_thr);
		attrsAccuracy[x.first] = tree.accuracy(trainingData);
		ParsedData<std::string> t = permutateDataSetAttr(trainingData, x.first);
		permAttrsAccuracy[x.first] = tree.accuracy(t);
		
	}

	std::pair<std::map<int, double>, std::map<int, double>> allAttrsAccuracy = std::pair<std::map<int, double>, std::map<int, double>>(attrsAccuracy, permAttrsAccuracy);
	return allAttrsAccuracy;
}


//Zwraca wagi kolejnych atrybutów.
//Waga jest liczona jako ró¿nica dok³adnoœci klasyfikacji dla danych i dok³adnoœci klsyfikacji dla danych im odpowiadaj¹cym ze zpermutowanymi wartoœciami
std::map<int, double> KoronackiForest::getAttrsWeight(std::map<int, double>& attrsAccuracy, std::map<int, double>& attrsAccuracyPermuted) {

	std::map<int, double> attrsWeight = std::map<int, double>();

	for (auto& x : attrsAccuracy) {
		attrsWeight[x.first] = x.second - attrsAccuracyPermuted[x.first];
	}

	return attrsWeight;
}

//Wypisuje dla kolejnych atrybutów ich dok³adnoœci klasyfikacyjne (zwy³e dane i zpermutowane) oraz ich wagi
void KoronackiForest::printAttrsWeight(std::map<int, double> attrsWeight, std::map<int, double> attrsAcc, std::map<int, double> attrsPAcc, std::vector<DataHeader> headers) {

	std::cout << "Attributes weights:" << std::endl;
	
	for (auto const& x : attrsWeight) {
		std::cout << headers[x.first].header << ": " << std::endl;
		std::cout << "   " << "accuracy - " << attrsAcc[x.first] << " / " << attrsPAcc[x.first] << std::endl;
		std::cout << "   " << "weights - " << x.second << std::endl;
	}
}


// Zwraca wagi atrybutów dla podanych danych i parameru decyzyjnego policzonych przy u¿yciu algorytmu Koronackiego
std::map<int, double> KoronackiForest::calculateAttrsWeight(ParsedData<std::string> data, int decisionAttr) {

	std::vector<int> condAttrsIndexes = getCondAttrsIndexes(data, decisionAttr);
	std::pair<std::vector<ParsedData<std::string>>, ParsedData<std::string>> dataSets = drawDataSets(data, condAttrsIndexes.size());
	std::vector<ParsedData<std::string>> drawedDataSets = dataSets.first;
	ParsedData<std::string> trainingData = dataSets.second;
	std::map<int, ParsedData<std::string>> drawedAttrsDataSets = std::map<int, ParsedData<std::string>>();

	for (int i = 0; i < condAttrsIndexes.size(); ++i) {
		int condAttrIndex = condAttrsIndexes[i];
		drawedAttrsDataSets[condAttrIndex] = drawedDataSets[i];
	}

	std::cout << "Trees: " << std::endl;
	std::pair<std::map<int, double>, std::map<int, double>> allAttrsAccuracy = getAttrsAccuracy(drawedAttrsDataSets, trainingData, decisionAttr);
	std::map<int, double> drawedAttrsAccuracy = allAttrsAccuracy.first;
	std::map<int, double> permutedAttrsAccuracy = allAttrsAccuracy.second;
	std::map<int, double> attrsWeight = getAttrsWeight(drawedAttrsAccuracy, permutedAttrsAccuracy);
	printAttrsWeight(attrsWeight, drawedAttrsAccuracy, permutedAttrsAccuracy, data.getHeaders());

	return attrsWeight;
}