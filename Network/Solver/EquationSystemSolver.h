#pragma once
#include "../INetworkSolver.h"
namespace Analyzer {
    namespace Network {
        namespace Solvers {
            class EquationSystemSolver :
                public INetworkSolver
            {
            public:
                EquationSystemSolver();
                LinMath::Matrix solve(LinMath::Matrix&) const;
                std::string print() const;
            };
        }
    }
}

