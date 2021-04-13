#pragma once
#include "ICoupledDevice.h"

namespace Analyzer {
	namespace Device {
		class VCCS :
			public ICoupledDevice
		{
			const double transferConductance;
		public:
			VCCS(unsigned, unsigned, unsigned, unsigned, double);
			double equation(LinMath::Matrix& eq);
			std::string print();
		};
	}
}