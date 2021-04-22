#pragma once

#include <stdexcept>

namespace Analyzer {
	namespace Network {
		/**
		 * \brief Exception thrown when a loading error is encountered.
		 */
		class LoadException : public std::exception
		{
		public:
			/**
			 * \brief Default constructor with message.
			 * 
			 * \param message
			 */
			LoadException(std::string message);
		};
	}
}