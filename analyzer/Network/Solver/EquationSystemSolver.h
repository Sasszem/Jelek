#pragma once
#include <Network/INetworkSolver.h>

namespace Analyzer {
	namespace Network {
		namespace Solvers {
			/**
			 * \brief Network "solver" for printing the equations.
			 * 
			 * Can print in a human-readable format or as matlab code
			 */
			class EquationSystemSolver :
				public INetworkSolver
			{
				/// mode to print in. 1 is human-readable, 2 is matlab
				const unsigned printMode;
			public:
				/// default mode (human readable)
				const static unsigned MODE_DEFAULT = 1;

				/**
				 * \brief Initialize with mode.
				 * 
				 * \param mode
				 */
				EquationSystemSolver(unsigned mode);
				/**
				 * \brief "Solve" the network (print equation system).
				 * 
				 * \param eq
				 * \return an empty (0x0) matrix
				 */
				LinMath::Matrix solve(LinMath::Matrix& eq) const;

				/**
				 * \brief Validate solver.
				 * 
				 * Validates mode only
				 * 
				 * \param N
				 * \param B
				 * \return the error string or empty string
				 */
				std::string validate(unsigned N, unsigned B);

				/**
				 * \brief Print in a guman readable format.
				 * 
				 * \return string representation of the solver
				 */
				std::string print() const;
			};
		}
	}
}

