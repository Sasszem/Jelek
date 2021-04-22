#pragma once
#include "IDevice.h"

namespace Analyzer {
	namespace Device {
		/**
		 * \brief Current source.
		 * 
		 * Parameter: current
		 * Equation: i=I0
		 */
		class CurrentSource :
			public IDevice
		{
			/// current of the source
			double current;
		public:
			/**
			 * \brief Initialize a new current source.
			 * 
			 * \param id
			 * \param port_plus
			 * \param port_minus
			 * \param current
			 */
			CurrentSource(unsigned id, unsigned port_plus, unsigned port_minus, double current);
			
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
			 * \brief Print this current source in human-readable format.
			 * 
			 * \return string representation of this current source
			 */
			std::string print();
		};
	}
}