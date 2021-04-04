#pragma once
#include "INetworkSolver.h"
class NetworkSolverGen :
    public INetworkSolver
{
    const double R1, R2;
    const unsigned deviceId;
public:
    NetworkSolverGen(unsigned, double, double);
    LinMath::LinVector solve(LinMath::LinearEquationSystem&) const;
    const static double R1_DEFAULT;
    const static double R2_DEFAULT;
};

