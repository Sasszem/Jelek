#pragma once
#include "IDevice.h"

namespace Analyzer {
	namespace Device {
		/**
		 * \brief Resistor.
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
			 * \brief Initialize a new resistor.
			 *
			 * \param id
			 * \param port_plus
			 * \param port_minus
			 * \param R
			 */
			Resistor(unsigned id, unsigned port_plus, unsigned port_minus, double R);

			/**
			 * \brief Write equation.
			 *
			 * See class description for equation, and IDevice::equation for details on this function
			 *
			 * \param eq
			 * \return
			 */
			double equation(LinMath::Matrix&);

			/**
			 * \brief Print this resistor in human-readable format.
			 *
			 * \return string representation of this resistor
			 */
			std::string print();
		};
	}
}