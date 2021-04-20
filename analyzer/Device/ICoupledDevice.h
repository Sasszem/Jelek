#pragma once
#include "IDevice.h"

namespace Analyzer {
	namespace Device {
		/**
		 * Coupled device abstract base class.
		 * 
		 * Coupled devices can have equations depending on other devices parameters.
		 */
		class ICoupledDevice :
			public IDevice
		{
		protected:
			/// ID of device this device is coupled to
			const unsigned coupledId;
		public:

			/**
			 * Constructor of the base class parameters.
			 * 
			 * \param id
			 * \param plus
			 * \param minus
			 * \param otherId
			 */
			ICoupledDevice(unsigned id, unsigned plus, unsigned minus, unsigned otherId);
		};
	}
}