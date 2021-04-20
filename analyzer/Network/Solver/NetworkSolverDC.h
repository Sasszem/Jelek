#pragma once
#include <Network/INetworkSolver.h>

namespace Analyzer {
	namespace Network {
		/**
		 * Namespace for all the different network solvers.
		 */
		namespace Solvers {
			/**
			 * DS solver, solves for all voltages and currents.
			 * 
			 * Result is 2B x 1 matrix, with voltages and currents
			 * in u1, i1, u2, i2, ... order
			 */
			class NetworkSolverDC :
				public INetworkSolver
			{
			public:
				/**
				 * Constructor.
				 * 
				 * This solver does not have any parameters.
				 * 
				 */
				NetworkSolverDC();
				/**
				 * Mandatory solve method.
				 * 
				 * Solves by inverting the matrix.
				 * 
				 * \param eq extended matrix of equation system
				 * \return the voltages and currents as a 2B x 1 matrix
				 */
				LinMath::Matrix solve(LinMath::Matrix& eq) const;

				/**
				 * Print the solver.
				 * 
				 * \return a string representation of the solver (human-readable)
				 */
				std::string print() const;

				/**
				 * Validate the solver for a network.
				 * 
				 * Always returns no error (empty string)
				 * 
				 * \param N
				 * \param B
				 * \return the error (always empty string)
				 */
				std::string validate(unsigned N, unsigned B);
			};
		}
	}
}