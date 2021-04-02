#pragma once

#include <iostream>
#include <initializer_list>

namespace LinMath {
	class LinVector
	{
		unsigned dim;
		double* data;
	public:
		unsigned getDim();
		LinVector(unsigned dim);
		LinVector(const std::initializer_list<double>& values);
		~LinVector();
		LinVector(const LinVector& other);
		double& operator[](unsigned index);
		const double& operator[](unsigned index) const;
		LinVector operator+(const LinVector& rhs) const;
		LinVector operator-(const LinVector& rhs) const;
		double operator*(const LinVector& rhs) const;
		LinVector operator*(double scalar) const;
		void operator*=(double scalar);
		void operator+=(const LinVector other);
		void operator-=(const LinVector other);

		friend std::ostream& operator<<(std::ostream&, const LinVector&);
	};
}