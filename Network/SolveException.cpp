#include "SolveException.h"

Analyzer::Network::SolveException::SolveException(std::string str) : std::exception(str.c_str())
{
}
