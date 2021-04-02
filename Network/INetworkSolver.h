#pragma once

#include "../LinMath/LinVector.h"
class INetworkSolver
{
	virtual LinMath::LinVector solve() = 0;
};

