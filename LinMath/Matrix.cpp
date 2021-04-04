#include "Matrix.h"
#include "DimensionException.h"
#include "../fmt/core.h"
#include <algorithm>

using namespace LinMath;

Matrix::Matrix(unsigned rows, unsigned columns) : rows(rows), columns(columns)
{
	for (unsigned r = 0; r < rows; r++)
		data.push_back(new LinVector(columns));
}

Matrix::Matrix(const Matrix& other): rows(other.rows), columns(other.columns)
{
	for (auto it = other.data.begin(); it != other.data.end(); it++)
		data.push_back(new LinVector(**it));
}

Matrix::~Matrix()
{
	for (auto i = data.begin(); i != data.end(); i++)
		delete* i;
}

Matrix Matrix::invert() const
{
	// invert matrix by gaussian elimination with an identity matrix

	// we can only invert square matrices
	if (rows != columns) {
		throw new DimensionException(fmt::format("Can't invert a non-square ({}x{}) matrix!!", rows, columns));
	}

	Matrix m(rows, columns * 2);
	// copy matrix and fill right side with identity matrix
	for (unsigned r = 0; r < rows; r++)
		for (unsigned c = 0; c < columns; c++)
		{
			m[r][c] = (*this)[r][c];
			m[r][c + columns] = r == c ? 1.0 : 0.0;
		}
	

	// aussian elimination column by column
	// 1. find a row r in i..rows such as m[r][i]!=0
	// 2. swap that row to be the ith row
	// 3. normalize the row (divide by m[i][i])
	// 4. subtract m[j][i] times the row from every OTHER row ("kill" other lines)

	for (unsigned i = 0; i < columns; i++) {
		// find a row
		bool found = false;
		unsigned r = 0;
		for (unsigned rc = i; rc < rows; rc++) {
			if (m[rc][i] != 0) {
				r = rc;
				found = true;
				break;
			}
		}
		if (!found) {
			// matrix can not be inverted
			throw std::runtime_error(fmt::format("Error: matrix can not be inverted!"));
		}

		// swap that with line i
		if (r!=i)
			std::iter_swap(m.data.begin() + r, m.data.begin() + i);
		
		// normalize row
		m[i] *= (1 / m[i][i]);

		// kill every other row
		for (unsigned rc = 0; rc < rows; rc++)
			if (rc!= i)
				m[rc] -= m[i] * m[rc][i];
	}

	// copy the result matrix into it's own
	Matrix resultMatrix(rows, rows);

	for (unsigned r = 0; r < rows; r++)
		for (unsigned c = 0; c < rows; c++)
			resultMatrix[r][c] = m[r][c + rows];

	return resultMatrix;
}

LinVector& Matrix::operator[](unsigned index)
{
	if (index >= rows) {
		throw new DimensionException(fmt::format("Can't access row {} of a {}x{} matrix!", index, rows, columns));
	}
	return *data[index];
}

const LinVector& LinMath::Matrix::operator[](unsigned index) const
{
	if (index >= rows) {
		throw new DimensionException(fmt::format("Can't access row {} of a {}x{} matrix!", index, rows, columns));
	}
	return *data[index];
}

LinVector LinMath::Matrix::operator*(LinVector rhs)
{
	if (rhs.getDim() != columns) {
		throw new DimensionException(fmt::format("Can't multiply a {}x{} matrix with a {} dimensional vector!", rows, columns, rhs.getDim()));
	}

	LinVector resVec(rows);

	for (unsigned i = 0; i < rows; i++) {
		resVec[i] = (*this)[i] * rhs;
	}

	return resVec;
}
