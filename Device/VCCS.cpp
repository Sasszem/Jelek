#include "VCCS.h"
#include "../fmt/core.h"

VCCS::VCCS(unsigned id, unsigned plus, unsigned minus, unsigned other, double transConductance) : ICoupledDevice(id, plus, minus, other), transferConductance(transConductance)
{
}

double VCCS::equation(LinMath::Matrix& eq)
{
	eq(id-1, voltageIndexFromId(coupledId)) = transferConductance;
	eq(id-1, currentIndex()) = -1.0;
	return 0.0;
}

std::string VCCS::print()
{
	return fmt::format("VCCS(id={}, plus={}, minus={}, other={}, gain={})", id, port_plus, port_minus, coupledId, transferConductance);
}