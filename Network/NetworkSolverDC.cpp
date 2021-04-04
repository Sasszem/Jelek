#include "NetworkSolverDC.h"

NetworkSolverDC::NetworkSolverDC()
{
}

LinMath::LinVector NetworkSolverDC::solve(LinMath::LinearEquationSystem& eq) const
{
	return eq.getMatrix().invert() * eq.getVector();
}

std::string NetworkSolverDC::print() const
{
	return "NetworkSolverDC()";
}
