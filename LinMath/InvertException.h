#pragma once
#include <stdexcept>

namespace LinMath {
	class InvertException : public std::exception
	{
	public:
		InvertException();
	};
}