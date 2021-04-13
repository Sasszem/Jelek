// Nagyhazi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Network/net.h"
#include "fmt/core.h"

int main(int argc, char**argv)
{
	std::string fname;

	if (argc > 2) {
		std::cout << "Usage:" << std::endl;
		std::cout << argv[0] << " [filename or - for CIN, defaults to -]" << std::endl;
		return -1;
	}
	if (argc == 1) {
		fname = "-";
	}
	else {
		fname = argv[1];
	}
	try {
		auto net = Analyzer::Network::loadFromFile(fname);

		auto solution = net->solve();
		if (solution.rows && solution.columns)
		std::cout << solution;
	}
	catch (Analyzer::Network::LoadException error) {
		std::cout << "Loading failed!" << std::endl;
		std::cout << error.what() << std::endl;
		return -1;
	}
	catch (Analyzer::Network::SolveException error) {
		std::cout << "Could not solve network!" << std::endl;
		std::cout << error.what() << std::endl;
		return -1;
	}
	return 0;
}