#include "LinearEquationSystem.h"

LinMath::LinearEquationSystem::LinearEquationSystem(unsigned unknowns): matrix(unknowns, unknowns), freeVector(unknowns, 1)
{
}

LinMath::Matrix& LinMath::LinearEquationSystem::getMatrix()
{
	return matrix;
}

LinMath::Matrix& LinMath::LinearEquationSystem::getVector()
{
	return freeVector;
}
