// Nagyhazi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Network/Network.h"
#include "fmt/core.h"

int main()
{
    try {
        auto net = Analyzer::Network::loadFromStream(std::cin);

        // uhh this ugly, need to implement load error and solve error subclasses...
        try {
            auto solution = net->solve();

            for (auto& br : net->getBranches()) {
                std::cout << "Device #" << br->id << ":" << std::endl;
                std::cout << "  u = " << solution(br->voltageIndex(), 0) << std::endl;
                std::cout << "  i = " << solution(br->currentIndex(), 0) << std::endl;
            }
        }
        catch (const std::runtime_error& error) {
            std::cout << "non-regular network!" << std::endl;
            std::cout << error.what() << std::endl;
        }
    }
    catch (const std::runtime_error& error) {
        std::cout << "Loading failed!" << std::endl;
        std::cout << error.what() << std::endl;
    }


}