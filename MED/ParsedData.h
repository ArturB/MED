#pragma once

#include <vector>
#include "DataTypes.h"

// klasa reprezentuj¹ca dane sparsowane z pliku CSV. Dane reprezentowane s¹ w formie macierzy 2D (wektor wektorów). 
// Mo¿liwy jest dowolny typ elementów, ale w praktyce korzystamy z parsowania do std::string. 
template<typename T>
class ParsedData
{
private:
	
	// nag³ówki wystêpuj¹cych w zbiorze danych atrybutów
	std::vector<DataHeader> headers;

	// sparsowane dane w formie macierzy 2D
	std::vector<std::vector<T>> data;

public:

	// domyœlny konstruktor
	ParsedData();

	// konstruktor pobieraj¹cy dane oraz ich nag³ówki
	ParsedData(std::vector<std::vector<T>> data, std::vector<DataHeader> headers);

	// zwraca nag³ówki danych
	std::vector<DataHeader>& getHeaders();

	// setter dla nag³ówków danych
	void setHeaders(std::vector<DataHeader>& headers_);

	// pobiera i-ty wiersz danych
	std::vector<T>& getRow(int i);

	// pozwala na dodanie nowego wiersza danych na koñcu 
	void addRow(const std::vector<T>& row);

	// zmienia i-ty wiersz danych
	void setRow(int i, const std::vector<T>& row);

	// usuwa i-ty wiersz danych
	void removeRow(int index);

	// pobiera i-t¹ kolumnê danych
	std::vector<T> getColumn(int i);

	// dodaje now¹ kolumnê danych na koñcu
	void addColumn(const std::vector<T>& column, DataHeader header);

	// zmienia i-t¹ kolumnê danych
	void setColumn(int i, const std::vector<T>& column);

	// pobiera element z wiersza row_num i kolumny col_num. 
	std::string getElem(int row_num, int col_num);

	// zwraca referencjê do ca³ego zbioru danych. 
	std::vector<std::vector<T>>& getData();

	// pozwala zmieniæ zbiór danych jako ca³oœæ. 
	void setData(std::vector<std::vector<std::string>>& data_);

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
void ParsedData<T>::addRow(const std::vector<T>& row) {
	data.push_back(row);
}

template<typename T>
void ParsedData<T>::setRow(int i, const std::vector<T>& row) {
	data[i] = row;
}

template<typename T>
void ParsedData<T>::removeRow(int index) {
	data.erase(data.begin() + index);
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
void ParsedData<T>::addColumn(const std::vector<T>& column, DataHeader header) {
	for (int r = 0; r < column.size(); ++r) {
		data[r].push_back(column[r]);
	}
	headers.push_back(header);
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
void ParsedData<T>::setData(std::vector<std::vector<std::string>>& data_) {
	data = data_;
}

template<typename T>
std::string ParsedData<T>::getElem(int row_num, int col_num) {
	return data[row_num][col_num];
}

template<typename T>
ParsedData<T>::~ParsedData()
{

}
