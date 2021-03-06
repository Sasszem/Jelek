#include "VoltageSource.h"
#include <fmt/core.h>

using Analyzer::Device::VoltageSource;
using Analyzer::Device::IDevice;

VoltageSource::VoltageSource(unsigned id, unsigned port_plus, unsigned port_minus, double V) : IDevice(id, port_plus, port_minus), voltage(V)
{
}

double VoltageSource::equation(LinMath::Matrix& eq)
{
	eq(id - 1, voltageIndex()) = 1.0;
	return voltage;
}

std::string VoltageSource::print()
{
	return fmt::format("VoltageSource(id={}, plus={}, minus={}, voltage={})", id, port_plus, port_minus, voltage);
}