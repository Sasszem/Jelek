#include "NetworkSolverGen.h"
#include "../fmt/core.h"

NetworkSolverGen::NetworkSolverGen(unsigned deviceId, double R1, double R2): R1(R1), R2(R2), deviceId(deviceId)
{
}

LinMath::LinVector NetworkSolverGen::solve(LinMath::LinearEquationSystem&) const
{
	std::cout << "generator solving not implemented yet!" << std::endl;
	return LinMath::LinVector(0);
}

std::string NetworkSolverGen::print() const
{
	return fmt::format("NetworkSolverGen(device={}, R1={}, R2={})", deviceId, R1, R2);
}

const double NetworkSolverGen::R1_DEFAULT = 1.0;
const double NetworkSolverGen::R2_DEFAULT = 1000000.0;