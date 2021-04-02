#pragma once
#include "../OnePort/IOnePort.h"
#include <vector>
#include <string>
#include "../LinMath/LinearEquationSystem.h"
#include "INetworkSolver.h"

typedef std::vector<std::vector<int>> NetworkGraph;

class Network
{
	const unsigned N, B;

	// storage
	// G[node] -> [ids]
	// id is positive or negative depending on voltage and current direction
	NetworkGraph graph;
	std::vector<IOnePort*> branches;

	INetworkSolver* solver;
public:
	Network(unsigned N, unsigned B);
	void addDevice(IOnePort &device);
	
	LinMath::LinearEquationSystem getEquations();
	void setSolver(INetworkSolver* solver);
	LinMath::LinVector solve();
	const std::vector<IOnePort *>& getBranches();
	~Network();
};

Network* loadFromStream(std::istream&);
NetworkGraph findCycles(unsigned, unsigned, NetworkGraph&, std::vector<IOnePort*>&);
void DFS(NetworkGraph graph, std::vector<IOnePort*>& branches, std::vector<int>& binding_branches, std::vector<int>& parent);


