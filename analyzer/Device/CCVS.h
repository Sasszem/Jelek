
#include "ICoupledDevice.h"

namespace Analyzer {
	namespace Device {
		/**
		 * Current controlled voltage source.
		 *
		 * Parameter: transfer resistance
		 * Equation: u - tr*i(other) = 0
		 */
		class CCVS :
			public ICoupledDevice
		{
			/// transfer resistance
			const double transferRes;
		public:
			/**
			 * Construct a new CCVS.
			 * 
			 * \param id
			 * \param plus
			 * \param minus
			 * \param other
			 * \param transferRes
			 */
			CCVS(unsigned id, unsigned plus, unsigned minus, unsigned other, double transferRes);
			
			/**
			 * Write equation.
			 *
			 * See class description for equation, and IDevice::equation for details on this function
			 *
			 * \param eq
			 * \return
			 */
			double equation(LinMath::Matrix& eq);

			/**
			 * Print this device in human-readable format.
			 *
			 * \return string representation of this device
			 */
			std::string print();
		};
	}
}