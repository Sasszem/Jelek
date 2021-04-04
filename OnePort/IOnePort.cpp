#include "IOnePort.h"
#include "../fmt/core.h"
unsigned IOnePort::currentIndexFromId(unsigned id)
{
	return 2*id - 1;
}
unsigned IOnePort::voltageIndexFromId(unsigned id)
{
	return 2*id - 2;
}
IOnePort::IOnePort(unsigned id, unsigned plus, unsigned minus): port_plus(plus), port_minus(minus), id(id)
{
}

#include <stdexcept>

unsigned IOnePort::other_port(unsigned port)
{
	if (port == port_plus)
		return port_minus;
	if (port == port_minus)
		return port_plus;
	//throw new std::invalid_argument(fmt::format("Error: invalid arument for other_port(): {}, valid ports are: {} and {}", port, port_plus, port_minus));
}

int IOnePort::sign(unsigned port)
{
	return port==port_plus ? 1 : -1;
}

unsigned IOnePort::currentIndex()
{
	return IOnePort::currentIndexFromId(id);
}

unsigned IOnePort::voltageIndex()
{
	return IOnePort::voltageIndexFromId(id);
}

IOnePort::~IOnePort()
{
}
