#pragma once

#include "../LinMath/LinearEquationSystem.h"
#include <string>

namespace Analyzer {
	namespace Network {
		class INetworkSolver
		{
		public:
			virtual LinMath::Matrix solve(LinMath::LinearEquationSystem&) const = 0;
			virtual std::string print() const = 0;
		};
	}
}
