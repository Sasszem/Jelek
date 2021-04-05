#pragma once
#include "ICoupledDevice.h"
class Nullator :
    public ICoupledDevice
{
public:
    Nullator(unsigned, unsigned, unsigned, unsigned);
    double equation(LinMath::Matrix& vec);
    std::string print();
};

