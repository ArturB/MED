#pragma once

#include <string>
#include <numeric>
#include <map>
#include "ParsedData.h"
#include "SprintPartition.h"


class SprintTree
{
private:
	int decision_attr;
	std::string decision_class;
	SprintPartition decision;
	SprintTree* Left;
	SprintTree* Right;

	std::string biggest_class(ParsedData<std::string>& data) {
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


	SprintPartition best_attr_numeric_partition(ParsedData<std::string>& data, int col_num) {
		std::vector<int> col = std::vector<int>();
		for (int i = 0; i < data.getData().size(); ++i) {
			col.push_back(std::stoi(data.getColumn(col_num)[i]));
		}
		ParsedData<std::string>* data1;
		ParsedData<std::string>* data2;
		std::vector<int> part = std::vector<int>();
		std::vector<int> best_part = std::vector<int>();
		double best_thr = 0;
		double best_gini = 0;
		for (int i = 0; i < col.size(); ++i) {
			int thr = col[i];
			data1 = new ParsedData<std::string>();
			data2 = new ParsedData<std::string>();
			for (int j = 0; j < col.size(); ++j) {
				if (col[j] <= thr) {
					data1->getData().push_back(data.getData()[j]);
				}
				else {
					data2->getData().push_back(data.getData()[j]);
				}
			}
			if (data1->getData().size() == 0 || data2->getData().size() == 0)
				continue;
			//std::cout << "Data1 size = " << data1->getData().size() << ", data2 size = " << data2->getData().size() << std::endl;
			double gini0 = gini(*data1, *data2);
			if (gini0 < best_gini) {
				best_gini = gini0;
				best_part = part;
				best_thr = thr;
			}
			delete data1;
			delete data2;
		}
		SprintPartition res;
		res.part_type = LE;
		res.thr = std::to_string(best_thr);
		res.gini = best_gini;
		data1 = new ParsedData<std::string>();
		data2 = new ParsedData<std::string>();
		for (int j = 0; j < col.size(); ++j) {
			if (col[j] <= best_thr) {
				data1->getData().push_back(data.getData()[j]);
			}
			else {
				data2->getData().push_back(data.getData()[j]);
			}
		}
		if (data1->getData().size() == 0 || data2->getData().size() == 0)
			res.gini = 1;
		res.data1 = data1;
		res.data2 = data2;
		return res;
	}


	SprintPartition best_attr_discrete_partition(ParsedData<std::string>& data, int col_num) {
		std::vector<std::string> vals = std::vector<std::string>();
		for (int i = 0; i < data.getData().size(); ++i) {
			if (std::find(vals.begin(), vals.end(), data.getColumn(col_num)[i]) == vals.end()) {
				vals.push_back(data.getColumn(col_num)[i]);
			}
		}
		ParsedData<std::string>* data1;
		ParsedData<std::string>* data2;
		std::string best_thr;
		double best_gini = 1;
		for (int i = 0; i < vals.size(); ++i) {
			std::string thr = vals[i];
			data1 = new ParsedData<std::string>();
			data2 = new ParsedData<std::string>();
			for (int j = 0; j < data.getData().size(); ++j) {
				if (data.getColumn(col_num)[j] == thr) {
					data1->getData().push_back(data.getData()[j]);
				}
				else {
					data2->getData().push_back(data.getData()[j]);
				}
			}
			if (data1->getData().size() == 0 || data2->getData().size() == 0)
				continue;
			//std::cout << "Data1 size = " << data1->getData().size() << ", data2 size = " << data2->getData().size() << std::endl;
			double gini0 = gini(*data1, *data2);
			if (gini0 < best_gini) {
				best_gini = gini0;
				best_thr = thr;
			}
			delete data1;
			delete data2;
		}
		SprintPartition res;
		res.part_type = IN;
		res.thr = best_thr;
		res.gini = best_gini;
		data1 = new ParsedData<std::string>();
		data2 = new ParsedData<std::string>();
		for (int j = 0; j < data.getData().size(); ++j) {
			if (data.getColumn(col_num)[j] == best_thr) {
				data1->getData().push_back(data.getData()[j]);
			}
			else {
				data2->getData().push_back(data.getData()[j]);
			}
		}
		if (data1->getData().size() == 0 || data2->getData().size() == 0)
			res.gini = 1;
		res.data1 = data1;
		res.data2 = data2;
		return res;
	}



	/**
	* Calculate Gini index of sets partition. Assumes, that all sets are mutually exclusive.
	*/
	double gini(ParsedData<std::string>& set1, ParsedData<std::string>& set2) {
		std::map<std::string, int> sizes1 = std::map<std::string, int>();
		std::map<std::string, int> sizes2 = std::map<std::string, int>();
		for (int i = 0; i < set1.getData().size(); ++i) {
			sizes1[set1.getRow(i)[decision_attr]] += 1;
		}
		for (int i = 0; i < set2.getData().size(); ++i) {
			sizes2[set2.getRow(i)[decision_attr]] += 1;
		}
		double sizes1sum = 0;
		for (auto i = sizes1.begin(); i != sizes1.end(); ++i) {
			sizes1sum += i->second;
		}
		double sizes2sum = 0;
		for (auto i = sizes2.begin(); i != sizes2.end(); ++i) {
			sizes2sum += i->second;
		}
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

public:
	SprintTree(int decision_attr_);
	SprintTree(ParsedData<std::string>& data, int decision_attr_, double gini_thr) {

		//std::cout << "Calculating next node... Data size: " << data.getData().size() << std::endl;

		decision_attr = decision_attr_;

		double gini0 = gini(data, data);

		//std::cout << "Whole gini is " << gini0 << std::endl;

		if (data.getData().size() < 2 || gini0 < gini_thr) {

			decision_class = biggest_class(data);
			decision.data1 = nullptr;
			decision.data2 = nullptr;
			decision.gini = gini0;
			decision.thr = "";
			decision.part_type = LEAF;
			Left = nullptr;
			Right = nullptr;
			//std::cout << "We reached a leaf!" << std::endl;
		}
		else {
			SprintPartition best_decision;
			best_decision.gini = 1;
			SprintPartition part;
			for (int i = 1; i < data.getHeaders().size(); ++i) {
				//std::cout << "Analyzing column " << i << std::endl;
				if (i != decision_attr) {
					if (data.getHeaders()[i].type == DISCRETE) {
						part = best_attr_discrete_partition(data, i);
						//std::cout << "data1 size = " << part.data1->getData().size() << ", data2 size = " << part.data2->getData().size() << std::endl;
						if (part.gini < best_decision.gini) {
							delete best_decision.data1;
							delete best_decision.data2;
							best_decision = part;
							best_decision.part_col = i;
						}
					}
					if (data.getHeaders()[i].type == NUMERIC) {
						part = best_attr_numeric_partition(data, i);
						//std::cout << "data1 size = " << part.data1->getData().size() << ", data2 size = " << part.data2->getData().size() << std::endl;
						if (part.gini < best_decision.gini) {
							delete best_decision.data1;
							delete best_decision.data2;
							best_decision = part;
							best_decision.part_col = i;
						}
					}
					//std::cout << "Best gini = " << best_decision.gini << " on column " << best_decision.part_col << " of type " << best_decision.part_type << " with thr = " << best_decision.val << ", data1 size = " << best_decision.data1->getData().size() << ", data2 size = " << best_decision.data2->getData().size() << std::endl;
				}
			}
			decision = best_decision;
			decision.data1->setHeaders(data.getHeaders());
			decision.data2->setHeaders(data.getHeaders());
			/*std::cout << "Decision: type = " << decision.part_type 
				      << ", col = " << decision.part_col
				      << ", val = " << decision.val 
				      << ", gini = " << decision.gini 
				      << ", data1.size = " << decision.data1->getData().size() 
				      << ", data2.size = " << decision.data2->getData().size() 
				      << std::endl; */
			Left  = new SprintTree(*(decision.data1), decision_attr_, gini_thr);
			Right = new SprintTree(*(decision.data2), decision_attr_, gini_thr);
		}
	}

	std::string classify(std::vector<std::string>& row) {
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

	double accuracy(ParsedData<std::string>& data) {
		double pos = 0;
		for (int i = 0; i < data.getData().size(); ++i) {
			if (classify(data.getRow(i)) == data.getRow(i)[decision_attr]) {
				pos += 1;
			}
		}
		return pos / data.getData().size();
	}

	int nodes_number() {
		if (decision.part_type == LEAF)
			return 1;
		else
			return 1 + Left->nodes_number() + Right->nodes_number();
	}

	int leafs_number() {
		if (decision.part_type == LEAF)
			return 1;
		else
			return Left->leafs_number() + Right->leafs_number();
	}


	

	/**
	*
	*/
	
	~SprintTree();
};


