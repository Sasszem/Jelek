#pragma once
#include "IDevice.h"
class Probe :
    public IDevice
{
public:
    Probe(unsigned id, unsigned port_plus, unsigned port_minus);
    double equation(LinMath::LinVector&);
    std::string print();
};

