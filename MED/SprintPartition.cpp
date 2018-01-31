#include "SprintPartition.h"

// Konwertuje typ decyzji podejmowanej w drzewie na stringa, którego mo¿na wypisaæ na stdout np. w celach debugowych
std::string printPartType(PART_TYPE type) {
	if (type == NO) {
		return "No partition";
	}
	else if (type == IN) {
		return "IN (discrete)";
	}
	else if (type == LE) {
		return "LE (numeric)";
	}
	else {
		return "LEAF";
	}
}

// domyœlny konstruktor
SprintPartition::SprintPartition()
{
}


SprintPartition::~SprintPartition()
{
}
