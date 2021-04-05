#include "NetworkSolverTwoport.h"
#include "../../fmt/core.h"
#include <iostream>

using Analyzer::Network::Solvers::NetworkSolverTwoport;

NetworkSolverTwoport::NetworkSolverTwoport(unsigned primaryID, unsigned secondaryID): primary(primaryID), secondary(secondaryID)
{
}

LinMath::Matrix NetworkSolverTwoport::solve(LinMath::Matrix&) const
{
	std::cout << "Two port analysis not implemented yet!" << std::endl;
	return LinMath::Matrix(0, 0);
}

std::string NetworkSolverTwoport::print() const
{
	return fmt::format("NetworkSolverTwoport(primary={}, secondary={})", primary, secondary);
}
