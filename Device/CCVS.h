
#include "ICoupledDevice.h"

namespace Analyzer {
	namespace Device {
        class CCVS :
            public ICoupledDevice
        {
            const double transferRes;
        public:
            CCVS(unsigned, unsigned, unsigned, unsigned, double);
            double equation(LinMath::Matrix& eq);
            std::string print();
        };
	}
}
