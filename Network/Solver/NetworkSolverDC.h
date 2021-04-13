#pragma once
#include "../INetworkSolver.h"

namespace Analyzer {
	namespace Network {
		namespace Solvers {
			class NetworkSolverDC :
				public INetworkSolver
			{
			public:
				NetworkSolverDC();
				LinMath::Matrix solve(LinMath::Matrix&) const;
				std::string print() const;
			};
		}
	}
}