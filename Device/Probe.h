#pragma once
#include "IDevice.h"

namespace Analyzer {
    namespace Device {
        class Probe :
            public IDevice
        {
        public:
            Probe(unsigned id, unsigned port_plus, unsigned port_minus);
            double equation(LinMath::Matrix&);
            std::string print();
        };
    }
}

