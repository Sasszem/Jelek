#pragma once
#include <stdexcept>

namespace Analyzer
{
	namespace Network {
		/**
		 * Exception thrown when the network can not be solver for some reason.
		 */
		class SolveException : public std::exception
		{
		public:
			/**
			 * Default constructor with message.
			 * 
			 * \param message
			 */
			SolveException(std::string message);
		};
	}
}