#include "net.h"
#include <fmt/core.h>
#include <Device/devices.h>
#include <Network/Solver/solvers.h>

#include <iostream>
#include <sstream>
#include <fstream>

using namespace Analyzer::Network;
using namespace Analyzer::Network::Solvers;
using namespace Analyzer::Device;

/**
 * \brief parse and load one coupled device pair shorthand.
 * 
 * \param line line to load from
 * \param network network to add the devices to
 * \throw LoadException if the shorthand is invalid
 */
void parseShorthand(std::string line, std::unique_ptr<Network>& network);

/**
 * \brief parse and load one normal (non-coupled) device.
 * 
 * \param line line to load from
 * \param network network to add the device to
 * \throw LoadException if the device is invalid
 */
void parseDevice(std::string line, std::unique_ptr<Network>& network);


/**
 * \brief parse and load one couple device.
 * 
 * \param line line to load from
 * \param network network to add the device to
 * \throw LoadException if the device is invalid
 */
void parseCoupledDevice(std::string line, std::unique_ptr<Network>& network);

/**
 * \brief parse network parameters and solver
 *
 * \param line first line of the file
 * \return the created network
 * \throw LoadException if the header is invalid
 */
std::unique_ptr<Network> parseHeader(std::string line);

/**
 * \brief load a network from a stream
 * 
 * \param stream
 * \return the loaded network
 * \throw LoadException if the contents are invalid
 */
std::unique_ptr<Network> loadFromStream(std::istream& stream);


std::unique_ptr<Network> Analyzer::Network::loadFromFile(std::string fname) {
	if (fname == "-") {
		return loadFromStream(std::cin);
	}
	else {
		std::ifstream fileStream(fname);
		if (!fileStream) {
			char buff[200];
			strerror_s(buff, 200);
			throw LoadException(fmt::format("Error opening file '{}': {}", fname, buff));
		}
		return loadFromStream(fileStream);
	}
}

std::unique_ptr<Network> loadFromStream(std::istream& stream)
{
	std::string line;
	while (!line.length() || line[0] == '#')
		std::getline(stream, line);
	auto network = parseHeader(line);


	bool run = true;
	while (std::getline(stream, line) && run)
	{
		// ignore empty lines and comments
		if (line.empty() || line[0] == '#')
			continue;

		// stop on any line starting with 'END'
		if (line.rfind("END", 0) == 0) {
			if (network->B != network->getBranchCount()) {
				throw LoadException(fmt::format("Load error: END but less then B={} branches were added!", network->B));
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

std::unique_ptr<Network> parseHeader(std::string line) {
	unsigned N, B;
	INetworkSolver* solver = nullptr;
	std::string analysisType;

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
			unsigned mode;
			iss >> mode;
			if (!iss) {
				mode = EquationSystemSolver::MODE_DEFAULT;
			}
			solver = new EquationSystemSolver(mode);
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
			throw LoadException(fmt::format("Error: invalid analysis type '{}' (valid options are 'DC' (default), 'GEN', 'TWOPORT', 'EQ' or 'RES')", analysisType));
		}
	}
	else {
		// default to DC
		solver = new NetworkSolverDC();
	}

	std::unique_ptr<INetworkSolver> solverPtr(solver);

	if (solver->validate(N, B).length()) {
		throw LoadException(fmt::format("Error: network solver validation failed: '{}'", solver->validate(N, B)));
	}

	return std::unique_ptr<Network>(new Network(N, B, std::move(solverPtr)));
}
