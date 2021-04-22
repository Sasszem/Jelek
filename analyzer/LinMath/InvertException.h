#pragma once
#include <stdexcept>

namespace LinMath {
	/**
	 * \brief The matrix could not be inverted because it was singular.
	 */
	class InvertException : public std::exception
	{
	public:
		/**
		 * \brief Default constructor.
		 * 
		 */
		InvertException();
	};
}