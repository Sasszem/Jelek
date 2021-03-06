#pragma once
#include "IDevice.h"

namespace Analyzer {
	namespace Device {
		/**
		 * \brief Probe / break / 0-current source.
		 * 
		 * Parameter: none
		 * Equation: i=0
		 */
		class Probe :
			public IDevice
		{
		public:
			/**
			 * \brief Initialize new probe.
			 * 
			 * \param id
			 * \param port_plus
			 * \param port_minus
			 */
			Probe(unsigned id, unsigned port_plus, unsigned port_minus);

			/**
			 * \brief Write equation.
			 *
			 * See class description for equation, and IDevice::equation for details on this function
			 *
			 * \param eq
			 * \return
			 */
			double equation(LinMath::Matrix& eq);

			/**
			 * \brief Print this probe in human-readable format.
			 *
			 * \return string representation of this probe
			 */
			std::string print();
		};
	}
}