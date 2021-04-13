#include "LoadException.h"

Analyzer::Network::LoadException::LoadException(std::string str) : std::exception(str.c_str())
{
}
