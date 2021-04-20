#pragma once
#include "IDevice.h"

namespace Analyzer {
	namespace Device {
		/**
		 * Voltage source.
		 *
		 * Parameter: voltage
		 * Equation: u=U0
		 */
		class VoltageSource :
			public IDevice
		{
			/// voltage of the source
			double voltage;
		public:
			/**
			 * Initialize a new voltage source.
			 *
			 * \param id
			 * \param port_plus
			 * \param port_minus
			 * \param V
			 */
			VoltageSource(unsigned id, unsigned port_plus, unsigned port_minus, double V);

			/**
			 * Write equation.
			 *
			 * See class description for equation, and IDevice::equation for details on this function
			 *
			 * \param eq
			 * \return
			 */
			double equation(LinMath::Matrix&);

			/**
			 * Print this voltage source in human-readable format.
			 *
			 * \return string representation of this voltage source
			 */
			std::string print();
		};
	}
}