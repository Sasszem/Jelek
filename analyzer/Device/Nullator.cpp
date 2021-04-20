#include "Nullator.h"
#include <fmt/core.h>

using Analyzer::Device::Nullator;
using Analyzer::Device::ICoupledDevice;

Nullator::Nullator(unsigned id, unsigned pPlus, unsigned pMinus, unsigned noratorID) : ICoupledDevice(id, pPlus, pMinus, noratorID)
{
}

double Nullator::equation(LinMath::Matrix& eq)
{
	// hijack the equation of the other device
	// making it a norator
	// see the documentation on the file format for more details

	// clean it up, as we can make no assumptions on what it was before
	for (unsigned i = 0; i < eq.columns; i++)
		eq(coupledId - 1, i) = 0.0;

	eq(coupledId - 1, currentIndex()) = 1.0;
	eq(id - 1, voltageIndex()) = 1.0;
	return 0.0;
}

std::string Nullator::print()
{
	return fmt::format("Nullator(id={}, plus={}, minus={}, norator={})", id, port_plus, port_minus, coupledId);
}
