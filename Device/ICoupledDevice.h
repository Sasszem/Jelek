#pragma once
#include "IDevice.h"
class ICoupledDevice :
    public IDevice
{
protected:
    const unsigned coupledId;
public:
    ICoupledDevice(unsigned, unsigned, unsigned, unsigned);
};

