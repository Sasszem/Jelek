#pragma once

#include "../LinMath/LinVector.h"
#include "../LinMath/LinearEquationSystem.h"

class INetworkSolver
{
public:
	virtual LinMath::LinVector solve(LinMath::LinearEquationSystem&) const = 0;
};

