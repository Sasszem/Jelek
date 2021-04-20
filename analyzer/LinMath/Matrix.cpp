#include <LinMath/math.h>
#include <fmt/core.h>
#include <algorithm>
#include <iostream>

using namespace LinMath;

Matrix::Matrix(unsigned rows, unsigned columns) : rows(rows), columns(columns), data(new double[rows * columns])
{
	for (unsigned row = 0; row < rows; row++)
		for (unsigned col = 0; col < columns; col++)
			(*this)(row, col) = 0.0;
}

Matrix::Matrix(const Matrix& other) : rows(other.rows), columns(other.columns), data(new double[other.rows * other.columns])
{
	for (unsigned row = 0; row < rows; row++)
		for (unsigned col = 0; col < columns; col++)
			(*this)(row, col) = other(row, col);
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
			m(r, c) = (*this)(r, c);
			m(r, c + columns) = r == c ? 1.0 : 0.0;
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
			if (m(rc, i) != 0) {
				r = rc;
				found = true;
				break;
			}
		}
		if (!found) {
			// matrix can not be inverted
			throw InvertException();
		}

		// swap that with line i
		if (r != i) {
			for (unsigned c = 0; c < columns * 2; c++) {
				double tmp = m(r, c);
				m(r, c) = m(i, c);
				m(i, c) = tmp;
			}
		}

		// normalize row
		for (unsigned c = columns * 2 - 1; c > i; c--)
			m(i, c) *= (1 / m(i, i));
		m(i, i) = 1.0;

		// kill every other row
		for (unsigned rc = 0; rc < rows; rc++) {
			if (rc != i) {
				double mul = m(rc, i);
				for (unsigned c = 0; c < columns * 2; c++)
					m(rc, c) -= m(i, c) * mul;
			}
		}

	}

	// copy the result matrix into it's own
	Matrix resultMatrix(rows, rows);

	for (unsigned r = 0; r < rows; r++)
		for (unsigned c = 0; c < rows; c++)
			resultMatrix(r, c) = m(r, c + rows);

	return resultMatrix;
}

double& LinMath::Matrix::operator()(unsigned row, unsigned column)
{
	if (row >= rows || column >= columns) {
		throw DimensionException(fmt::format("Can not get element ({}, {}) of a {}x{} matrix!", row, column, rows, columns));
	}
	return data[row * columns + column];
}

const double& LinMath::Matrix::operator()(unsigned row, unsigned column) const
{
	if (row >= rows || column >= columns) {
		throw DimensionException(fmt::format("Can not get element ({}, {}) of a {}x{} matrix!", row, column, rows, columns));
	}
	return data[row * columns + column];
}

Matrix LinMath::Matrix::operator*(const Matrix& rhs) const
{
	if (rhs.rows != columns) {
		throw DimensionException(fmt::format("Can not multiply {}x{} and {}x{} matrices!", rows, columns, rhs.rows, rhs.columns));
	}

	Matrix result(rows, rhs.columns);
	for (unsigned r = 0; r < rows; r++) {
		for (unsigned c = 0; c < rhs.columns; c++) {
			double dotP = 0.0;
			for (unsigned x = 0; x < columns; x++) {
				dotP += (*this)(r, x) * rhs(x, c);
			}
			result(r, c) = dotP;
		}
	}

	return result;
}

Matrix Matrix::subMatrix(unsigned startRow, unsigned startColumn, unsigned height, unsigned width) const {
	if (startRow >= rows || startColumn >= columns || startRow + height > rows || startColumn + width > columns) {
		throw DimensionException(fmt::format("Error: could not extract {}x{} rectangle starting at row {} and column {} from a {}x{} matrix!", height, width, startRow, startColumn, rows, columns));
	}
	Matrix res(height, width);
	for (unsigned r = 0; r < height; r++) {
		for (unsigned c = 0; c < width; c++) {
			res(r, c) = (*this)(startRow + r, startColumn + c);
		}
	}
	return res;
}

Matrix Matrix::transpose() const {
	Matrix result(columns, rows);
	for (unsigned r = 0; r < rows; r++)
		for (unsigned c = 0; c < columns; c++)
			result(c, r) = (*this)(r, c);
	return result;
}

Matrix Matrix::operator+(const Matrix& rhs) const {
	if (rhs.rows != rows || rhs.columns != columns) {
		throw DimensionException(fmt::format("Can not add or subtract {}x{} and {}x{} matrices", rows, columns, rhs.rows, rhs.columns));
	}

	Matrix result(rows, columns);
	for (unsigned r = 0; r < rows; r++)
		for (unsigned c = 0; c < columns; c++)
			result(r, c) = (*this)(r, c) + rhs(r, c);
	return result;
}

Matrix Matrix::operator*(double scalar) const {
	Matrix result(rows, columns);
	for (unsigned r = 0; r < rows; r++)
		for (unsigned c = 0; c < columns; c++)
			result(r, c) = scalar * (*this)(r, c);
	return result;
}

Matrix Matrix::operator-(const Matrix& rhs) const {
	return (*this) + rhs * (-1.0);
}

namespace LinMath {
	std::ostream& operator<<(std::ostream& stream, const LinMath::Matrix& m)
	{
		stream << "[" << std::endl;
		for (unsigned r = 0; r < m.rows; r++) {
			for (unsigned c = 0; c < m.columns; c++) {
				stream << "\t" << m(r, c);
			}
			stream << std::endl;
		}
		stream << "]" << std::endl;
		return stream;
	}
}