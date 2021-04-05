#include "Wire.h"
#include "../fmt/core.h"

Wire::Wire(unsigned id, unsigned port_plus, unsigned port_minus): IDevice(id, port_plus, port_minus)
{
}

double Wire::equation(LinMath::Matrix& vec)
{
	vec(id-1, voltageIndex()) = 1.0;
	return 0.0;
}

std::string Wire::print()
{
	return fmt::format("Wire(id={}, plus={}, minus={})", id, port_plus, port_minus);
}
