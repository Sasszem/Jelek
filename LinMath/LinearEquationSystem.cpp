#include "LinearEquationSystem.h"

LinMath::LinearEquationSystem::LinearEquationSystem(unsigned unknowns): matrix(unknowns, unknowns), freeVector(unknowns)
{
}

LinMath::Matrix& LinMath::LinearEquationSystem::getMatrix()
{
	return matrix;
}

LinMath::LinVector& LinMath::LinearEquationSystem::getVector()
{
	return freeVector;
}
