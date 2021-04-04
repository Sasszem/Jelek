// Nagyhazi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Network/Network.h"
#include "fmt/core.h"

int main()
{
    /*
    Network n(4, 6);

    Resistor R1(1, 3, 1, 10);
    n.addDevice(R1);
    Resistor R2(2, 2, 1, 20);
    n.addDevice(R2);
    Resistor R3(3, 4, 1, 10);
    n.addDevice(R3);
    Resistor R4(4, 3, 2, 30);
    n.addDevice(R4);
    Resistor R5(5, 4, 2, 20);
    n.addDevice(R5);
    VoltageSource V6(6, 3, 4, 5);
    n.addDevice(V6);
    */
    /*
    Network n(2, 3);
    VoltageSource v1(1, 1, 2, 5);
    n.addDevice(v1);
    CurrentSource c1(2, 1, 2, 0.1);
    n.addDevice(c1);
    Resistor r1(3, 1, 2, 50);
    n.addDevice(r1);

    n.printEquations();
    */
    auto net = loadFromStream(std::cin);
    try {
        auto solution = net->solve();

        for (auto& br : net->getBranches()) {
            std::cout << "Device #" << br->id << ":" << std::endl;
            std::cout << "  u = " << solution[br->voltageIndex()] << std::endl;
            std::cout << "  i = " << solution[br->currentIndex()] << std::endl;
        }
    }
    catch (const std::runtime_error& error) {
        std::cout << "non-regular network!" << std::endl;
        std::cout << error.what() << std::endl;
    }
}