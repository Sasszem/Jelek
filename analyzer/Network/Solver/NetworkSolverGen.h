#pragma once
#include "../INetworkSolver.h"

namespace Analyzer {
	namespace Network {
		namespace Solvers {
			class NetworkSolverGen :
				public INetworkSolver
			{
				const double R1, R2;
				const unsigned deviceId;
			public:
				NetworkSolverGen(unsigned, double, double);
				LinMath::Matrix solve(LinMath::Matrix&) const;
				std::string print() const;
				std::string validate(unsigned N, unsigned B);
				const static double R1_DEFAULT;
				const static double R2_DEFAULT;
			};
		}
	}
}