#pragma once
#include "../INetworkSolver.h"

namespace Analyzer {
	namespace Network {
		namespace Solvers {
			class NetworkSolverResistance :
				public INetworkSolver
			{
				const unsigned port;
			public:
				NetworkSolverResistance(unsigned);
				LinMath::Matrix solve(LinMath::Matrix&) const;
				std::string print() const;
				std::string validate(unsigned N, unsigned B);
			};
		}
	}
}