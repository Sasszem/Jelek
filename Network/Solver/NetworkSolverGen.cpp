#include "NetworkSolverGen.h"

#include "../../Device/Resistor.h"
#include "../../fmt/core.h"

#include <iostream>
using Analyzer::Network::Solvers::NetworkSolverGen;
using Analyzer::Device::Resistor;

NetworkSolverGen::NetworkSolverGen(unsigned deviceId, double R1, double R2): R1(R1), R2(R2), deviceId(deviceId)
{
}

LinMath::Matrix NetworkSolverGen::solve(LinMath::Matrix& eqOrig) const
{
	// return 3 numbers: V I R
	
	LinMath::Matrix eq = eqOrig;
	
	Resistor r1(deviceId, 0, 0, R1);
	Resistor r2(deviceId, 0, 0, R2);

	// remove the device's equations
	for (unsigned c = 0; c < eq.columns; c++)
		eq(deviceId - 1, c) = 0.0;

	double u1, u2, i1, i2;

	// set up as a resistor with resistance R1;
	r1.equation(eq);

	LinMath::Matrix result1 = eq.subMatrix(0, 0, eq.rows, eq.columns - 1).invert() * eq.subMatrix(0, eq.columns - 1, eq.rows, 1);
	u1 = result1(2 * deviceId - 2, 0);
	i1 = result1(2 * deviceId - 1, 0);

	r2.equation(eq);
	
	LinMath::Matrix result2 = eq.subMatrix(0, 0, eq.rows, eq.columns - 1).invert() * eq.subMatrix(0, eq.columns - 1, eq.rows, 1);
	u2 = result2(2 * deviceId - 2, 0);
	i2 = result2(2 * deviceId - 1, 0);
	

	// now we have 2 datapoints: (u1, i1) and (u2, i2)
	// we need to fit a line on them
	// and find it's tangent and the intersections with the axes


	// the equation: u = u0 - Ri
	// u1 = u0 - Ri1
	// u2 = u0 - Ri2
	// rearrange: (u1-u2) = R(i2-i1)
	// R = (u1-u2) / (i2-i1)

	double u0, i0, R;

	R = (u2 - u1) / (i1 - i2); // R

	// given R
	// u0 = u1 + Ri1
	u0 = u1 + R * i1; // U0

	// i0 = U0 / R
	i0 = u0 / R;
	
	// handle pathological case of R=inf
	if (u0 == INFINITY && (R == -INFINITY || R == INFINITY))
		i0 = i2;

	LinMath::Matrix generatorParameters(3, 1);

	generatorParameters(0, 0) = u0;
	generatorParameters(1, 0) = i0;
	generatorParameters(2, 0) = R;

	return generatorParameters;
}

std::string NetworkSolverGen::print() const
{
	return fmt::format("NetworkSolverGen(device={}, R1={}, R2={})", deviceId, R1, R2);
}

const double NetworkSolverGen::R1_DEFAULT = 1.0;
const double NetworkSolverGen::R2_DEFAULT = 1000000.0;
