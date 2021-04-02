#pragma once
#include "LinVector.h"
#include "Matrix.h"

namespace LinMath {
	class LinearEquationSystem
	{
	private:
		Matrix matrix;
		LinVector freeVector;
	public:
		LinearEquationSystem(unsigned);
		Matrix& getMatrix();
		LinVector& getVector();
	};
}


