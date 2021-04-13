#include "Network.h"
#include "graphFunctions.h"
#include "../fmt/core.h"
#include <algorithm>
#include <stdexcept>
#include "SolveException.h"
#include "LoadException.h"

using Analyzer::Network::Network;
using Analyzer::Network::NetworkGraph;
using Analyzer::Network::Branches;
using Analyzer::Network::DFS;
using Analyzer::Network::SolveException;
using Analyzer::Device::IDevice;

Network::Network(unsigned N, unsigned B, std::unique_ptr<INetworkSolver> solver) : N(N), B(B), graph(N), solver(std::move(solver))
{
}

void Network::addDevice(std::unique_ptr<IDevice> device)
{
	// check if the ID is valid
	if (device->id == 0 || device->id > B) {
		throw LoadException(fmt::format("Device ID {} is outside of the allowed range 1..{}", device->id, B));
	}

	// check if the ID is not a duplicate one
	if (std::any_of(branches.begin(), branches.end(), [&device](std::unique_ptr<IDevice>& test) {return device->id == test->id; })) {
		throw LoadException(fmt::format("Device ID {} is already taken!", device->id));
	}

	// validate ports
	if (device->port_minus == 0 || device->port_minus > N) {
		throw LoadException(fmt::format("Invalid minus port of device: {}", device->print()));
	}

	if (device->port_plus== 0 || device->port_plus > N) {
		throw LoadException(fmt::format("Invalid plus port of device: {}", device->print()));
	}

	graph[device->port_plus - 1].push_back(device->id);
	graph[device->port_minus - 1].push_back(0-device->id);
	branches.push_back(std::move(device));
}

void Network::finishLoading()
{
	std::sort(branches.begin(), branches.end(), [](std::unique_ptr<IDevice>& a, std::unique_ptr<IDevice>& b) {
		return a->id < b->id; 
	});
}

LinMath::Matrix Network::getEquations() {
	this->finishLoading();
	LinMath::Matrix eq(2 * B, 2*B + 1);

	// unknowns: u0, i0, u1, i1, ... 

	unsigned idx = B;

	// characteristic equations of nodes
	for (auto& branch : branches) {
		eq(branch->id-1, 2*B) = branch->equation(eq);
	}
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
				eq(idx, 2 * abs(*it) - 1) = (*it > 0) ? 1 : -1;
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
			eq(idx, 2 * abs(b) - 2) = (b > 0) ? 1 : -1;
		}
		idx++;
	}

	return eq;
}

LinMath::Matrix Network::solve() {
	try {
		auto eq = getEquations();
		return solver->solve(eq);
	}
	catch (const std::exception& e) {
		throw SolveException(e.what());
	}
}

const std::vector<std::unique_ptr<IDevice>>& Network::getBranches()
{
	return branches;
}