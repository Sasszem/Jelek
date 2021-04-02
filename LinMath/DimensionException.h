#pragma once
#include <stdexcept>
class DimensionException: public std::exception
{
	std::string message;
public:
	DimensionException(std::string message);
	const char* what() const;
};

