#include <iostream>
#include <string>
#include <vector>
#include "ParsedData.h"

int main(int argc, char** argv) {

	ParsedData<std::string> d = ParsedData<std::string>();
	std::vector<std::string> hs = std::vector<std::string>();
	hs.push_back("header1");
	hs.push_back("header2");
	d.setHeaders(hs);

	std::cout << d.getHeaders()[0] << std::endl;


	return 0;
}