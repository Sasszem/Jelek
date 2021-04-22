#pragma once

#include <memory>

/// \brief namespace to hold linear algebra releated classes
/// use the batch import file <LinMath/math.h> to include!
namespace LinMath {
	/**
	* @brief a simple matrix class for linear calculations
	*
	* This is a simple matrix class. It's not a general matrix, as it only implements the operations required by this project.
	*/
	class Matrix
	{
		/// matrix data as a rows x columns array
		std::unique_ptr<double[]> data;
	public:
		/// rows of this matrix
		const unsigned rows;
		/// columns of this matrix
		const unsigned columns;
		/**
		 * \brief Create an empty (zeros) matrix with a given size.
		 * 
		 * \param rows
		 * \param columns
		 */
		Matrix(unsigned rows, unsigned columns);
		/**
		 * \brief Deep-copy another matrix.
		 * 
		 * \param other
		 */
		Matrix(const Matrix& other);

		/**
		 * \brief Invert the matrix.
		 * 
		 * \return the inverted matrix
		 * \throw InvertException if the matrix is singular
		 */
		Matrix invert() const;

		/**
		 * \brief Acces an element of the matrix. 
		 * 
		 * Uses 0-based indexing!
		 * 
		 * \param row
		 * \param column
		 * \return a writable reference to that element
		 * \throw DimensionException if row or column is invalid
		 */
		double& operator()(unsigned row, unsigned column);

		/**
		 * \brief Read an element of the matrix. 
		 * 
		 * Uses 0-based indexing!
		 * 
		 * \param row
		 * \param column
		 * \return a const reference to the specified element
		 * \throw DimensionException if row or column is invalid
		 */
		const double& operator()(unsigned row, unsigned column) const;

		/**
		 * \brief Multiply this matrix from the right with another (matrix-matrix multiplication).
		 * 
		 * \param rhs the other matrix to multiply with
		 * \return the result of the multiplication
		 * \throw DimensionException if the the matrices are nonconformant
		 */
		Matrix operator*(const Matrix& rhs) const;

		/**
		 * \brief Extract a continous rectangular submatrix from this matrix.
		 * 
		 * \param startRow the row to start from
		 * \param startColumn the column to start from
		 * \param height the number of rows to extract
		 * \param width the number of columns to extract
		 * \return the extrafted submatrix
		 * \throw DimensionException of the rectangular submatrix is not valid
		 */
		Matrix subMatrix(unsigned startRow, unsigned startColumn, unsigned height, unsigned width) const;

		/**
		 * \brief Transpose the matrix (swap columns with rows).
		 * 
		 * \return the transposed matrix
		 */
		Matrix transpose() const;

		/**
		 * \brief Add two matrices (element by element).
		 * 
		 * \param rhs the matrix to add
		 * \return the sum of the matrices
		 * \throw DimensionException if the matrices sizes does not match
		 */
		Matrix operator+(const Matrix& rhs) const;

		/**
		 * \brief Multiply the matrix by a scalar.
		 * 
		 * \param scalar
		 * \return the scaled matrix
		 */
		Matrix operator*(double scalar) const;

		/**
		 * \brief Subtract matrices.
		 * 
		 * \param rhs matrix to subtract from this one
		 * \return the result
		 * \throw DimensionException if the matrices sized does not match
		 */
		Matrix operator-(const Matrix& rhs) const;

		/**
		 * \brief print the matrix in a human-readable format.
		 * \param stream stream to print into
		 * \param m matrix to print
		 * \return the stream for chaining
		 */
		friend std::ostream& operator<<(std::ostream& stream, const LinMath::Matrix& m);
	};
}