#pragma once
#include "ICoupledDevice.h"

namespace Analyzer {
    namespace Device {
        class Nullator :
            public ICoupledDevice
        {
        public:
            Nullator(unsigned, unsigned, unsigned, unsigned);
            double equation(LinMath::Matrix& eq);
            std::string print();
        };
    }
}

