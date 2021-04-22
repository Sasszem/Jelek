#pragma once
#include <stdexcept>

namespace Analyzer
{
	namespace Network {
		/**
		 * \brief Exception thrown when the network can not be solver for some reason.
		 */
		class SolveException : public std::exception
		{
		public:
			/**
			 * \brief Default constructor with message.
			 * 
			 * \param message
			 */
			SolveException(std::string message);
		};
	}
}