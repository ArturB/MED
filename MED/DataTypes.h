#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>


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

	static const std::vector<std::string> getHeaders(const DataType::dataType& type) {
		switch (type) {
		case adult: {
			std::vector<std::string> headers{
				"age",
				"workclass",
				"fnlwgt",
				"education",
				"education-num",
				"martial-status",
				"occupation",
				"relationship",
				"race",
				"sex",
				"capital-gain",
				"capital-loss",
				"hours-per-week",
				"native-country",
				"salary"
			};
			return headers;
		}
		case flag: {
			std::vector<std::string> headers{
				"name",
				"landmass",
				"zone",
				"area",
				"population",
				"language",
				"religion",
				"bars",
				"stripes",
				"colours",
				"red",
				"green",
				"blue",
				"gold",
				"white",
				"black",
				"orange",
				"mainhue",
				"circles",
				"crosses",
				"saltires",
				"quarters",
				"sunstars",
				"crescent",
				"triangle",
				"icon",
				"animate",
				"text",
				"topleft",
				"botright"
			};
			return headers;
		}
		case tumor: {
			std::vector<std::string> headers{
				"",
				""
			};
			return headers;
		}
		}
	}
};

DataType dataType;