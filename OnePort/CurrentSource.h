#pragma once
#include "IOnePort.h"
class CurrentSource :
    public IOnePort
{
    double current;
public:
    CurrentSource(unsigned, unsigned, unsigned, double);
    IOnePort* copy();
    double equation(LinMath::LinVector& vec);
    void print(std::ostream&);
};

