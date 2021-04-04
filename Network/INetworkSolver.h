#pragma once

#include "../LinMath/LinVector.h"
#include "../LinMath/LinearEquationSystem.h"

namespace Analyzer {
	namespace Network {
		class INetworkSolver
		{
		public:
			virtual LinMath::LinVector solve(LinMath::LinearEquationSystem&) const = 0;
			virtual std::string print() const = 0;
		};
	}
}
