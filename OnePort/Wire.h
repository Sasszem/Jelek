#pragma once
#include "IOnePort.h"
class Wire :
    public IOnePort
{
public:
    Wire(unsigned id, unsigned port_plus, unsigned port_minus);
    double equation(LinMath::LinVector&);
    void print(std::ostream&);
};

