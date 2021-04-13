#include "Resistor.h"
#include "../fmt/core.h"

using Analyzer::Device::Resistor;
using Analyzer::Device::IDevice;

Resistor::Resistor(unsigned id, unsigned port_plus, unsigned port_minus, double R) : IDevice(id, port_plus, port_minus), resistance(R)
{
}

double Resistor::equation(LinMath::Matrix& eq)
{
	eq(id - 1, voltageIndex()) = -1.0;
	eq(id - 1, currentIndex()) = resistance;
	return 0.0;
}


std::string Resistor::print()
{
	return fmt::format("Resistor(id={}, plus={}, minus={}, resistance={})", id, port_plus, port_minus, resistance);
}
