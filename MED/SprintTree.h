#pragma once

#include <string>
#include <numeric>
#include <unordered_map>
#include "ParsedData.h"
#include "SprintPartition.h"

// Drzewo decyzyjne tworzone algorytmem SPRINT. Cz�ciowo randomizowane, poprzez losowy wyb�r analizowanych atrybut�w tworzonych w ka�dym w�le. 
class SprintTree
{
private:
	// atrybut definiuj�cy klas� rekordu
	int decision_attr;

	// memoizacja podzia�u zbioru danych na podzbiory
	std::unordered_map<std::string, int> subset_1_attr_vals;
	std::unordered_map<std::string, int> subset_2_attr_vals;

	// nazwa tekstowa atrybutu klasy rekordu
	std::string decision_class;

	// typ i warunek podejmowanej w w�le decyzji
	SprintPartition decision;

	// lewe poddrzewo
	SprintTree* Left;

	//prawe poddrzewo
	SprintTree* Right;

	// wyczy�� podzia� na podzbiory
	void clear_subset_vals(ParsedData<std::string>&, std::vector<int>);

	// zwraca najliczniejsz� w zbiorze danych klas�. 
	std::string biggest_class(ParsedData<std::string>& data, std::vector<int>);

	// Wybiera najlepszy podzia� na podzbiory dla atrybutu typu numerycznego. 
	SprintPartition best_attr_numeric_partition(ParsedData<std::string>& data, std::vector<int>& set, int col_num);

	// Wybiera najlepszy podzia� dla atrybutu typu dyskretnego. 
	SprintPartition best_attr_discrete_partition(ParsedData<std::string>& data, std::vector<int>& set, int col_num);

	// oblicza wsp�czynnik Giniego, korzysta z liczno�ci klas w podzbiorach zachowanych w polach subset_1_attr_vals i subset_2_attr_vals.
	double gini(ParsedData<std::string>&, std::vector<int>&, std::vector<int>&);

	// prosty konstruktor przyjmuj�cy jedynie atrybut klasy. U�ywany w SprintTree::create. 
	SprintTree(int decision_attr_);

	// Rekurencyjny konstruktor tworz�cy drzewo decyzyjne. Wywo�ywany przez metod� statyczn� SprintTree::create. 
	SprintTree(ParsedData<std::string>& data, std::vector<int>, int decision_attr_, double gini_thr);
	

public:
	// Metoda statyczna tworz�ca drzewo decyzyjne na podstawie zbioru danych, indeksu atrybutu okre�laj�cego klas� rekordu oraz progu wsp. Giniego, poni�ej kt�rego twrozymy li�� drzewa decyzyjnego. 
	// powinna by� u�ywana jako w�a�ciwy kontruktor drzewa decyzyjnego
	static SprintTree create(ParsedData<std::string>& data, int decision_attr_, double gini_thr);

	// Klasyfikuje rekord na ju� utworzonym drzewie. 
	std::string classify(std::vector<std::string>& row);

	// Oblicza skuteczno�� klasyfikacji na zbiorze rekord�w testuj�cych. 
	double accuracy(ParsedData<std::string>& data);

	// Zwraca liczb� w�z��w w drzewie. 
	int nodes_number();

	// Zwraca liczb� li�ci w drzewie. 
	int leafs_number();

	~SprintTree();
};


