#pragma once
#include "IDevice.h"

namespace Analyzer {
	namespace Device {
		class VoltageSource :
			public IDevice
		{
			double voltage;
		public:
			VoltageSource(unsigned id, unsigned port_plus, unsigned port_minus, double V);
			double equation(LinMath::Matrix&);
			std::string print();
		};
	}
}