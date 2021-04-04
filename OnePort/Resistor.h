#pragma once
#include "IOnePort.h"
class Resistor :
    public IOnePort
{
    double resistance;
public:
    Resistor(unsigned id, unsigned port_plus, unsigned port_minus, double R);
    double equation(LinMath::LinVector&);
    void print(std::ostream&);
};

