#pragma once

#include <stdexcept>

namespace Analyzer {
	namespace Network {
		class LoadException : public std::exception
		{
		public:
			LoadException(std::string);
		};
	}
}