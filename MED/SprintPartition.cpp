#include "SprintPartition.h"

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

SprintPartition::SprintPartition()
{
}


SprintPartition::~SprintPartition()
{
}
