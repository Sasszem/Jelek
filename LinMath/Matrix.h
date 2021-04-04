#pragma once

#include "LinVector.h"
#include <vector>


namespace LinMath {
	class Matrix
	{
		const unsigned rows, columns;
		std::vector<LinVector*> data;
	public:
		Matrix(unsigned rows, unsigned columns);
		Matrix(const Matrix&);
		~Matrix();
		Matrix invert() const;
		LinVector& operator[](unsigned index);
		const LinVector& operator[](unsigned index) const;
		LinVector operator*(LinVector rhs);
	};
}