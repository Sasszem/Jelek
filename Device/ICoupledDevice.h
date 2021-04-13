#pragma once
#include "IDevice.h"

namespace Analyzer {
	namespace Device {
		class ICoupledDevice :
			public IDevice
		{
		protected:
			const unsigned coupledId;
		public:
			ICoupledDevice(unsigned, unsigned, unsigned, unsigned);
		};
	}
}
