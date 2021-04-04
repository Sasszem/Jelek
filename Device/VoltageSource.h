#pragma once
#include "IDevice.h"
class VoltageSource :
    public IOnePort
{
    double voltage;
public:
    VoltageSource(unsigned id, unsigned port_plus, unsigned port_minus, double V);
    double equation(LinMath::LinVector&);
    void print(std::ostream&);
};

