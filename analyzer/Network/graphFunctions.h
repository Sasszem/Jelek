#pragma once

#include "Network.h"

namespace Analyzer {
	namespace Network {
		/**
		 * \brief A function returning all the cycles found in the network graph.
		 * 
		 * It returns the branches of the cycle as signed IDs depending on the orientation (+ if same as the cycle, - otherwise)
		 * 
		 * \param graph as in Network
		 * \param branches as in Network
		 */
		std::unique_ptr<NetworkGraph> findCycles(NetworkGraph& graph, Branches& branches);

		/**
		 * \brief DFS traversal on the graph 
		 * 
		 * this dual purpose function is a DFS traversal for two jobs
		 * it finds all binding branches of the spanning forest of the network graph
		 * and also records for each node the branch it was reached from
		 * and that will be used to trace paths.
		 * 
		 * \param graph as in Network
		 * \param branches as in Network
		 * \param binding_branches (return) vector of found binding branches
		 * \param parent (return) [node]->[signed ID of branch discovered via | 0 for root of component] for tracing paths
		 */
		void DFS(NetworkGraph& graph, const Branches& branches, std::vector<int>& binding_branches, std::vector<int>& parent);
	}
}