#pragma once

#include <memory>

namespace LinMath {
	class Matrix
	{
		std::unique_ptr<double[]> data;
	public:
		const unsigned rows, columns;
		Matrix(unsigned rows, unsigned columns);
		Matrix(const Matrix&);
		Matrix invert() const;
		double& operator()(unsigned row, unsigned column);
		const double& operator()(unsigned row, unsigned column) const;
		Matrix operator*(const Matrix& rhs);
		Matrix subMatrix(unsigned, unsigned, unsigned, unsigned);
		friend std::ostream& operator<<(std::ostream&, Matrix&);
	};
}