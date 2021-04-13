// Nagyhazi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Network/net.h"
#include "fmt/core.h"

int main()
{
	try {
		auto net = Analyzer::Network::loadFromStream(std::cin);

		auto solution = net->solve();
			std::cout << solution;
	}
	catch (Analyzer::Network::LoadException error) {
		std::cout << "Loading failed!" << std::endl;
		std::cout << error.what() << std::endl;
	}
	catch (Analyzer::Network::SolveException error) {
		std::cout << "Could not solve network!" << std::endl;
		std::cout << error.what() << std::endl;
	}
	return 0;
}