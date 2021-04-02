#include "Network.h"

#include <iostream>
#include <stack>
#include <sstream>

#include "../LinMath/Matrix.h"

Network::Network(unsigned N, unsigned B) : N(N), B(B), graph(N), branches(B), solver(nullptr)
{
}

void Network::addDevice(IOnePort& device)
{
	auto deviceCopy = device.copy();
	branches[device.id - 1] = deviceCopy;
	graph[device.port_plus - 1].push_back(device.id);
	graph[device.port_minus - 1].push_back(0-device.id);
}

LinMath::LinearEquationSystem Network::getEquations() {
	LinMath::LinearEquationSystem eq(2 * B);

	// unknowns: u0, i0, u1, i1, ... 

	unsigned idx = 0;

	// node equations
	// basicly KIRCHOFF's NODE LAWS
	// so current going into a node equals the current going out

	// find relevant nodes
	// all but one from each component
	// with DFS we can find the components
	// and since we marked their roots
	// we can just take every node EXCEPT them
	auto parent = std::vector<int>(N);;
	std::vector<int> binding_branches;
	DFS(graph, branches, binding_branches, parent);

	for (unsigned i = 0; i < N; i++) {
		if (parent[i] != 0) {
			for (auto it = graph[i].begin(); it != graph[i].end(); it++) {
				eq.getMatrix()[idx][2 * abs(*it) - 1] = (*it > 0) ? 1 : -1;
			}
			idx++;
		}
	}

	// cycles
	// Kirchoff's voltage laws (KVLs)
	// going round in a loop, the sum of all voltages (with proper directions) is 0

	auto cycles = findCycles(N, B, graph, branches);
	for (auto& cycle : cycles) {
		for (auto& b : cycle) {
			eq.getMatrix()[idx][2 * abs(b) - 2] = (b > 0) ? 1 : -1;
		}
		idx++;
	}

	// characteristic equations of nodes
	for (auto& branch : branches) {
		eq.getVector()[idx] = branch->equation(eq.getMatrix()[idx]);
		idx++;
	}

	return eq;
}

LinMath::LinVector Network::solve() {
	auto eq = getEquations();

	return eq.getMatrix().invert() * eq.getVector();
}

const std::vector<IOnePort*>& Network::getBranches()
{
	return branches;
}


Network::~Network()
{
	for (auto& elem : branches)
 		delete elem;
}

void DFS(NetworkGraph graph, std::vector<IOnePort*>& branches, std::vector<int> &binding_branches, std::vector<int> &parent) {
	/// <summary>
	/// this dual purpose function is a DFS traversal for two jobs
	/// it finds all binding branches of the spanning forest of the network graph
	///	and also records for each node the branch it was reached from
	///	and that will be used to trace paths
	/// </summary>
	/// <param name="graph">the network graph</param>
	/// <param name="branches">branches of the graph</param>
	/// <param name="binding_branches">returns the binding branches found</param>
	/// <param name="parent">returns the recoded parent branches</param>
	
	unsigned N = graph.size();
	
	auto seen = std::vector<bool>(N);
	auto finished = std::vector<bool>(N);
	for (unsigned i = 0; i < N; i++) {
		seen[i] = false;
		finished[i] = false;
	}

	// For each component
	for (unsigned i = 0; i < N; i++) {
		if (!seen[i]) {
			// DFS bookkeeping
			std::stack<unsigned> to_process;

			// start with a node from the component
			to_process.push(i);
			seen[i] = true;
			parent[i] = 0;

			while (!to_process.empty()) {
				// get node to process
				unsigned current = to_process.top();
				to_process.pop();

				// for each branch (ID)
				for (auto& b : graph[current]) {
					// get actual branch
					const auto& br = branches[abs(b) - 1];

					// get other end
					auto other_end = (b < 0 ? br->port_plus : br->port_minus) - 1;

					// if not seen, add to stack to visit
					// and mark seen
					// else we found a binding branch
					if (!seen[other_end]) {
						seen[other_end] = true;
						parent[other_end] = b;
						to_process.push(other_end);
					}
					else {
						if (!finished[other_end])
							binding_branches.push_back(b);
					}

				}
				finished[current] = true;
			}
		}
	}
}

#include "../OnePort/Resistor.h"
#include "../OnePort/CurrentSource.h"
#include "../OnePort/VoltageSource.h"

Network* loadFromStream(std::istream& stream)
{
	// stream must start with 2 numbers: N and B
	unsigned N, B;

	std::string line;
	std::getline(stream, line);

	std::istringstream iss(line);
	if (!(iss >> N >> B)) { 
		// TODO
	}

	Network* network = new Network(N, B);

	bool run = true;
	while (std::getline(stream, line) && run)
	{
		std::istringstream iss(line);
		std::string type;
		
		iss >> type;
		std::cout << type << std::endl;
		std::cout << line << std::endl;
		
		if (type[0] == '$') {
			unsigned id = 0, portPlus = 0, portMinus = 0;
			iss >> id >> portPlus >> portMinus;
			if (!iss) {
				// TODO
			}
			double paramd = 0;
			IOnePort* device = nullptr;

			if (type == "$RES") {
				iss >> paramd;
				if (!iss) {
					// TODO
				}
				device = new Resistor(id, portPlus, portMinus, paramd);
			} else if (type == "$VOL") {
				iss >> paramd;
				if (!iss) {
					// TODO
				}
				device = new VoltageSource(id, portPlus, portMinus, paramd);
			} else if (type == "$CUR") {
				iss >> paramd;
				if (!iss) {
					// TODO
				}
				device = new CurrentSource(id, portPlus, portMinus, paramd);
			} else {
				
			}
			if (device) {
				std::cout << "adding:" << std::endl;
				device->print(std::cout);
				network->addDevice(*device);
				delete device;
			}
			else {
				// TODO
			}
		} else {
			if (type == ".end") {
				std::cout << "EOF!" << std::endl;
				run = false;
			}
		}
	}


	return network;
}

NetworkGraph findCycles(unsigned N, unsigned B, NetworkGraph& graph, std::vector<IOnePort*>& branches) {
	NetworkGraph ret;

	// run traversal on graph and find tree branches and binding branches
	// need to keep track of what nodes did we already visit
	

	// parents so we can backtrack paths to the root
	// holds ID of branch (1..B)
	// the root has 0
	auto parent = std::vector<int>(N);;
	std::vector<int> binding_branches;
	DFS(graph, branches, binding_branches, parent);
	

	// binding branches now hold a list of all branches that induce a cycle
	// we use them to find those cycles
	// to do that, we have to determine the shortest path between their ends
	// but only from tree branches
	// so we backtrack to the root from them to build a path
	// and then remove all elements that are in both paths
	// those MUST be at the end of them

	// for each binding branch
	for (int& br : binding_branches) {

		std::vector<int> path_a, path_b;
		
		auto branch = branches[abs(br) - 1];

		auto a = branch->port_plus;
		auto b = branch->port_minus;

		// trace back path to root from A
		while (parent[a - 1] != 0) {
			auto parent_branch = branches[abs(parent[a - 1]) - 1];
			path_a.push_back(parent_branch->id * (parent_branch->sign(a)));
			a = parent_branch->other_port(a);
		}

		// trace back path from B
		while (parent[b - 1] != 0) {
			auto parent_branch = branches[abs(parent[b - 1]) - 1];
			path_b.push_back(parent_branch->id * (parent_branch->sign(b)));
			b = parent_branch->other_port(b);
		}

		// remove same elements from back
		while (path_a.size() > 0 && path_b.size() > 0 && abs(path_a[path_a.size() - 1]) == abs(path_b[path_b.size() - 1])) {
			path_b.pop_back();
			path_a.pop_back();
		}

		// create the cycle
		std::vector<int> cycle;

		// root to A
		for (auto& n : path_a) {
			cycle.push_back(-n);
		}
		// B to root
		for (auto& n : path_b) {
			cycle.push_back(n);
		}
		// A to B
		cycle.push_back(abs(br));

		// add cycle
		ret.push_back(cycle);
	}

	return ret;
};