#pragma once
#include "../Device/IDevice.h"
#include "../LinMath/LinearEquationSystem.h"
#include "INetworkSolver.h"

#include <vector>
#include <memory>

namespace Analyzer {
	namespace Network {
		typedef std::vector<std::vector<int>> NetworkGraph;
		typedef std::vector<std::unique_ptr<IDevice>> Branches;

		class Network
		{
			const unsigned N, B;

			// storage
			// G[node] -> [ids]
			// id is positive or negative depending on voltage and current direction
			NetworkGraph graph;
			Branches branches;

			std::unique_ptr<INetworkSolver> solver;
		public:
			Network(unsigned N, unsigned B, std::unique_ptr<INetworkSolver>);
			void addDevice(std::unique_ptr<IDevice>);
			void finishLoading();
			LinMath::LinearEquationSystem getEquations();
			LinMath::LinVector solve();
			const Branches& getBranches();
		};

		std::unique_ptr<Network> loadFromStream(std::istream&);
	}
}

