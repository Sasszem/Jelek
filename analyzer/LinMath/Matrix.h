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
		Matrix operator*(const Matrix& rhs) const;
		Matrix subMatrix(unsigned, unsigned, unsigned, unsigned) const;
		Matrix transpose() const;
		Matrix operator+(const Matrix&) const;
		Matrix operator*(double) const;
		Matrix operator-(const Matrix&) const;
		friend std::ostream& operator<<(std::ostream&, const Matrix&);
	};
}