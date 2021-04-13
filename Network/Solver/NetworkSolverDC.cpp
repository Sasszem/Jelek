#include "NetworkSolverDC.h"

using Analyzer::Network::Solvers::NetworkSolverDC;

NetworkSolverDC::NetworkSolverDC()
{
}

LinMath::Matrix NetworkSolverDC::solve(LinMath::Matrix& eq) const
{
	// just solve the equation system by inverting the matrix

	return eq.subMatrix(0, 0, eq.rows, eq.columns - 1).invert() * eq.subMatrix(0, eq.columns - 1, eq.rows, 1);
}

std::string NetworkSolverDC::print() const
{
	return "NetworkSolverDC()";
}
