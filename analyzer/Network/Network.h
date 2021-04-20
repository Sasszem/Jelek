#pragma once
#include <Device/IDevice.h>
#include "INetworkSolver.h"

#include <vector>
#include <memory>

/// namespace for analyzer project
namespace Analyzer {
	/// namespace for network-related classes and functions
	namespace Network {
		/// vector of vector of ints used for network graph or list of cycles
		typedef std::vector<std::vector<int>> NetworkGraph;
		/// vector of managed device objects for storing devices
		typedef std::vector<std::unique_ptr<Analyzer::Device::IDevice>> Branches;


		/**
		 * Class representing a linear resistive network.
		 * 
		 * Stores network and produces equations for solving.
		 */
		class Network
		{
			/// the network graph itself
			/// stored as an outgoing adjecency list format
			/// with signed IDs of the devices / branches
			/// positive means outgoing, negative is incoming
			NetworkGraph graph;
			/// list of branches of the network graph
			/// generally unsorted, but finishLoading() sorts it ascending by ID
			Branches branches;

			/// a pointer to the solver object of the network
			std::unique_ptr<INetworkSolver> solver;
		public:
			/// number of nodes of the network graph
			const unsigned N;
			/// number of branches of the network graph (number of devices)
			const unsigned B;
			/**
			 * Initialize empty network with capacity and solver.
			 * 
			 * \param N number of nodes
			 * \param B number of branches
			 * \param solver solver object
			 */
			Network(unsigned N, unsigned B, std::unique_ptr<INetworkSolver> solver);
			/**
			 * Add one device to the network.
			 * 
			 * \param device
			 * \throw LoadException if the device ID is invalid (0 or >B)
			 * \throw LoadException if a device with the same ID is already added
			 * \throw LoadException if either port of the device is invalid (0 or >N)
			 */
			void addDevice(std::unique_ptr<Analyzer::Device::IDevice> device);
			/**
			 * Sort devices by ID.
			 */
			void finishLoading();

			/**
			 * Generate equation system of the network.
			 * 
			 * It'll be
			 * - B characteristic equations of devices sorted by ID ascending
			 * - N-1 Kirchoff's node laws (no particular order)
			 * - B-N+1 Kirchoff's voltage laws (no particular order)
			 * 
			 * \return the equation system's extended matrix
			 */
			LinMath::Matrix getEquations();

			/**
			 * Solve the network using the solver. The meaning of the solution matrix depends on the solver used.
			 * 
			 * \return a matrix representing the solution
			 * \throw SolveException if anything goes wrong
			 */
			LinMath::Matrix solve();


			/**
			 * Get current branch count (used while loading).
			 * 
			 * \return the number of already added branches
			 */
			unsigned getBranchCount() const;
		};

		/**
		 * Load a network from a file.
		 * 
		 * Filename "-" means load from stdin
		 * 
		 * \param fname file name / path to load from
		 * \return a managed pointer to the loaded network
		 * \throw LoadException if the file is not valid
		 */
		std::unique_ptr<Network> loadFromFile(std::string fname);
	}
}