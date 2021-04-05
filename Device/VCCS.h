#pragma once
#include "ICoupledDevice.h"
class VCCS :
    public ICoupledDevice
{
    const double transferConductance;
public:
    VCCS(unsigned, unsigned, unsigned, unsigned, double);
    double equation(LinMath::Matrix& vec);
    std::string print();
};

