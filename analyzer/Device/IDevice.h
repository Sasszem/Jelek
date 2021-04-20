#pragma once

#include <LinMath/math.h>
#include <string>

namespace Analyzer {
	/**
	 * Namespace for (coupled) devices.
	 */
	namespace Device {
		/**
		 * Abstract base class of devices.
		 * 
		 * Devices have 2 connections (plus and minus), have a voltage drop, current and a characteristic equation describing the relationship between those 2.
		 * 
		 * Devices are responsible for writing their characteristic equations into the extended equation system matrix. This happens in a fixed order (sorted by ID), so they MIGHT overwrite other devices equations (this is needed by Nullator, and I could not eliminate this requirement).
		 * 
		 * Signs of voltage and current:
		 * - voltage drop is positive if the plus connection is at higher potential
		 * - current is positive if it's going into the plus connection
		 */
		class IDevice
		{
		protected:
			/**
			 * Current index helper when writing equations into the matrix
			 * 
			 * \param id id of device to get current index of
			 * \return unknown index of the current of the given device
			 */
			static unsigned currentIndexFromId(unsigned id);
			/**
			 * Voltage index helper when writing equations into the matrix
			 *
			 * \param id id of device to get voltage index of
			 * \return unknown index of the voltage of the given device
			 */
			static unsigned voltageIndexFromId(unsigned id);
		public:
			/// plus connection (node ID)
			const unsigned port_plus;
			/// minus connection (node ID)
			const unsigned port_minus;
			/// ID of self
			const unsigned id;
			/**
			 * Construct device base common to all devices.
			 * 
			 * \param id
			 * \param plus
			 * \param minus
			 */
			IDevice(unsigned id, unsigned plus, unsigned minus);

			/**
			 * Get other port if one port is given.
			 * 
			 * If the port is neither plus nor minus port, it returns 0.
			 * Since this, care must be taken to ensure that port is valid
			 * 
			 * \param port
			 * \return other port or 0
			 */
			unsigned other_port(unsigned port);

			/**
			 * Sign of this device as seen from port.
			 * 
			 * - 1 if port is plus port
			 * - -1 otherwise
			 * 
			 * Since this care must be taken to only call this function with one of the ports of this device
			 * 
			 * \param port
			 * \return sign of this device
			 */
			int sign(unsigned port);

			/**
			 * Write characteristic equation of this node into the matrix.
			 * 
			 * \param eq matrix to write variable coefficents
			 * \return free component of the equation
			 */
			virtual double equation(LinMath::Matrix& eq) = 0;

			/**
			 * Get current index of this device.
			 * 
			 * \return index of the current of this device in the matrix
			 */
			unsigned currentIndex();
			/**
			 * Get voltage index of this device.
			 *
			 * \return index of the voltage of this device in the matrix
			 */
			unsigned voltageIndex();

			/**
			 * Print this device in a human-readable format.
			 * 
			 * \return string representation of this device
			 */
			virtual std::string print() = 0;

			/**
			 * (empty) virtual destructor for futureproofing.
			 */
			virtual ~IDevice();
		};
	}
}