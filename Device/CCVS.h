
#include "ICoupledDevice.h"
class CCVS :
    public ICoupledDevice
{
    const double transferRes;
public:
    CCVS(unsigned, unsigned, unsigned, unsigned, double);
    double equation(LinMath::LinVector& vec);
    std::string print();
};
