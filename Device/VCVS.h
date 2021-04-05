#pragma once
#include "ICoupledDevice.h"
class VCVS :
    public ICoupledDevice
{
    const double voltageGain;
public:
    VCVS(unsigned, unsigned, unsigned, unsigned, double);
    double equation(LinMath::Matrix& vec);
    std::string print();
};

