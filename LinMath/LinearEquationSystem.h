#pragma once
#include "Matrix.h"

namespace LinMath {
	class LinearEquationSystem
	{
	private:
		Matrix matrix;
		Matrix freeVector;
	public:
		LinearEquationSystem(unsigned);
		Matrix& getMatrix();
		Matrix& getVector();
	};
}


