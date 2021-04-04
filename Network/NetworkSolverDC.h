#pragma once
#include "INetworkSolver.h"
class NetworkSolverDC :
    public INetworkSolver
{
public:
    NetworkSolverDC();
    LinMath::LinVector solve(LinMath::LinearEquationSystem&) const;
};

