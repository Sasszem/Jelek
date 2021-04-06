#pragma once
#include "ICoupledDevice.h"

class CCCS :
    public ICoupledDevice
{
    const double currGain;
public:
    CCCS(unsigned, unsigned, unsigned, unsigned, double);
    double equation(LinMath::Matrix& eq);
    std::string print();
};

