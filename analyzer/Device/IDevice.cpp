#include "IDevice.h"
#include <fmt/core.h>

using Analyzer::Device::IDevice;

unsigned IDevice::currentIndexFromId(unsigned id)
{
	return 2 * id - 1;
}
unsigned IDevice::voltageIndexFromId(unsigned id)
{
	return 2 * id - 2;
}
IDevice::IDevice(unsigned id, unsigned plus, unsigned minus) : port_plus(plus), port_minus(minus), id(id)
{
}

unsigned IDevice::other_port(unsigned port)
{
	if (port == port_plus)
		return port_minus;
	if (port == port_minus)
		return port_plus;
	return 0;
}

int IDevice::sign(unsigned port)
{
	return port == port_plus ? 1 : -1;
}

unsigned IDevice::currentIndex()
{
	return IDevice::currentIndexFromId(id);
}

unsigned IDevice::voltageIndex()
{
	return IDevice::voltageIndexFromId(id);
}

IDevice::~IDevice()
{
}
