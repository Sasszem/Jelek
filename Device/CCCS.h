#pragma once
#include "ICoupledDevice.h"

namespace Analyzer {
	namespace Device {
		class CCCS :
			public ICoupledDevice
		{
			const double currGain;
		public:
			CCCS(unsigned, unsigned, unsigned, unsigned, double);
			double equation(LinMath::Matrix& eq);
			std::string print();
		};
	}
}