#pragma once

#include <string>
#include "../LinMath/Matrix.h"

namespace Analyzer {
	namespace Network {
		class INetworkSolver
		{
		public:
			virtual LinMath::Matrix solve(LinMath::Matrix&) const = 0;
			virtual std::string print() const = 0;
			virtual std::string validate(unsigned N, unsigned B) = 0;
		};
	}
}