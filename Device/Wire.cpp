#include "Wire.h"
#include "../fmt/core.h"

Wire::Wire(unsigned id, unsigned port_plus, unsigned port_minus): IOnePort(id, port_plus, port_minus)
{
}

double Wire::equation(LinMath::LinVector& vec)
{
	vec[voltageIndex()] = 1.0;
	return 0.0;
}

std::string Wire::print()
{
	return fmt::format("Wire(id={}, plus={}, minus={})", id, port_plus, port_minus);
}
