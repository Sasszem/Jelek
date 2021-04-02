#pragma once
#include "IOnePort.h"
class Resistor :
    virtual public IOnePort
{
    double resistance;
public:
    Resistor(unsigned id, unsigned port_plus, unsigned port_minus, double R);
    double equation(LinMath::LinVector&);
    IOnePort* copy();
    void print(std::ostream&);
};

