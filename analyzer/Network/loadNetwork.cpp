#include "Network.h"
#include "../fmt/core.h"
#include "../Device/devices.h"
#include "Solver/solvers.h"
#include "LoadException.h"

#include <iostream>
#include <sstream>


using Analyzer::Network::Network;
using Analyzer::Network::INetworkSolver;
using Analyzer::Network::Solvers::NetworkSolverDC;
using Analyzer::Network::Solvers::NetworkSolverGen;
using Analyzer::Network::Solvers::NetworkSolverTwoport;
using Analyzer::Network::Solvers::EquationSystemSolver;
using Analyzer::Network::Solvers::NetworkSolverResistance;
using Analyzer::Network::LoadException;
using namespace Analyzer::Device;

void parseDevice(std::string, std::unique_ptr<Network>&);
void parseCoupledDevice(std::string, std::unique_ptr<Network>&);
void parseShorthand(std::string, std::unique_ptr<Network>&);

std::unique_ptr<Network> Analyzer::Network::loadFromStream(std::istream& stream)
{
	unsigned N, B;
	INetworkSolver* solver = nullptr;
	std::string analysisType;

	std::string line;
	std::getline(stream, line);

	std::istringstream iss(line);
	if (!(iss >> N >> B)) {
		throw LoadException(fmt::format("Error: could not parse header N and B: '{}'", line));
	}

	iss >> analysisType;
	if (iss) {
		if (analysisType == "DC") {
			solver = new NetworkSolverDC();
		}
		else if (analysisType == "GEN") {
			unsigned deviceid;
			iss >> deviceid;
			if (!iss) {
				throw LoadException(fmt::format("Error: could not parse GEN analysis parameters: '{}'", line));
			}
			double R1 = NetworkSolverGen::R1_DEFAULT, R2 = NetworkSolverGen::R2_DEFAULT;
			iss >> R1 >> R2;
			solver = new NetworkSolverGen(deviceid, R1, R2);

		}
		else if (analysisType == "TWOPORT") {
			unsigned primaryId, secondaryId;
			iss >> primaryId >> secondaryId;
			if (!iss) {
				throw LoadException(fmt::format("Error: could not parse TWOPORT analysis parameters: '{}'", line));
			}
			solver = new NetworkSolverTwoport(primaryId, secondaryId);
		}
		else if (analysisType == "EQ") {
		}
		else if (analysisType == "RES") {
			unsigned port;
			iss >> port;
			if (!iss) {
				throw LoadException(fmt::format("Error: could not parse RES device id: '{}'", line));
			}
			solver = new NetworkSolverResistance(port);
		}
		else {
			throw LoadException(fmt::format("Error: invalid analysis type '{}' (valid options are 'DC' (default), 'GEN' or 'TWOPORT')", analysisType));
		}
	}
	else {
		// default to DC
		solver = new NetworkSolverDC();
	}

	std::unique_ptr<Network> network = std::unique_ptr<Network>(new Network(N, B, std::unique_ptr<INetworkSolver>(solver)));



	bool run = true;
	while (std::getline(stream, line) && run)
	{
		// ignore empty lines and comments
		if (line.empty() || line[0] == '#')
			continue;

		// stop on 'END'
		if (line == "END") {
			if (B != network->getBranches().size()) {
				throw LoadException(fmt::format("Load error: END but less then B={} branches were added!", B));
			}
			run = false;
			continue;
		}

		std::istringstream iss(line);
		std::string type;

		iss >> type;

		IDevice* device = nullptr;


		if (type[0] == '$') {
			// coupled devices
			parseCoupledDevice(line, network);
		}
		else if (type[0] == '!') {
			// shorthand forms
			parseShorthand(line, network);
		}
		else {
			// normal devices
			parseDevice(line, network);

		}
	}

	return network;
}

void parseDevice(std::string line, std::unique_ptr<Network>& network) {
	std::istringstream iss(line);
	std::string type;

	iss >> type;
	IDevice* device = nullptr;

	unsigned id, pPlus, pMinus;
	iss >> id >> pPlus >> pMinus;
	if (!iss) {
		throw LoadException(fmt::format("Error: can not parse device base parameters: '{}'", line));
	}

	double param = 0;
	if (type == "RES" || type == "VOL" || type == "CUR") {
		iss >> param;
		if (!iss) {
			throw LoadException(fmt::format("Error: could not parse device parameter: '{}'", line));
		}
	}

	if (type == "RES") {
		device = new Resistor(id, pPlus, pMinus, param);
	}
	else if (type == "VOL") {
		device = new VoltageSource(id, pPlus, pMinus, param);
	}
	else if (type == "CUR") {
		device = new CurrentSource(id, pPlus, pMinus, param);
	}
	else if (type == "WIRE") {
		device = new Wire(id, pPlus, pMinus);
	}
	else if (type == "PROBE") {
		device = new Probe(id, pPlus, pMinus);
	}
	else {
		throw LoadException(fmt::format("Error: unknown device: '{}'", type));
	}

	network->addDevice(std::unique_ptr<IDevice>(device));
}

void parseCoupledDevice(std::string line, std::unique_ptr<Network>& network) {
	std::istringstream iss(line);
	std::string type;

	iss >> type;
	IDevice* device;

	unsigned id, pPlus, pMinus, coupledId;
	iss >> id >> pPlus >> pMinus >> coupledId;
	if (!iss) {
		throw LoadException(fmt::format("Error: can not parse coupled device base parameters: '{}'", line));
	}

	if (coupledId == 0 || coupledId > network->B) {
		throw LoadException(fmt::format("Error: invalid coupled device ID {}: '{}'", coupledId, line));
	}

	double param = 0;
	if (type == "$CCVS" || type == "$CCCS" || type == "$VCVS" || type == "$VCCS") {
		iss >> param;
		if (!iss) {
			throw LoadException(fmt::format("Error: could not parse coupled device parameter: '{}'", line));
		}
	}

	if (type == "$CCCS") {
		device = new CCCS(id, pPlus, pMinus, coupledId, param);
	}
	else if (type == "$CCVS") {
		device = new CCVS(id, pPlus, pMinus, coupledId, param);
	}
	else if (type == "$VCVS") {
		device = new VCVS(id, pPlus, pMinus, coupledId, param);
	}
	else if (type == "$VCCS") {
		device = new VCCS(id, pPlus, pMinus, coupledId, param);
	}
	else if (type == "$NULL") {
		if (coupledId >= id) {
			throw LoadException(fmt::format("Error: Nullator friend id can not be higher than own id: '{}'", line));
		}
		device = new Nullator(id, pPlus, pMinus, coupledId);
	}
	else {
		throw LoadException(fmt::format("Error: unknown coupled device: '{}'", type));
	}

	network->addDevice(std::unique_ptr<IDevice>(device));
}

void parseShorthand(std::string line, std::unique_ptr<Network>& network) {
	std::istringstream iss(line);
	std::string type;

	iss >> type;

	IDevice* device1;
	IDevice* device2;

	unsigned id, plus1, minus1, plus2, minus2;
	iss >> id >> plus1 >> minus1 >> plus2 >> minus2;
	if (!iss) {
		throw LoadException(fmt::format("Error: can not parse shorthand base parameters: '{}'", line));
	}

	double param = 0;
	if (type == "!TRAN" || type == "!GYR") {
		iss >> param;
		if (!iss) {
			throw LoadException(fmt::format("Error: could not parse shorthand parameter: '{}'", line));
		}
	}

	if (type == "!AMP") {
		device1 = new Wire(id, plus2, minus2);
		device2 = new Nullator(id + 1, plus1, minus1, id);
	}
	else if (type == "!GYR") {
		device1 = new CCVS(id, plus1, minus1, id + 1, -param);
		device2 = new CCVS(id + 1, plus2, minus2, id, param);
	}
	else if (type == "!TRAN") {
		device1 = new VCVS(id, plus1, minus1, id + 1, param);
		device2 = new CCCS(id + 1, plus2, minus2, id, -param);
	}
	else {
		throw LoadException(fmt::format("Error: unknown coupled device: '{}'", type));
	}

	network->addDevice(std::unique_ptr<IDevice>(device1));
	network->addDevice(std::unique_ptr<IDevice>(device2));
}
