#pragma once
#include "../INetworkSolver.h"

namespace Analyzer {
	namespace Network {
		namespace Solvers {
			class EquationSystemSolver :
				public INetworkSolver
			{
				const unsigned printMode;
			public:
				const static unsigned MODE_DEFAULT = 1;
				EquationSystemSolver(unsigned mode);
				LinMath::Matrix solve(LinMath::Matrix&) const;
				std::string print() const;
			};
		}
	}
}

