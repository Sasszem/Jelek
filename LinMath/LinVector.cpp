#include "LinVector.h"
#include "DimensionException.h"
#include "../fmt/core.h"

namespace LinMath {

	unsigned LinVector::getDim()
	{
		return dim;
	}

	LinVector::LinVector(unsigned dim)
	{
		this->dim = dim;
		this->data = new double[dim];
		for (unsigned i = 0; i < dim; i++)
			data[i] = 0.0;
	}

	LinVector::LinVector(const std::initializer_list<double>& values)
	{
		dim = values.size();
		data = new double[dim];
		unsigned idx = 0;
		for (auto i = values.begin(); i != values.end(); i++)
			data[idx++] = *i;
	}

	LinVector::~LinVector()
	{
		delete[] data;
	}

	LinVector::LinVector(const LinVector& other)
	{
		dim = other.dim;
		data = new double[dim];
		for (unsigned i = 0; i < dim; i++)
			data[i] = other.data[i];
	}

	double& LinVector::operator[](unsigned index)
	{
		if (index >= dim) {
			throw new DimensionException(fmt::format("Can't access element {} of a {} dimensional vector!", index, dim));
		}
		return data[index];
	}

	const double& LinVector::operator[](unsigned index) const
	{
		if (index >= dim) {
			throw new DimensionException(fmt::format("Can't get element {} of a {} dimensional vector!", index, dim));
		}
		return data[index];
	}

	LinVector LinVector::operator+(const LinVector& rhs) const
	{
		if (rhs.dim != dim) {
			throw new DimensionException(fmt::format("Can't add {} and {} dimension vectors!", dim, rhs.dim));
		}
		LinVector ret = *this;
		ret -= rhs;
		return ret;
	}

	LinVector LinVector::operator-(const LinVector& rhs) const
	{
		return *this + rhs * (-1.0);
	}

	double LinVector::operator*(const LinVector& rhs) const
	{
		if (rhs.dim != dim) {
			throw new DimensionException(fmt::format("Can't take dot product of {} and {} dimension vectors!", dim, rhs.dim));
		}
		double dotProduct = 0.0;
		for (unsigned i = 0; i < dim; i++)
			dotProduct += data[i] * rhs[i];
		return dotProduct;
	}

	LinVector LinVector::operator*(double scalar) const
	{
		LinVector ret = *this;
		ret *= scalar;
		return ret;
	}

	void LinVector::operator*=(double scalar)
	{
		for (unsigned i = 0; i < dim; i++)
			data[i] *= scalar;
	}

	void LinVector::operator+=(const LinVector rhs)
	{
		if (rhs.dim != dim) {
			throw new DimensionException(fmt::format("Can't add {} and {} dimension vectors!", dim, rhs.dim));
		}
		for (unsigned i = 0; i < dim; i++)
			data[i] += rhs[i];
	}

	void LinVector::operator-=(const LinVector other)
	{
		// don't need to check as the += operator does thar
		*this += other * (-1.0);
	}

	std::ostream& operator<<(std::ostream& stream, const LinVector& vec)
	{
		stream << vec.dim << ":[";
		for (double i = 0; i < vec.dim; i++) {
			stream << vec[i];
			if (i < vec.dim - 1)
				stream << ", ";
		}
		stream << "]";
		return stream;
	}
}