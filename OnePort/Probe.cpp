#include "Probe.h"
#include "../fmt/core.h"

Probe::Probe(unsigned id, unsigned port_plus, unsigned port_minus): IOnePort(id, port_plus, port_minus)
{
}

double Probe::equation(LinMath::LinVector& vector)
{
	vector[currentIndex()] = 1.0;
	return 0.0;
}

void Probe::print(std::ostream& stream)
{
	stream << fmt::format("Probe(id={}, plus={}, minus={})", id, port_plus, port_minus);
}
