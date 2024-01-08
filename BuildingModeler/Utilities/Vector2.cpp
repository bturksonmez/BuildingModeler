#include "Vector2.h"

using namespace utility;

Vector2::Vector2(double x, double y) : x(x), y(y)
{
}

double Vector2::norm() const
{
	return std::abs(x) + std::abs(y);
}

double Vector2::norm2() const
{
	return std::sqrt(std::pow(x, 2.0) + std::pow(y, 2.0));
}

Vector2 utility::operator+(const Vector2& v1, const Vector2& v2)
{
	Vector2 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;

	return result;
}

Vector2 utility::operator-(const Vector2& v1, const Vector2& v2)
{
	Vector2 result;

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;

	return result;
}