#include "Wire.h"
#include "../fmt/core.h"

using Analyzer::Device::Wire;
using Analyzer::Device::IDevice;

Wire::Wire(unsigned id, unsigned port_plus, unsigned port_minus): IDevice(id, port_plus, port_minus)
{
}

double Wire::equation(LinMath::Matrix& eq)
{
	eq(id-1, voltageIndex()) = 1.0;
	return 0.0;
}

std::string Wire::print()
{
	return fmt::format("Wire(id={}, plus={}, minus={})", id, port_plus, port_minus);
}
