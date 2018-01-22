#pragma once

#include <vector>

enum DATA_TYPE { DISCRETE, NUMERIC };

template<typename T>
class ParsedData
{
private:
	std::vector<T> headers;
	std::vector<DATA_TYPE> types;
	std::vector<std::vector<T>> data;

public:
	ParsedData(std::vector<std::vector<T>> data, std::vector<T> headers);
	/** Opcjonalnie jakieœ konstruktory */
	std::vector<T>& getHeaders();
	void setHeaders(const std::vector<T>& headers);
	std::vector<DATA_TYPE> getTypes();
	void setTypes(std::vector<const DATA_TYPE>& types);
	std::vector<T>& getRow(int i);
	void setRow(int i, const std::vector<T>& row);
	std::vector<T> getColumn(int i);
	void setColumn(int i, const std::vector<T>& column);
	

	~ParsedData();
};

template<typename T>
ParsedData<T>::ParsedData(std::vector<std::vector<T>> data, std::vector<T> headers)
{
	this->data = data;
	this->headers = headers;
}

template<typename T>
std::vector<T>& ParsedData<T>::getHeaders() {
	return headers;
}

template<typename T>
std::vector<DATA_TYPE> ParsedData<T>::getTypes() {
	return types;
}

template<typename T>
void ParsedData<T>::setTypes(std::vector<const DATA_TYPE>& types) {
	this->types = types;
}

template<typename T>
std::vector<T>& ParsedData<T>::getRow(int i) {
	return data[i];
}

template<typename T>
void ParsedData<T>::setRow(int i, const std::vector<T>& row) {
	data[i] = row;
}

template<typename T>
std::vector<T> ParsedData<T>::getColumn(int i) {
	std::vector<T> column;
	for (auto const& row: data) {
		column.push_back(row[i]);
	}
	return column;
}

template<typename T>
void ParsedData<T>::setColumn(int i, const std::vector<T>& column) {
	for (int r = 0; r < column.size(); ++r) {
		data[r][i] = column[r];
	}
}


template<typename T>
ParsedData<T>::~ParsedData()
{

}
