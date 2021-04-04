#include "Network.h"

#include <iostream>
#include <stack>
#include <sstream>

#include "../fmt/core.h"
#include "../LinMath/Matrix.h"
#include <algorithm>
#include "NetworkSolverDC.h"
#include "NetworkSolverGen.h"
#include "NetworkSolverTwoport.h"

Network::Network(unsigned N, unsigned B, std::unique_ptr<INetworkSolver> solver) : N(N), B(B), graph(N), solver(std::move(solver))
{
}

void Network::addDevice(std::unique_ptr<IOnePort> device)
{
	graph[device->port_plus - 1].push_back(device->id);
	graph[device->port_minus - 1].push_back(0-device->id);
	branches.push_back(std::move(device));
}

void Network::finishLoading()
{
	std::sort(branches.begin(), branches.end(), [](std::unique_ptr<IOnePort>& a, std::unique_ptr<IOnePort>& b) {
		return a->id > b->id; 
	});
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
	for (auto& cycle : *cycles) {
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

	return solver->solve(eq);
}

const std::vector<std::unique_ptr<IOnePort>>& Network::getBranches()
{
	return branches;
}

void DFS(NetworkGraph& graph, const Branches& branches, std::vector<int>& binding_branches, std::vector<int>& parent) {
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

std::unique_ptr<Network> loadFromStream(std::istream& stream)
{
	unsigned N, B;
	INetworkSolver* solver = nullptr;
	std::string analysisType;

	std::string line;
	std::getline(stream, line);

	std::istringstream iss(line);
	if (!(iss >> N >> B)) { 
		throw std::runtime_error(fmt::format("Error: could not parse header N and B: '{}'", line));
	}

	iss >> analysisType;
	if (iss) {
		if (analysisType == "DC") {
			solver = new NetworkSolverDC();
		} else if (analysisType == "GEN") {
			unsigned deviceid;
			iss >> deviceid;
			if (!iss) {
				throw std::runtime_error(fmt::format("Error: could not parse GEN analysis parameters: '{}'", line));
			}
			double R1 = NetworkSolverGen::R1_DEFAULT, R2 = NetworkSolverGen::R2_DEFAULT;
			iss >> R1 >> R2;
			solver = new NetworkSolverGen(deviceid, R1, R2);

		}
		else if (analysisType == "TWOPORT") {
			unsigned primaryId, secondaryId;
			iss >> primaryId >> secondaryId;
			if (!iss) {
				throw std::runtime_error(fmt::format("Error: could not parse TWOPORT analysis parameters: '{}'", line));
			}
			solver = new NetworkSolverTwoport(primaryId, secondaryId);
		} else {
			throw std::runtime_error(fmt::format("Error: invalid analysis type '{}' (valid options are 'DC' (default), 'GEN' or 'TWOPORT')", analysisType));
		}
	} else {
		// default to DC
		solver = new NetworkSolverDC();
		std::cout << "No analysis was specified, defaulting to DC!" << std::endl;
	}

	std::cout << "Solver: " << solver->print() << std::endl;

	std::unique_ptr<Network> network = std::unique_ptr<Network>(new Network(N, B, std::unique_ptr<INetworkSolver>(solver)));


	/*
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
	*/

	return network;
}

std::unique_ptr<NetworkGraph> findCycles(unsigned, unsigned, NetworkGraph& graph, Branches& branches) {
	std::unique_ptr<NetworkGraph> ret = std::make_unique<NetworkGraph>();	
	unsigned N = graph.size();

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
		
		std::unique_ptr<IOnePort>& branch = branches[abs(br) - 1];

		auto a = branch->port_plus;
		auto b = branch->port_minus;

		// trace back path to root from A
		while (parent[a - 1] != 0) {
			std::unique_ptr<IOnePort>&  parent_branch = branches[abs(parent[a - 1]) - 1];
			path_a.push_back(parent_branch->id * (parent_branch->sign(a)));
			a = parent_branch->other_port(a);
		}

		// trace back path from B
		while (parent[b - 1] != 0) {
			std::unique_ptr<IOnePort>& parent_branch = branches[abs(parent[b - 1]) - 1];
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
		ret->push_back(cycle);
	}

	return std::move(ret);
};