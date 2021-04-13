#include "NetworkSolverResistance.h"
#include "../../fmt/core.h"
#include "../../Device/CurrentSource.h"
#include "../SolveException.h"

using Analyzer::Device::CurrentSource;

Analyzer::Network::Solvers::NetworkSolverResistance::NetworkSolverResistance(unsigned device): port(device)
{
}

LinMath::Matrix Analyzer::Network::Solvers::NetworkSolverResistance::solve(LinMath::Matrix& eqOrig) const
{
	LinMath::Matrix eq = eqOrig;

	// remove the equation of our device
	for (unsigned c = 0; c < eq.columns; c++)
		eq(port - 1, c) = 0.0;

	// assert that terminating with a 0-current device, we have no voltage
	CurrentSource probe(port, 0, 0, 0.0);
	eq(port - 1, eq.columns - 1) = probe.equation(eq);
	double v0 = (eq.subMatrix(0, 0, eq.rows, eq.columns - 1).invert() * eq.subMatrix(0, eq.columns - 1, eq.rows, 1))(2 * port - 2, 0);

	if (v0 != 0.0) {
		throw Analyzer::Network::SolveException(fmt::format("Zero current voltage is not 0!"));
	}


	// voltage response of the network to a 1A stimulus is the resistance
	
	// take into care the direction of the current
	CurrentSource gen(port, 0, 0, - 1.0);
	eq(port - 1, eq.columns - 1) = gen.equation(eq);
	double res = (eq.subMatrix(0, 0, eq.rows, eq.columns - 1).invert() * eq.subMatrix(0, eq.columns - 1, eq.rows, 1))(2 * port - 2, 0);

	LinMath::Matrix result(1, 1);
	result(0, 0) = res;

	return result;
}

std::string Analyzer::Network::Solvers::NetworkSolverResistance::print() const
{
	return fmt::format("NetworkSolverResistance(port={})", port);
}

