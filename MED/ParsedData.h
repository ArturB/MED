#pragma once

#include <vector>



template<typename T>
class ParsedData
{
private:
	std::vector<T> headers;
	std::vector<std::vector<T>> data;

public:
	ParsedData(std::vector<std::vector<T>> data, std::vector<T> headers);
	/** Opcjonalnie jakieœ konstruktory */
	std::vector<T>& getHeaders();
	void setHeaders(const std::vector<T>& headers);
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
void ParsedData<T>::setHeaders(const std::vector<T>& headers) {
	this->headers = headers;
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
