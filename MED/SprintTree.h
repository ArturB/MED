#pragma once

#include <string>
#include <numeric>
#include <unordered_map>
#include "ParsedData.h"
#include "SprintPartition.h"

// Drzewo decyzyjne tworzone algorytmem SPRINT. Czêœciowo randomizowane, poprzez losowy wybór analizowanych atrybutów tworzonych w ka¿dym wêŸle. 
class SprintTree
{
private:
	// atrybut definiuj¹cy klasê rekordu
	int decision_attr;

	// memoizacja podzia³u zbioru danych na podzbiory
	std::unordered_map<std::string, int> subset_1_attr_vals;
	std::unordered_map<std::string, int> subset_2_attr_vals;

	// nazwa tekstowa atrybutu klasy rekordu
	std::string decision_class;

	// typ i warunek podejmowanej w wêŸle decyzji
	SprintPartition decision;

	// lewe poddrzewo
	SprintTree* Left;

	//prawe poddrzewo
	SprintTree* Right;

	// wyczyœæ podzia³ na podzbiory
	void clear_subset_vals(ParsedData<std::string>&, std::vector<int>);

	// zwraca najliczniejsz¹ w zbiorze danych klasê. 
	std::string biggest_class(ParsedData<std::string>& data, std::vector<int>);

	// Wybiera najlepszy podzia³ na podzbiory dla atrybutu typu numerycznego. 
	SprintPartition best_attr_numeric_partition(ParsedData<std::string>& data, std::vector<int>& set, int col_num);

	// Wybiera najlepszy podzia³ dla atrybutu typu dyskretnego. 
	SprintPartition best_attr_discrete_partition(ParsedData<std::string>& data, std::vector<int>& set, int col_num);

	// oblicza wspó³czynnik Giniego, korzysta z licznoœci klas w podzbiorach zachowanych w polach subset_1_attr_vals i subset_2_attr_vals.
	double gini(ParsedData<std::string>&, std::vector<int>&, std::vector<int>&);

	// prosty konstruktor przyjmuj¹cy jedynie atrybut klasy. U¿ywany w SprintTree::create. 
	SprintTree(int decision_attr_);

	// Rekurencyjny konstruktor tworz¹cy drzewo decyzyjne. Wywo³ywany przez metodê statyczn¹ SprintTree::create. 
	SprintTree(ParsedData<std::string>& data, std::vector<int>, int decision_attr_, double gini_thr);
	

public:
	// Metoda statyczna tworz¹ca drzewo decyzyjne na podstawie zbioru danych, indeksu atrybutu okreœlaj¹cego klasê rekordu oraz progu wsp. Giniego, poni¿ej którego twrozymy liœæ drzewa decyzyjnego. 
	// powinna byæ u¿ywana jako w³aœciwy kontruktor drzewa decyzyjnego
	static SprintTree create(ParsedData<std::string>& data, int decision_attr_, double gini_thr);

	// Klasyfikuje rekord na ju¿ utworzonym drzewie. 
	std::string classify(std::vector<std::string>& row);

	// Oblicza skutecznoœæ klasyfikacji na zbiorze rekordów testuj¹cych. 
	double accuracy(ParsedData<std::string>& data);

	// Zwraca liczbê wêz³ów w drzewie. 
	int nodes_number();

	// Zwraca liczbê liœci w drzewie. 
	int leafs_number();

	~SprintTree();
};


