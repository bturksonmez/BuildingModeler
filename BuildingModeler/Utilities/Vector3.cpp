#include "Vector3.h"

using namespace utility;

Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z)
{
}

double Vector3::norm() const
{
	return std::abs(x) + std::abs(y) + std::abs(z);
}

double Vector3::norm2() const
{
	return std::sqrt(std::pow(x, 2.0) + std::pow(y, 2.0) + std::pow(z, 2.0));
}