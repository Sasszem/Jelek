#pragma once
#include "IDevice.h"
class CurrentSource :
    public IDevice
{
    double current;
public:
    CurrentSource(unsigned, unsigned, unsigned, double);
    double equation(LinMath::Matrix& vec);
    std::string print();
};

