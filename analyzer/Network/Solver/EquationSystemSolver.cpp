#include "EquationSystemSolver.h"
#include <iostream>

Analyzer::Network::Solvers::EquationSystemSolver::EquationSystemSolver(unsigned mode) : printMode(mode <= 2 && mode >= 1 ? mode : 1)
{
}

void printExtended(const LinMath::Matrix& eq) {
	std::cout << "Extended matrix:" << std::endl;
	std::cout << eq;
}

void printMatlab(const LinMath::Matrix& eq) {
	std::cout << "%% [SECTION NAME]" << std::endl;
	std::cout << "% equation matrix and vector imported from network analyzer" << std::endl;
	std::cout << "% in the form A*x = B" << std::endl;
	std::cout << std::endl;
	std::cout << "A = [ ..." << std::endl;;
	for (unsigned r = 0; r < eq.rows; r++) {
		for (unsigned c = 0; c < eq.columns - 1; c++) {
			std::cout << eq(r, c);
			std::cout << (c == eq.columns - 2 ? ';' : ',');
		}
		std::cout << " ..." << std::endl;
	}
	std::cout << "];" << std::endl;
	std::cout << std::endl;
	std::cout << "B = [";
	for (unsigned r = 0; r < eq.rows; r++) {
		std::cout << eq(r, eq.columns - 1) << (r == eq.rows - 1 ? "" : ";");
	}
	std::cout << "];" << std::endl;
	std::cout << std::endl;
	std::cout << "% end of imported equation sytem" << std::endl;
	std::cout << std::endl;
}

LinMath::Matrix Analyzer::Network::Solvers::EquationSystemSolver::solve(LinMath::Matrix& eq) const
{
	if (printMode == 1)
		printExtended(eq);
	else if (printMode == 2)
		printMatlab(eq);
	return LinMath::Matrix(0, 0);
}

std::string Analyzer::Network::Solvers::EquationSystemSolver::print() const
{
	return "EquationSystemSolver()";
}
