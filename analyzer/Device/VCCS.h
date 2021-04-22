#pragma once
#include "ICoupledDevice.h"

namespace Analyzer {
	namespace Device {
		/**
		 * \brief voltage controlled current source.
		 *
		 * Parameter: transfer conductance
		 * Equation: i - tr*u(other) = 0
		 */
		class VCCS :
			public ICoupledDevice
		{
			/// transfer conductance
			const double transferConductance;
		public:
			/**
			 * \brief Construct a new VCCS.
			 * 
			 * \param id
			 * \param plus
			 * \param minus
			 * \param other
			 * \param transConductance
			 */
			VCCS(unsigned id, unsigned plus, unsigned minus, unsigned other, double transConductance);
			
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
			 * \brief Print this device in human-readable format.
			 *
			 * \return string representation of this device
			 */
			std::string print();
		};
	}
}