#pragma once
#include <stdexcept>

namespace LinMath {
	/**
	 * The matrix could not be inverted because it was singular.
	 */
	class InvertException : public std::exception
	{
	public:
		/**
		 * Default constructor.
		 * 
		 */
		InvertException();
	};
}