#include "InvertException.h"

LinMath::InvertException::InvertException() : std::exception("Matrix can not be inverted!")
{
}
