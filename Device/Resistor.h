#pragma once
#include "IDevice.h"

namespace Analyzer {
    namespace Device {
        class Resistor :
            public IDevice
        {
            double resistance;
        public:
            Resistor(unsigned id, unsigned port_plus, unsigned port_minus, double R);
            double equation(LinMath::Matrix&);
            std::string print();
        };
    }
}


