#pragma once

#include <string>
#include "ParsedData.h"

// typ decyzji podejmwowanej w wêŸle drzewa decyzyjnego: 
// - NO - brak decyzji, b³¹d obliczeñ
// - IN - nale¿enie do zbioru - atrybut typu dyskretnego
// - LE - relacja mniejszoœci - atrybut typu numerycznego
// - LEAF - liœæ drzewa decyzyjnego
enum PART_TYPE { NO, IN, LE, LEAF};

// Konwertuje typ decyzji podejmowanej w drzewie na stringa, którego mo¿na wypisaæ na stdout np. w celach debugowych
std::string printPartType(PART_TYPE type);

// Decyzja podejmowana w wêŸle drzewa decyzyjnego. 
class SprintPartition
{
public:

	// typ decyzji
	PART_TYPE part_type;

	// indeks atrybutu na którym wykonujemy porównanie
	int part_col;
	
	// wartoœæ progowa do której porównujemy
	std::string thr;

	// mapy intowe okreœlaj¹ce podzia³ ca³ego zbioru danych na podzbiory
	std::vector<int> set1;
	std::vector<int> set2;

	// wsp. Giniego dla podzia³u generowanego przez decyzjê. 
	double gini;

	SprintPartition();
	~SprintPartition();
};

