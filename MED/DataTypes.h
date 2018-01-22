#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

enum DATA_TYPE { DISCRETE, NUMERIC };

struct DataHeader {
	std::string header;
	DATA_TYPE type;
	DataHeader(std::string h, DATA_TYPE t) : header(h), type(t) {}
};

struct DataType {

	enum dataType {
		adult, flag, tumor
	};

	static const std::string getFileName(const DataType::dataType& type) {
		switch (type) {
		case adult: return "adult.txt";
		case flag: return "flag.txt";
		case tumor: return "tumor.txt";
		}
	}

	static const std::vector<DataHeader> getHeaders(const DataType::dataType& type) {
		switch (type) {
		case adult: {
			std::vector<DataHeader> headers{
				DataHeader("age", NUMERIC),
				DataHeader("workclass", DISCRETE),
				DataHeader("fnlwgt", NUMERIC),
				DataHeader("education", DISCRETE),
				DataHeader("education-num", NUMERIC),
				DataHeader("martial-status", DISCRETE),
				DataHeader("occupation", DISCRETE),
				DataHeader("relationship", DISCRETE),
				DataHeader("race", DISCRETE),
				DataHeader("sex", DISCRETE),
				DataHeader("capital-gain", NUMERIC),
				DataHeader("capital-loss", NUMERIC),
				DataHeader("hours-per-week", NUMERIC),
				DataHeader("native-country", DISCRETE),
				DataHeader("salary", DISCRETE)
			};
			return headers;
		}
		case flag: {
			std::vector<DataHeader> headers{
				DataHeader("name", DISCRETE),
				DataHeader("landmass", DISCRETE),
				DataHeader("zone", DISCRETE),
				DataHeader("area", NUMERIC),
				DataHeader("population", NUMERIC),
				DataHeader("language", DISCRETE),
				DataHeader("religion", DISCRETE),
				DataHeader("bars", NUMERIC),
				DataHeader("stripes", NUMERIC),
				DataHeader("colours", NUMERIC),
				DataHeader("red", DISCRETE),
				DataHeader("green", DISCRETE),
				DataHeader("blue", DISCRETE),
				DataHeader("gold", DISCRETE),
				DataHeader("white", DISCRETE),
				DataHeader("black", DISCRETE),
				DataHeader("orange", DISCRETE),
				DataHeader("mainhue", DISCRETE),
				DataHeader("circles", NUMERIC),
				DataHeader("crosses", NUMERIC),
				DataHeader("saltires", NUMERIC),
				DataHeader("quarters", NUMERIC),
				DataHeader("sunstars", NUMERIC),
				DataHeader("crescent", DISCRETE),
				DataHeader("triangle", DISCRETE),
				DataHeader("icon", DISCRETE),
				DataHeader("animate", DISCRETE),
				DataHeader("text", DISCRETE),
				DataHeader("topleft", DISCRETE),
				DataHeader("botright", DISCRETE)
			};
			return headers;
		}
		case tumor: {
			std::vector<DataHeader> headers{
				DataHeader("name", DISCRETE)
			};
			return headers;
		}
		}
	}
};

DataType dataType;