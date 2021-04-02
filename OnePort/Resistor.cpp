#include "Resistor.h"
#include "../fmt/core.h"

Resistor::Resistor(unsigned id, unsigned port_plus, unsigned port_minus, double R): IOnePort(id, port_plus, port_minus), resistance(R)
{
}

double Resistor::equation(LinMath::LinVector& vector)
{
	vector[voltageIndex()] = -1.0;
	vector[currentIndex()] = resistance;
	return 0.0;
}

IOnePort* Resistor::copy()
{
	return new Resistor(*this);
}

void Resistor::print(std::ostream& stream)
{
	stream << fmt::format("Resistor(id={}, plus={}, minus={}, resistance={})", id, port_plus, port_minus, resistance) << std::endl;
}
