#pragma once
#include "ICoupledDevice.h"

namespace Analyzer {
	namespace Device {
		/**
		 * Current controlled current source.
		 *
		 * Parameter: current gain
		 * Equation: i - gain*i(other) = 0
		 */
		class CCCS :
			public ICoupledDevice
		{
			/// current gain
			const double currGain;
		public:
			/**
			 * Construct a new CCCS.
			 * 
			 * \param id
			 * \param plus
			 * \param minus
			 * \param other
			 * \param gain
			 */
			CCCS(unsigned id, unsigned plus, unsigned minus, unsigned other, double gain);
			
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