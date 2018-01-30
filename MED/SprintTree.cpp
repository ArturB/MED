#include <algorithm>
#include "SprintTree.h"
#include "ParsedData.h"


SprintTree::SprintTree(int decision_attr_)
{
	decision_attr = decision_attr_;
}

SprintTree SprintTree::create(ParsedData<std::string>& data, int decision_attr_, double gini_thr) {
	std::vector<int> set;
	for (int i = 0; i < data.getData().size(); ++i) {
		set.push_back(i);
	}
	return SprintTree(data, set, decision_attr_, gini_thr);
}

void SprintTree::clear_subset_vals(ParsedData<std::string>& data, std::vector<int> set_) {
	for (int i = 0; i < set_.size(); ++i) {
		subset_1_attr_vals[data.getElem(set_[i], decision_attr)] = 0;
		subset_2_attr_vals[data.getElem(set_[i], decision_attr)] = 0;
	}
}

SprintTree::SprintTree(ParsedData<std::string>& data, std::vector<int> set_, int decision_attr_, double gini_thr) {
	decision_attr = decision_attr_;

	clear_subset_vals(data, set_);

	double gini0 = gini(data, set_, set_);

	//std::cout << "Calculating next node... Data size: " << set_.size() << ", total gini: " << gini0 << std::endl;

	if (set_.size() < 2 || gini0 < gini_thr) {

		decision_class = biggest_class(data, set_);
		decision.gini = gini0;
		decision.thr = "";
		decision.part_type = LEAF;
		Left = nullptr;
		Right = nullptr;

		//std::cout << "Reached a leaf!" << std::endl;

	}
	else {
		SprintPartition best_decision;
		best_decision.gini = 1.9;
		SprintPartition part;
		int checked_attr_num = std::floor(std::sqrt(data.getHeaders().size()));
		std::vector<int> checked_attrs;
		for (int i = 0; i < checked_attr_num; ++i) {
			int val;
			bool val_exists;
			do {
				val = std::rand() % checked_attr_num;
				val_exists = false;
				for (int j = 0; i < checked_attrs.size(); ++j) {
					if (checked_attrs[j] == val) {
						val_exists = true;
						break;
					}
				}
			} while (val_exists);
			checked_attrs.push_back(val);
		}
		for (int i = 1; i < checked_attr_num; ++i) {
			int h = checked_attrs[i];
			if (h != decision_attr) {
				//std::cout << "Checking column " << i << std::endl;
				if (data.getHeaders()[h].type == DISCRETE) {
					//std::cout << "Checking discrete column" << std::endl;
					part = best_attr_discrete_partition(data, set_, h);
					if (part.gini < best_decision.gini) {
						best_decision = part;
						best_decision.part_col = h;
					}
				}
				if (data.getHeaders()[h].type == NUMERIC) {
					//generate best part
					part = best_attr_numeric_partition(data, set_, h);
					if (part.gini < best_decision.gini) {
						best_decision = part;
						best_decision.part_col = h;
					}
				}
			}
		}

		// if elements are undistinguishable, there is a leaf
		if (best_decision.gini > 1) {
			decision_class = biggest_class(data, set_);
			decision.gini = gini0;
			decision.thr = "";
			decision.part_type = LEAF;
			Left = nullptr;
			Right = nullptr;
		}
		else {
			//std::cout << "Decision type made: " << printPartType(best_decision.part_type) << std::endl;
			decision = best_decision;

			Left = new SprintTree(data, decision.set1, decision_attr_, gini_thr);
			Right = new SprintTree(data, decision.set2, decision_attr_, gini_thr);
		}

	}
}


SprintTree::~SprintTree()
{
}

std::string SprintTree::biggest_class(ParsedData<std::string>& data, std::vector<int> set) {
	std::unordered_map<std::string, int> sizes = std::unordered_map<std::string, int>();
	for (int i = 0; i < set.size(); ++i) {
		sizes[data.getElem(set[i],decision_attr)] += 1;
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

//assumes that data are sorted by column col_num
SprintPartition SprintTree::best_attr_numeric_partition(ParsedData<std::string>& data, std::vector<int>& set, int col_num) {
	ParsedData<std::string> sorted;
	sorted.setHeaders(data.getHeaders());
	for (int i = 0; i < set.size(); ++i) {
		sorted.getData().push_back(data.getRow(set[i]));
	}

	std::sort(sorted.getData().begin(), sorted.getData().end(),
		[col_num](const std::vector<std::string>& a, const std::vector<std::string>& b) {
		return std::stoi(a[col_num]) > std::stoi(b[col_num]);
	});

	std::vector<int> set1 = std::vector<int>();
	std::vector<int> set2 = std::vector<int>();
	for (int i = 0; i < set.size(); ++i) {
		set2.push_back(i);
	}
	
	set1.push_back(set2.back());
	set2.pop_back();

	double best_thr = std::stoi(sorted.getElem(set1.back(),col_num));

	clear_subset_vals(data, set);
	for (int i = 0; i < set1.size(); ++i) {
		subset_1_attr_vals[data.getElem(set1[i], decision_attr)] += 1;
	}
	for (int i = 0; i < set2.size(); ++i) {
		subset_2_attr_vals[data.getElem(set2[i], decision_attr)] += 1;
	}

	double best_gini = gini(sorted, set1, set2);

	for (int i = 0; i < set.size() - 1; ++i) {
		
		if (sorted.getElem(set1.back(),col_num) == sorted.getElem(set2.back(),col_num)) {
			set1.push_back(set2.back());
			set2.pop_back();
			subset_1_attr_vals[data.getElem(set1.back(), decision_attr)] += 1;
			subset_2_attr_vals[data.getElem(set1.back(), decision_attr)] -= 1;
		}
		else {
			double gini0 = gini(sorted, set1, set2);
			if (gini0 < best_gini) {
				best_gini = gini0;
				best_thr = std::stoi(sorted.getElem(set1.back(),col_num));
			}
			set1.push_back(set2.back());
			set2.pop_back();
			subset_1_attr_vals[data.getElem(set1.back(), decision_attr)] += 1;
			subset_2_attr_vals[data.getElem(set1.back(), decision_attr)] -= 1;
		}
	}

	SprintPartition res;
	res.part_type = LE;
	res.thr = std::to_string(best_thr);
	res.gini = best_gini;

	set1.clear();
	set2.clear();

	for (int j = 0; j < set.size(); ++j) {
		if (std::stoi(data.getData()[set[j]][col_num]) <= best_thr) {
			set1.push_back(set[j]);
		}
		else {
			set2.push_back(set[j]);
		}
	}
	if (set1.size() == 0) {
		res.gini = 2;
	}
	if (set2.size() == 0)
		res.gini = 2;
	res.set1 = set1;
	res.set2 = set2;
	return res;
}

SprintPartition SprintTree::best_attr_discrete_partition(ParsedData<std::string>& data, std::vector<int>& set, int col_num) {
	std::vector<std::string> vals = std::vector<std::string>();
	for (int i = 0; i < set.size(); ++i) {
		if (std::find(vals.begin(), vals.end(), data.getElem(set[i],col_num)) == vals.end()) {
			vals.push_back(data.getElem(i, col_num));
		}
	}
	std::vector<int> set1;
	std::vector<int> set2;
	std::string best_thr;
	double best_gini = 1.1;
	for (int i = 0; i < vals.size(); ++i) {
		std::string thr = vals[i];
		//std::cout << "Checking discrete thr = " << thr << std::endl;
		for (int j = 0; j < set.size(); ++j) {
			if (data.getElem(set[j],col_num) == thr) {
				set1.push_back(j);
			}
			else {
				set2.push_back(j);
			}
		}

		clear_subset_vals(data, set);
		for (int i = 0; i < set1.size(); ++i) {
			subset_1_attr_vals[data.getElem(set1[i], decision_attr)] += 1;
		}
		for (int i = 0; i < set2.size(); ++i) {
			subset_2_attr_vals[data.getElem(set2[i], decision_attr)] += 1;
		}

		double gini0 = gini(data, set1, set2);
		if (gini0 < best_gini) {
			best_gini = gini0;
			best_thr = thr;
		}
	}
	SprintPartition res;
	res.part_type = IN;
	res.thr = best_thr;
	res.gini = best_gini;
	set1.clear();
	set2.clear();
	for (int j = 0; j < set.size(); ++j) {
		if (data.getElem(set[j],col_num) == best_thr) {
			set1.push_back(j);
		}
		else {
			set2.push_back(j);
		}
	}
	if (set1.size() == 0) {
		res.gini = 2;
	}
	if (set2.size() == 0) {
		res.gini = 2;
	}
	
	res.set1 = set1;
	res.set2 = set2;
	return res;
}

double SprintTree::gini(ParsedData<std::string>& data, std::vector<int>& set1, std::vector<int>& set2) {
	double sizes1sum = set1.size();
	double sizes2sum = set2.size();
	double gini1 = 0; double gini2 = 0;
	for (auto i = subset_1_attr_vals.begin(); i != subset_1_attr_vals.end(); ++i) {
		gini1 += std::pow(i->second / sizes1sum, 2);
	}
	for (auto i = subset_2_attr_vals.begin(); i != subset_2_attr_vals.end(); ++i) {
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
