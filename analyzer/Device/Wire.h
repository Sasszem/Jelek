#pragma once
#include "IDevice.h"

namespace Analyzer {
	namespace Device {
		/**
		 * Wire / short / 0-voltage source.
		 *
		 * Parameter: none
		 * Equation: u=0
		 */
		class Wire :
			public IDevice
		{
		public:
			/**
			 * Initialize a new wire
			 *
			 * \param id
			 * \param port_plus
			 * \param port_minus
			 */
			Wire(unsigned id, unsigned port_plus, unsigned port_minus);

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
			 * Print this wire in human-readable format.
			 *
			 * \return string representation of this wire
			 */
			std::string print();
		};
	}
}