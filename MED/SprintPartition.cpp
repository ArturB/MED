#include "SprintPartition.h"

// Konwertuje typ decyzji podejmowanej w drzewie na stringa, kt�rego mo�na wypisa� na stdout np. w celach debugowych
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

// domy�lny konstruktor
SprintPartition::SprintPartition()
{
}


SprintPartition::~SprintPartition()
{
}
