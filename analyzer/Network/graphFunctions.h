#pragma once

#include "Network.h"

namespace Analyzer {
	namespace Network {
		std::unique_ptr<NetworkGraph> findCycles(unsigned, unsigned, NetworkGraph&, Branches&);
		void DFS(NetworkGraph& graph, const Branches& branches, std::vector<int>& binding_branches, std::vector<int>& parent);
	}
}