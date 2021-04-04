#pragma once
#include "../OnePort/IOnePort.h"
#include <vector>
#include <string>
#include "../LinMath/LinearEquationSystem.h"
#include "INetworkSolver.h"
#include <memory>

typedef std::vector<std::vector<int>> NetworkGraph;
typedef std::vector<std::unique_ptr<IOnePort>> Branches;

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
	void addDevice(std::unique_ptr<IOnePort>);
	void finishLoading();
	LinMath::LinearEquationSystem getEquations();
	LinMath::LinVector solve();
	const Branches& getBranches();
};

std::unique_ptr<Network> loadFromStream(std::istream&);
std::unique_ptr<NetworkGraph> findCycles(unsigned, unsigned, NetworkGraph&, Branches&);
void DFS(NetworkGraph& graph, const Branches& branches, std::vector<int>& binding_branches, std::vector<int>& parent);


