#include "EquationSystemSolver.h"
#include <iostream>

Analyzer::Network::Solvers::EquationSystemSolver::EquationSystemSolver()
{
}

LinMath::Matrix Analyzer::Network::Solvers::EquationSystemSolver::solve(LinMath::Matrix& eq) const
{
	std::cout << "Extended matrix:" << std::endl;
	std::cout << eq;
	return LinMath::Matrix(0,0);
}

std::string Analyzer::Network::Solvers::EquationSystemSolver::print() const
{
	return "EquationSystemSolver()";
}
