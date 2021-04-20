#pragma once
#include "ICoupledDevice.h"

namespace Analyzer {
	namespace Device {
		/**
		 * Voltage controlled voltage source.
		 *
		 * Parameter: voltage gain
		 * Equation: u - gain*u(other) = 0
		 */
		class VCVS :
			public ICoupledDevice
		{
			/// gain
			const double voltageGain;
		public:
			/**
			 * Construct a new VCVS.
			 * 
			 * \param id
			 * \param plus
			 * \param minus
			 * \param other
			 * \param gain
			 */
			VCVS(unsigned id, unsigned plus, unsigned minus, unsigned other, double gain);

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