#include "VoltageSource.h"
#include "../fmt/core.h"

VoltageSource::VoltageSource(unsigned id, unsigned port_plus, unsigned port_minus, double V): IOnePort(id, port_plus, port_minus), voltage(V)
{
}

double VoltageSource::equation(LinMath::LinVector& vector)
{
	vector[voltageIndex()] = 1.0;
	return voltage;
}

void VoltageSource::print(std::ostream& stream)
{
	stream << fmt::format("VoltageSource(id={}, plus={}, minus={}, voltage={})", id, port_plus, port_minus, voltage) << std::endl;
}