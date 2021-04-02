#include "CurrentSource.h"
#include "../fmt/core.h"

CurrentSource::CurrentSource(unsigned id, unsigned port_plus, unsigned port_minus, double current) : IOnePort(id, port_plus, port_minus), current(current)
{
}

IOnePort* CurrentSource::copy() {
	return new CurrentSource(*this);
}

double CurrentSource::equation(LinMath::LinVector& vec) {
	vec[currentIndex()] = 1;
	return current;
}

void CurrentSource::print(std::ostream& stream)
{
	stream << fmt::format("CurrentSource(id={}, plus={}, minus={}, current={})", id, port_plus, port_minus, current) << std::endl;
}