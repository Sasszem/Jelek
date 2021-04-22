#pragma once
#include <Network/INetworkSolver.h>

namespace Analyzer {
	namespace Network {
		namespace Solvers {
			/**
			 * \brief Network solver to determine equivalent resistance of a network.
			 */
			class NetworkSolverResistance :
				public INetworkSolver
			{
				/// termination device ID
				const unsigned port;
			public:
				/**
				 * \brief Initialize solver using a termination device ID.
				 * 
				 * \param device
				 */
				NetworkSolverResistance(unsigned device);
				/**
				 * \brief Solve for the equivalent resistance of the network seen from the termination device.
				 * 
				 * \param eqOrig extended equation system matrix
				 * \return 1x1 matrix with the resistance
				 * \throw SolveException if the 0-current termination voltage is not 0
				 */
				LinMath::Matrix solve(LinMath::Matrix& eqOrig) const;
				/**
				 * \brief Convert to a human readable format.
				 * 
				 * \return string representation of the solver
				 */
				std::string print() const;

				/**
				 * \brief Validate the solver.
				 * 
				 * - termination device ID should be valid
				 * 
				 * \param N
				 * \param B
				 * \return error string or empty string
				 */
				std::string validate(unsigned N, unsigned B);
			};
		}
	}
}