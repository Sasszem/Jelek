#include "Network.h"
#include "../fmt/core.h"
#include "../Device/devices.h"
#include "Solver/solvers.h"


#include <sstream>




using Analyzer::Network::Network;
using Analyzer::Network::INetworkSolver;
using Analyzer::Network::Solvers::NetworkSolverDC;
using Analyzer::Network::Solvers::NetworkSolverGen;
using Analyzer::Network::Solvers::NetworkSolverTwoport;


std::unique_ptr<Network> Analyzer::Network::loadFromStream(std::istream& stream)
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
		}
		else if (analysisType == "GEN") {
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
		}
		else {
			throw std::runtime_error(fmt::format("Error: invalid analysis type '{}' (valid options are 'DC' (default), 'GEN' or 'TWOPORT')", analysisType));
		}
	}
	else {
		// default to DC
		solver = new NetworkSolverDC();
		std::cout << "No analysis was specified, defaulting to DC!" << std::endl;
	}

	std::cout << "Solver: " << solver->print() << std::endl;

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
				throw std::runtime_error(fmt::format("Load error: END but less then B={} branches were added!", B));
			}
			run = false;
			continue;
		}

		std::istringstream iss(line);
		std::string type;

		iss >> type;
		std::cout << type << std::endl;
		std::cout << line << std::endl;

		IDevice* device = nullptr;


		if (type[0] == '$') {
			// coupled devices
			unsigned id, pPlus, pMinus, coupledId;
			iss >> id >> pPlus >> pMinus >> coupledId;
			if (!iss) {
				throw std::runtime_error(fmt::format("Error: can not parse coupled device base parameters: '{}'", line));
			}

			if (coupledId == 0 || coupledId > N) {
				throw std::runtime_error(fmt::format("Error: invalid coupled device ID {}: '{}'", coupledId, line));
			}

			double param = 0;
			if (type == "$CCVS" || type == "$CCCS" || type == "$VCVS" || type == "$VCCS") {
				iss >> param;
				if (!iss) {
					throw std::runtime_error(fmt::format("Error: could not parse coupled device parameter: '{}'", line));
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
			else {
				throw std::runtime_error(fmt::format("Error: unknown coupled device: '{}'", type));
			}

			network->addDevice(std::unique_ptr<IDevice>(device));
		}
		else if (type[0] == '!') {
			// shorthand forms

		}
		else {
			// normal devices

			unsigned id, pPlus, pMinus;
			iss >> id >> pPlus >> pMinus;
			if (!iss) {
				throw std::runtime_error(fmt::format("Error: can not parse device base parameters: '{}'", line));
			}

			double param = 0;
			if (type == "RES" || type == "VOL" || type == "CUR") {
				iss >> param;
				if (!iss) {
					throw std::runtime_error(fmt::format("Error: could not parse device parameter: '{}'", line));
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
				throw std::runtime_error(fmt::format("Error: unknown device: '{}'", type));
			}

			network->addDevice(std::unique_ptr<IDevice>(device));
		}
	}

	return network;
}
