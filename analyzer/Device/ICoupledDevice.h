#pragma once
#include "IDevice.h"

namespace Analyzer {
	namespace Device {
		/**
		 * \brief Coupled device abstract base class.
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
			 * \brief Constructor of the base class parameters.
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