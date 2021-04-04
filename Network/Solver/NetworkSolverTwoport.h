#pragma once
#include "../INetworkSolver.h"

namespace Analyzer {
    namespace Network {
        namespace Solvers {
            class NetworkSolverTwoport :
                public INetworkSolver
            {
                const unsigned primary, secondary;
            public:
                NetworkSolverTwoport(unsigned, unsigned);
                LinMath::LinVector solve(LinMath::LinearEquationSystem&) const;
                std::string print() const;
            };
        }
    }
}
