#pragma once

#include "../LinMath/LinVector.h"

class IDevice
{
protected:
	static unsigned currentIndexFromId(unsigned);
	static unsigned voltageIndexFromId(unsigned);
public:
	const unsigned port_plus, port_minus;
	const unsigned id;
	IDevice(unsigned id, unsigned plus, unsigned minus);
	unsigned other_port(unsigned);
	int sign(unsigned);
	virtual double equation(LinMath::LinVector&) = 0;
	unsigned currentIndex();
	unsigned voltageIndex();

	virtual std::string print() = 0;
	~IDevice();
};

