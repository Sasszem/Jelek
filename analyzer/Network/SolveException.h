#pragma once
#include <stdexcept>

namespace Analyzer
{
	namespace Network {
		class SolveException : public std::exception
		{
		public:
			SolveException(std::string);
		};
	}
}