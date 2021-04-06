#include "Probe.h"
#include "../fmt/core.h"

Probe::Probe(unsigned id, unsigned port_plus, unsigned port_minus): IDevice(id, port_plus, port_minus)
{
}

double Probe::equation(LinMath::Matrix& eq)
{
	eq(id-1, currentIndex()) = 1.0;
	return 0.0;
}

std::string Probe::print()
{
	return fmt::format("Probe(id={}, plus={}, minus={})", id, port_plus, port_minus);
}
