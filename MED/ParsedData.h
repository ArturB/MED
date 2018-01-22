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
	/** Opcjonalnie jakie� konstruktory */
	std::vector<T>& getHeaders();
	void setHeaders(const std::vector<T>& headers);
	std::vector<DATA_TYPE> getTypes();
	void setTypes(std::vector<const DATA_TYPE>& types);
	std::vector<T>& getRow(int i);
	void setRow(int i, const std::vector<T>& row);
	std::vector<T>& getColumn(int i);
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
std::vector<T>& ParsedData<T>::getTypes() {
	return types;
}

template<typename T>
void ParsedData<T>::setTypes(const std::vector<T>& types) {
	this->types = types;
}

template<typename T>
std::vector<T>& ParsedData<T>::getRow(int i) {

}

template<typename T>
void ParsedData<T>::setRow(int i, const std::vector<T>& row) {

}

template<typename T>
std::vector<T>& ParsedData<T>::getColumn(int i) {

}

template<typename T>
void ParsedData<T>::setColumn(int i, const std::vector<T>& column) {

}


template<typename T>
ParsedData<T>::~ParsedData()
{

}
