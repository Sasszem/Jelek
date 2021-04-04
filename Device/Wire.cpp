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

void Wire::print(std::ostream& stream)
{
	stream << fmt::format("Wire(id={}, plus={}, minus={})", id, port_plus, port_minus) << std::endl;
}
