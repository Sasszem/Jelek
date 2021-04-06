#include "DimensionException.h"

LinMath::DimensionException::DimensionException(std::string msg): std::exception(msg.c_str())
{
}