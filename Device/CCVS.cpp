#include "CCVS.h"
#include "../fmt/core.h"

CCVS::CCVS(unsigned id, unsigned plus, unsigned minus, unsigned other, double transferRes) : ICoupledDevice(id, plus, minus, other), transferRes(transferRes)
{
}

double CCVS::equation(LinMath::Matrix& vec)
{
	vec(id-1, currentIndexFromId(coupledId)) = transferRes;
	vec(id-1, voltageIndex()) = -1.0;
	return 0.0;
}

std::string CCVS::print()
{
	return fmt::format("CCVS(id={}, plus={}, minus={}, other={}, gain={})", id, port_plus, port_minus, coupledId, transferRes);
}
