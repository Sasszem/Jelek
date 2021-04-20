#pragma once
#include "IDevice.h"

namespace Analyzer {
	namespace Device {
		/**
		 * Resistor.
		 *
		 * Parameter: resistance
		 * Equation: iR - u = 0
		 */
		class Resistor :
			public IDevice
		{
			/// resistance (who would have quessed it?)
			double resistance;
		public:
			/**
			 * Initialize a new resistor.
			 *
			 * \param id
			 * \param port_plus
			 * \param port_minus
			 * \param R
			 */
			Resistor(unsigned id, unsigned port_plus, unsigned port_minus, double R);

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
			 * Print this resistor in human-readable format.
			 *
			 * \return string representation of this resistor
			 */
			std::string print();
		};
	}
}