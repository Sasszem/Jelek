#include "NetworkSolverTwoport.h"

#include <LinMath/InvertException.h>
#include <fmt/core.h>
#include <iostream>
#include <algorithm>
#include <limits>

using Analyzer::Network::Solvers::NetworkSolverTwoport;

NetworkSolverTwoport::NetworkSolverTwoport(unsigned primaryID, unsigned secondaryID) : primary(primaryID), secondary(secondaryID)
{
}

/**
 * \brief generate a permute matrix.
 * 
 * It will have 1s in the specified columns for the first rows, one in each row
 * it will be a N x N matrix
 * 
 * \param n number of rows & columns of the result matrix
 * \param targets target columns to place 1s into
 * \return generated permute matrix
 */
LinMath::Matrix permuteMatrix(unsigned n, LinMath::Matrix targets);

/**
 * \brief solve for 2 unknowns from 2 others.
 * 
 * Uses a permute matrix to reorder, then uses simple matrix arithmetics
 * Assumes no constant is added
 * 
 * \param for1
 * \param for2
 * \param from1
 * \param from2
 * \param eq equation system to solve in
 * \return 2x2 matrix
 */
LinMath::Matrix solve_for(unsigned for1, unsigned for2, unsigned from1, unsigned from2, LinMath::Matrix& eq);

/**
 * \brief copy a 2x2 matrix into a bigger matrix starting at row.
 * 
 * \param from matrix to copy from
 * \param to matrix to copy to
 * \param row copy to that row
 */
void copy_matrix(LinMath::Matrix& from, LinMath::Matrix& to, unsigned row);

/**
 * \brief solve_for but catch any exception and NaN the matrix instead.
 * 
 * \param for1
 * \param for2
 * \param from1
 * \param from2
 * \param eq
 * \return 
 */
LinMath::Matrix solve_or_nan(unsigned for1, unsigned for2, unsigned from1, unsigned from2, LinMath::Matrix& eq);

LinMath::Matrix NetworkSolverTwoport::solve(LinMath::Matrix& eqOrig) const
{
	// remove 2 equations
	LinMath::Matrix target(2, 1);
	target(0, 0) = primary - 1;
	target(1, 0) = secondary - 1;

	auto pM = permuteMatrix(eqOrig.rows, target);

	LinMath::Matrix eq = (pM * eqOrig).subMatrix(2, 0, eqOrig.rows - 2, eqOrig.columns);

	for (unsigned r = 0; r < eq.rows; r++) {
		eq(r, 2 * primary - 1) *= -1.0;
		eq(r, 2 * secondary - 1) *= -1.0;
	}

	LinMath::Matrix result(12, 2);

	auto impedance = solve_or_nan(2 * primary - 2, 2 * secondary - 2, 2 * primary - 1, 2 * secondary - 1, eq).subMatrix(0, 0, 2, 2);
	copy_matrix(impedance, result, 0);

	auto conductance = solve_or_nan(2 * primary - 1, 2 * secondary - 1, 2 * primary - 2, 2 * secondary - 2, eq).subMatrix(0, 0, 2, 2);
	copy_matrix(conductance, result, 2);

	auto hybrid = solve_or_nan(2 * primary - 2, 2 * secondary - 1, 2 * primary - 1, 2 * secondary - 2, eq).subMatrix(0, 0, 2, 2);
	copy_matrix(hybrid, result, 4);

	auto invhybrid = solve_or_nan(2 * primary - 1, 2 * secondary - 2, 2 * primary - 2, 2 * secondary - 1, eq).subMatrix(0, 0, 2, 2);
	copy_matrix(invhybrid, result, 6);


	// chain type characteristics use a differenc reference direction for i2
	for (unsigned r = 0; r < eq.rows; r++) {
		eq(r, 2 * secondary - 1) *= -1.0;
	}

	auto chain = solve_or_nan(2 * primary - 2, 2 * primary - 1, 2 * secondary - 2, 2 * secondary - 1, eq).subMatrix(0, 0, 2, 2);
	copy_matrix(chain, result, 8);

	auto invchain = solve_or_nan(2 * secondary - 2, 2 * secondary - 1, 2 * primary - 2, 2 * primary - 1, eq).subMatrix(0, 0, 2, 2);
	copy_matrix(invchain, result, 10);

	return result;
}

std::string NetworkSolverTwoport::print() const
{
	return fmt::format("NetworkSolverTwoport(primary={}, secondary={})", primary, secondary);
}

std::string Analyzer::Network::Solvers::NetworkSolverTwoport::validate(unsigned N, unsigned B)
{
	if (primary == 0 || primary > B) {
		return fmt::format("primary device {} is invalid!", primary);
	}
	if (secondary == 0 || secondary > B) {
		return fmt::format("secondary device {} is invalid!", secondary);
	}
	if (primary == secondary) {
		return fmt::format("TWOPORT's ports can't be the same!");
	}
	return "";
}

LinMath::Matrix solve_for(unsigned for1, unsigned for2, unsigned from1, unsigned from2, LinMath::Matrix& eq) {
	// swap in lines, chop up matrix, invert and multiply

	auto mat = eq.subMatrix(0, 0, eq.rows, eq.columns - 1);
	auto vec = eq.subMatrix(0, eq.columns - 1, eq.rows, 1);

	// swap columns
	// for1, for2, from1, from2
	LinMath::Matrix ptarget(4, 1);
	ptarget(0, 0) = from1;
	ptarget(1, 0) = from2;
	ptarget(2, 0) = for1;
	ptarget(3, 0) = for2;

	auto pM = permuteMatrix(mat.columns, ptarget).transpose();
	auto matPermuted = mat * pM;

	auto m1 = matPermuted.subMatrix(0, 0, mat.rows, 2);
	auto m2 = matPermuted.subMatrix(0, 2, mat.rows, mat.columns - 2);

	auto resM = m2.invert() * m1 * (-1);
	return resM;
}

LinMath::Matrix permuteMatrix(unsigned n, LinMath::Matrix targets) {
	LinMath::Matrix m(n, n);
	for (unsigned i = 0; i < targets.rows; i++) {
		m(i, (unsigned)targets(i, 0)) = 1.0;
	}
	unsigned r = targets.rows;
	for (unsigned c = 0; c < n; c++) {
		// write ONLY IF not in targets
		bool contains = false;
		for (unsigned i = 0; i < targets.rows; i++) {
			if (targets(i, 0) == c) {
				contains = true;
			}
		}

		if (!contains) {
			m(r++, c) = 1.0;
		}
	}
	return m;
}

void copy_matrix(LinMath::Matrix& from, LinMath::Matrix& to, unsigned row) {
	for (unsigned r = 0; r < 2; r++) {
		for (unsigned c = 0; c < 2; c++) {
			to(r + row, c) = from(r, c);
		}
	}
	// this could be 1 line shorter if I unrolled the loop by hand...
}

LinMath::Matrix solve_or_nan(unsigned for1, unsigned for2, unsigned from1, unsigned from2, LinMath::Matrix& eq) {
	try {
		LinMath::Matrix m = solve_for(for1, for2, from1, from2, eq);
		return m;
	}
	catch (const LinMath::InvertException&) {
		LinMath::Matrix m(2, 2);
		m(0, 0) = std::numeric_limits<double>::quiet_NaN();
		m(1, 0) = std::numeric_limits<double>::quiet_NaN();
		m(0, 1) = std::numeric_limits<double>::quiet_NaN();
		m(1, 1) = std::numeric_limits<double>::quiet_NaN();
		return m;
	}
}