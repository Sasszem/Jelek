#include "CurrentSource.h"
#include "../fmt/core.h"

CurrentSource::CurrentSource(unsigned id, unsigned port_plus, unsigned port_minus, double current) : IDevice(id, port_plus, port_minus), current(current)
{
}

double CurrentSource::equation(LinMath::Matrix& vec) {
	vec(id-1, currentIndex()) = 1;
	return current;
}

std::string CurrentSource::print()
{
	return fmt::format("CurrentSource(id={}, plus={}, minus={}, current={})", id, port_plus, port_minus, current);
}