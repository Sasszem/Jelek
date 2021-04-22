#pragma once

#include <string>
#include <LinMath/Matrix.h>

namespace Analyzer {
	namespace Network {
		/**
		 * \brief Abstract base class of all network solvers.
		 */
		class INetworkSolver
		{
		public:
			/**
			 * \brief Solve the network based on extended equation system matrix.
			 * 
			 * The result's interpretation depends on the solver instance
			 * 
			 * Can throw any exception, they'll be catched by Network::solve and rethrown as a SolveException
			 * 
			 * \param eq extended equation system matrix of the network
			 * \return result
			 */
			virtual LinMath::Matrix solve(LinMath::Matrix& eq) const = 0;

			/**
			 * \brief Print the solver in a human readable format.
			 * 
			 * Used when debugging
			 * 
			 * \return the solver in a human-readable format
			 */
			virtual std::string print() const = 0;

			/**
			 * \brief Validate solver based on network parameters.
			 * 
			 * \param N number of nodes in the network
			 * \param B number of branches (devices) in the network
			 * \return an error string representing the error or empty string when valid
			 */
			virtual std::string validate(unsigned N, unsigned B) = 0;
		};
	}
}