#include "SprintTree.h"
#include "ParsedData.h"


SprintTree::SprintTree(int decision_attr_)
{
	decision_attr = decision_attr_;
}

SprintTree::SprintTree(ParsedData<std::string>& data, int decision_attr_, double gini_thr) {

	decision_attr = decision_attr_;

	double gini0 = gini(data, data);

	if (data.getData().size() < 2 || gini0 < gini_thr) {

		decision_class = biggest_class(data);
		decision.data1 = nullptr;
		decision.data2 = nullptr;
		decision.gini = gini0;
		decision.thr = "";
		decision.part_type = LEAF;
		Left = nullptr;
		Right = nullptr;
	}
	else {
		SprintPartition best_decision;
		best_decision.gini = 1;
		SprintPartition part;
		for (int i = 1; i < data.getHeaders().size(); ++i) {
			if (i != decision_attr) {
				if (data.getHeaders()[i].type == DISCRETE) {
					part = best_attr_discrete_partition(data, i);
					if (part.gini < best_decision.gini) {
						delete best_decision.data1;
						delete best_decision.data2;
						best_decision = part;
						best_decision.part_col = i;
					}
				}
				if (data.getHeaders()[i].type == NUMERIC) {
					part = best_attr_numeric_partition(data, i);
					if (part.gini < best_decision.gini) {
						delete best_decision.data1;
						delete best_decision.data2;
						best_decision = part;
						best_decision.part_col = i;
					}
				}
			}
		}
		decision = best_decision;
		decision.data1->setHeaders(data.getHeaders());
		decision.data2->setHeaders(data.getHeaders());
		Left = new SprintTree(*(decision.data1), decision_attr_, gini_thr);
		Right = new SprintTree(*(decision.data2), decision_attr_, gini_thr);
	}
}


SprintTree::~SprintTree()
{
}

std::string SprintTree::biggest_class(ParsedData<std::string>& data) {
	std::map<std::string, int> sizes = std::map<std::string, int>();
	for (int i = 0; i < data.getData().size(); ++i) {
		sizes[data.getRow(i)[decision_attr]] += 1;
	}
	int biggest_class = 0;
	std::string res = "";
	for (auto it = sizes.begin(); it != sizes.end(); ++it) {
		if (it->second > biggest_class) {
			biggest_class = it->second;
			res = it->first;
		}
	}
	return res;
}

SprintPartition SprintTree::best_attr_numeric_partition(ParsedData<std::string>& data, int col_num) {
	std::vector<int> col = std::vector<int>();
	for (int i = 0; i < data.getData().size(); ++i) {
		col.push_back(std::stoi(data.getColumn(col_num)[i]));
	}
	ParsedData<std::string> data1;
	ParsedData<std::string> data2;
	double best_thr = 0;
	double best_gini = 0;
	for (int i = 0; i < col.size(); ++i) {
		int thr = col[i];
		for (int j = 0; j < col.size(); ++j) {
			if (col[j] <= thr) {
				data1.getData().push_back(data.getData()[j]);
			}
			else {
				data2.getData().push_back(data.getData()[j]);
			}
		}
		if (data1.getData().size() == 0 || data2.getData().size() == 0)
			continue;
		double gini0 = gini(data1, data2);
		if (gini0 < best_gini) {
			best_gini = gini0;
			best_thr = thr;
		}
		data1.getData().clear();
		data2.getData().clear();
	}
	SprintPartition res;
	res.part_type = LE;
	res.thr = std::to_string(best_thr);
	res.gini = best_gini;
	ParsedData<std::string>* rdata1 = new ParsedData<std::string>();
	ParsedData<std::string>* rdata2 = new ParsedData<std::string>();
	for (int j = 0; j < col.size(); ++j) {
		if (col[j] <= best_thr) {
			rdata1->getData().push_back(data.getData()[j]);
		}
		else {
			rdata2->getData().push_back(data.getData()[j]);
		}
	}
	if (rdata1->getData().size() == 0 || rdata2->getData().size() == 0)
		res.gini = 1;
	res.data1 = rdata1;
	res.data2 = rdata2;
	return res;
}

SprintPartition SprintTree::best_attr_discrete_partition(ParsedData<std::string>& data, int col_num) {
	std::vector<std::string> vals = std::vector<std::string>();
	for (int i = 0; i < data.getData().size(); ++i) {
		if (std::find(vals.begin(), vals.end(), data.getColumn(col_num)[i]) == vals.end()) {
			vals.push_back(data.getColumn(col_num)[i]);
		}
	}
	ParsedData<std::string> data1;
	ParsedData<std::string> data2;
	std::string best_thr;
	double best_gini = 1;
	for (int i = 0; i < vals.size(); ++i) {
		std::string thr = vals[i];
		for (int j = 0; j < data.getData().size(); ++j) {
			if (data.getColumn(col_num)[j] == thr) {
				data1.getData().push_back(data.getData()[j]);
			}
			else {
				data2.getData().push_back(data.getData()[j]);
			}
		}
		if (data1.getData().size() == 0 || data2.getData().size() == 0)
			continue;
		double gini0 = gini(data1, data2);
		if (gini0 < best_gini) {
			best_gini = gini0;
			best_thr = thr;
		}
		data1.getData().clear();
		data2.getData().clear();
	}
	SprintPartition res;
	res.part_type = IN;
	res.thr = best_thr;
	res.gini = best_gini;
	ParsedData<std::string>* rdata1 = new ParsedData<std::string>();
	ParsedData<std::string>* rdata2 = new ParsedData<std::string>();
	for (int j = 0; j < data.getData().size(); ++j) {
		if (data.getColumn(col_num)[j] == best_thr) {
			rdata1->getData().push_back(data.getData()[j]);
		}
		else {
			rdata2->getData().push_back(data.getData()[j]);
		}
	}
	if (rdata1->getData().size() == 0 || rdata2->getData().size() == 0)
		res.gini = 1;
	res.data1 = rdata1;
	res.data2 = rdata2;
	return res;
}

double SprintTree::gini(ParsedData<std::string>& set1, ParsedData<std::string>& set2) {
	std::map<std::string, int> sizes1 = std::map<std::string, int>();
	std::map<std::string, int> sizes2 = std::map<std::string, int>();
	for (int i = 0; i < set1.getData().size(); ++i) {
		sizes1[set1.getRow(i)[decision_attr]] += 1;
	}
	for (int i = 0; i < set2.getData().size(); ++i) {
		sizes2[set2.getRow(i)[decision_attr]] += 1;
	}
	double sizes1sum = set1.getData().size();
	double sizes2sum = set2.getData().size();
	double gini1 = 0; double gini2 = 0;
	for (auto i = sizes1.begin(); i != sizes1.end(); ++i) {
		gini1 += std::pow(i->second / sizes1sum, 2);
	}
	for (auto i = sizes2.begin(); i != sizes2.end(); ++i) {
		gini2 += std::pow(i->second / sizes2sum, 2);
	}
	gini1 = 1 - gini1;
	gini2 = 1 - gini2;
	double sum = sizes1sum + sizes2sum;
	return (gini1 * sizes1sum / sum) + (gini2 * sizes2sum / sum);
}

std::string SprintTree::classify(std::vector<std::string>& row) {
	if (decision.part_type == LEAF) {
		return decision_class;
	}
	else if (decision.part_type == IN) {
		if (row[decision.part_col] == decision.thr) {
			return Left->classify(row);
		}
		else {
			return Right->classify(row);
		}
	}
	else {
		if (row[decision.part_col] <= decision.thr) {
			return Left->classify(row);
		}
		else {
			return Right->classify(row);
		}
	}
}

double SprintTree::accuracy(ParsedData<std::string>& data) {
	double pos = 0;
	for (int i = 0; i < data.getData().size(); ++i) {
		if (classify(data.getRow(i)) == data.getRow(i)[decision_attr]) {
			pos += 1;
		}
	}
	return pos / data.getData().size();
}

int SprintTree::nodes_number() {
	if (decision.part_type == LEAF)
		return 1;
	else
		return 1 + Left->nodes_number() + Right->nodes_number();
}

int SprintTree::leafs_number() {
	if (decision.part_type == LEAF)
		return 1;
	else
		return Left->leafs_number() + Right->leafs_number();
}
