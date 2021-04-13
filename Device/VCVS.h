#pragma once
#include "ICoupledDevice.h"

namespace Analyzer {
	namespace Device {
		class VCVS :
			public ICoupledDevice
		{
			const double voltageGain;
		public:
			VCVS(unsigned, unsigned, unsigned, unsigned, double);
			double equation(LinMath::Matrix& eq);
			std::string print();
		};
	}
}