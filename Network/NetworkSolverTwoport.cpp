#include "NetworkSolverTwoport.h"
#include "../fmt/core.h"

NetworkSolverTwoport::NetworkSolverTwoport(unsigned primaryID, unsigned secondaryID): primary(primaryID), secondary(secondaryID)
{
}

LinMath::LinVector NetworkSolverTwoport::solve(LinMath::LinearEquationSystem&) const
{
	std::cout << "Two port analysis not implemented yet!" << std::endl;
	return LinMath::LinVector(0);
}

std::string NetworkSolverTwoport::print() const
{
	return fmt::format("NetworkSolverTwoport(primary={}, secondary={})", primary, secondary);
}
