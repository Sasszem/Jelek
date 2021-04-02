#include "DimensionException.h"

DimensionException::DimensionException(std::string msg)
{
	this->message = msg;
}

const char* DimensionException::what() const
{
	return message.c_str();
}
