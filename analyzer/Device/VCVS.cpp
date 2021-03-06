#include "VCVS.h"
#include <fmt/core.h>

using Analyzer::Device::VCVS;
using Analyzer::Device::ICoupledDevice;

VCVS::VCVS(unsigned id, unsigned plus, unsigned minus, unsigned other, double gain) : ICoupledDevice(id, plus, minus, other), voltageGain(gain)
{
}

double VCVS::equation(LinMath::Matrix& eq)
{
	eq(id - 1, voltageIndexFromId(coupledId)) = voltageGain;
	eq(id - 1, voltageIndex()) = -1.0;
	return 0.0;
}

std::string VCVS::print()
{
	return fmt::format("VCVS(id={}, plus={}, minus={}, other={}, gain={})", id, port_plus, port_minus, coupledId, voltageGain);
}
