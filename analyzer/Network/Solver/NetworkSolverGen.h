#pragma once
#include <Network/INetworkSolver.h>

namespace Analyzer {
	namespace Network {
		namespace Solvers {
			/**
			 * Solver for determining equivalent generator (Thevenin, Norton) parameters.
			 * 
			 * Returns 3x1 matrix [U0; I0; R]
			 */
			class NetworkSolverGen :
				public INetworkSolver
			{
				
				/// first termination resistance to "measure" at
				const double R1;
				/// second termination resistance to "measure at"
				const double R2;
				/**
				 * Device id to use as termination.
				 */
				const unsigned deviceId;
			public:
				/**
				 * Create a new solver instance.
				 * 
				 * \param deviceId termination device id
				 * \param R1 first termination resistance to "measure" with
				 * \param R2 second termination resistance
				 */
				NetworkSolverGen(unsigned deviceId, double R1, double R2);

				/**
				 * Solve the network for generator parameters.
				 * 
				 * \param eqOrig extended equation system matrix of the network
				 * \return generator parameters as a [U0; I0; R] column matrix
				 */
				LinMath::Matrix solve(LinMath::Matrix& eqOrig) const;
				/**
				 * Print the solver in a human readable format.
				 * 
				 * \return string representation of the solver
				 */
				std::string print() const;
				/**
				 * Validate the solver.
				 * 
				 * - termination device must be valid
				 * - R1 and R2 must be two different positive numbers
				 * 
				 * \param N
				 * \param B
				 * \return error string on empty string when no error occured
				 */
				std::string validate(unsigned N, unsigned B);

				/// default value for R1
				const static double R1_DEFAULT;
				/// default value for R2
				const static double R2_DEFAULT;
			};
		}
	}
}