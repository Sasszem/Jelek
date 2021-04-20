#pragma once
#include "ICoupledDevice.h"

namespace Analyzer {
	namespace Device {
		/**
		 * Nullator.
		 *
		 * Parameter: none
		 * Equation: u=i=0
		 * 
		 * This actually has 2 equations, so it hijacks the other device and writes it's own equation in the place of the others.
		 * To this to work, the other device must not modify the equation after this, so the other device should have ID lower than this.
		 * Also, the other will be turned into a norator (device with no equations), and the two will become a nullor
		 */
		class Nullator :
			public ICoupledDevice
		{
		public:
			/**
			 * Construct a new nullator.
			 * 
			 * \param id
			 * \param pPlus
			 * \param pMinus
			 * \param noratorID
			 */
			Nullator(unsigned id, unsigned pPlus, unsigned pMinus, unsigned noratorID);

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