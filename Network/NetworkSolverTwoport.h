#pragma once
#include "INetworkSolver.h"
class NetworkSolverTwoport :
    public INetworkSolver
{
    const unsigned primary, secondary;
public:
    NetworkSolverTwoport(unsigned, unsigned);
    LinMath::LinVector solve(LinMath::LinearEquationSystem&) const;
    std::string print() const;
};

