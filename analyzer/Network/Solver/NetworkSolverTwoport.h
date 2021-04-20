#pragma once
#include <Network/INetworkSolver.h>

namespace Analyzer {
	namespace Network {
		namespace Solvers {
			/**
			 * Network solver for two port characteristic matrices.
			 * 
			 * Returns a 12x2 matrix, containing the 6 characteristic matrices (R, G, H, K, A, B). If any of them does not exitst, it'll be NaN-d
			 */
			class NetworkSolverTwoport :
				public INetworkSolver
			{
				/// primary port termination device ID
				const unsigned primary;
				/// secondary port termination device ID
				const unsigned secondary;
			public:
				/**
				 * Initialize a solver with port termination devices.
				 * 
				 * \param primaryID
				 * \param secondaryID
				 */
				NetworkSolverTwoport(unsigned primaryID, unsigned secondaryID);

				/**
				 * Solve for characteristic matrices.
				 * 
				 * \param eqOrig
				 * \return the 12x2 matrix as described in the class's description
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
				 * - both device IDs must be valid
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