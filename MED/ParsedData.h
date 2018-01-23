#pragma once

#include <vector>
#include "DataTypes.h"

template<typename T>
class ParsedData
{
private:
	std::vector<DataHeader> headers;
	std::vector<std::vector<T>> data;

public:
	ParsedData();

	ParsedData(std::vector<std::vector<T>> data, std::vector<DataHeader> headers);
	std::vector<DataHeader>& getHeaders();
	void setHeaders(std::vector<DataHeader>& headers_);
	std::vector<T>& getRow(int i);
	void setRow(int i, const std::vector<T>& row);
	std::vector<T> getColumn(int i);
	void setColumn(int i, const std::vector<T>& column);

	std::vector<std::vector<T>>& getData();

	~ParsedData();
};

template<typename T> ParsedData<T>::ParsedData() { }

template<typename T>
ParsedData<T>::ParsedData(std::vector<std::vector<T>> data, std::vector<DataHeader> headers)
{
	this->data = data;
	this->headers = headers;
}

template<typename T>
std::vector<DataHeader>& ParsedData<T>::getHeaders() {
	return headers;
}

template<typename T>
void ParsedData<T>::setHeaders(std::vector<DataHeader>& headers_) {
	headers = headers_;
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
	for (auto const& row : data) {
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
std::vector<std::vector<T>>& ParsedData<T>::getData() {
	return data;
}


template<typename T>
ParsedData<T>::~ParsedData()
{

}
