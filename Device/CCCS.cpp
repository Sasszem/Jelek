#include "CCCS.h"
#include "../fmt/core.h"

CCCS::CCCS(unsigned id, unsigned plus, unsigned minus, unsigned other, double gain) : ICoupledDevice(id, plus, minus, other), currGain(gain)
{
}

double CCCS::equation(LinMath::LinVector& vec)
{
	vec[currentIndexFromId(coupledId)] = currGain;
	vec[currentIndex()] = -1.0;
	return 0.0;
}

std::string CCCS::print()
{
	return fmt::format("CCCS(id={}, plus={}, minus={}, other={}, gain={})", id, port_plus, port_minus, coupledId, currGain);
}
