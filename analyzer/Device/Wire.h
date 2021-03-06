#pragma once
#include "IDevice.h"

namespace Analyzer {
	namespace Device {
		/**
		 * \brief Wire / short / 0-voltage source.
		 *
		 * Parameter: none
		 * Equation: u=0
		 */
		class Wire :
			public IDevice
		{
		public:
			/**
			 * \brief Initialize a new wire
			 *
			 * \param id
			 * \param port_plus
			 * \param port_minus
			 */
			Wire(unsigned id, unsigned port_plus, unsigned port_minus);

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
			 * \brief Print this wire in human-readable format.
			 *
			 * \return string representation of this wire
			 */
			std::string print();
		};
	}
}