#pragma once
#include <stdexcept>

namespace LinMath {
	/**
	* \brief something dimension-related gone wrong
	* 
	 * Error class for matrix dimension errors (nonconformant matrix operations, wrong indexing, etc).
	 */
	class DimensionException : public std::exception
	{
	public:
		/**
		 * \brief Default constructor.
		 * 
		 * \param message provided message
		 */
		DimensionException(std::string message);
	};
}