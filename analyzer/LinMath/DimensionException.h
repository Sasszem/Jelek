#pragma once
#include <stdexcept>

namespace LinMath {
	class DimensionException : public std::exception
	{
	public:
		DimensionException(std::string message);
	};
}