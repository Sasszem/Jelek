#include "NetworkSolverDC.h"

using Analyzer::Network::Solvers::NetworkSolverDC;

NetworkSolverDC::NetworkSolverDC()
{
}

LinMath::Matrix NetworkSolverDC::solve(LinMath::LinearEquationSystem& eq) const
{
	// just solve the equation system by inverting the matrix
	return eq.getMatrix().invert() * eq.getVector();
}

std::string NetworkSolverDC::print() const
{
	return "NetworkSolverDC()";
}
