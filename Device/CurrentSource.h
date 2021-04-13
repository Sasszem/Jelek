#pragma once
#include "IDevice.h"

namespace Analyzer {
    namespace Device {
        class CurrentSource :
            public IDevice
        {
            double current;
        public:
            CurrentSource(unsigned, unsigned, unsigned, double);
            double equation(LinMath::Matrix& eq);
            std::string print();
        };
    }
}

