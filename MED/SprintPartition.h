#pragma once

#include <string>
#include "ParsedData.h"

// typ decyzji podejmwowanej w w�le drzewa decyzyjnego: 
// - NO - brak decyzji, b��d oblicze�
// - IN - nale�enie do zbioru - atrybut typu dyskretnego
// - LE - relacja mniejszo�ci - atrybut typu numerycznego
// - LEAF - li�� drzewa decyzyjnego
enum PART_TYPE { NO, IN, LE, LEAF};

// Konwertuje typ decyzji podejmowanej w drzewie na stringa, kt�rego mo�na wypisa� na stdout np. w celach debugowych
std::string printPartType(PART_TYPE type);

// Decyzja podejmowana w w�le drzewa decyzyjnego. 
class SprintPartition
{
public:

	// typ decyzji
	PART_TYPE part_type;

	// indeks atrybutu na kt�rym wykonujemy por�wnanie
	int part_col;
	
	// warto�� progowa do kt�rej por�wnujemy
	std::string thr;

	// mapy intowe okre�laj�ce podzia� ca�ego zbioru danych na podzbiory
	std::vector<int> set1;
	std::vector<int> set2;

	// wsp. Giniego dla podzia�u generowanego przez decyzj�. 
	double gini;

	SprintPartition();
	~SprintPartition();
};

