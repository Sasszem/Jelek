#pragma once
#include "IDevice.h"
class CurrentSource :
    public IOnePort
{
    double current;
public:
    CurrentSource(unsigned, unsigned, unsigned, double);
    double equation(LinMath::LinVector& vec);
    void print(std::ostream&);
};

