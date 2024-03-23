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

Vector3 utility::operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
}

Vector3 utility::operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	return result;
}

Vector3 utility::operator/(const Vector3& v1, const double c)
{
	Vector3 result;

	result.x = v1.x / c;
	result.y = v1.y / c;
	result.z = v1.z / c;

	return result;
}

Vector3 utility::operator*(const double c, const Vector3& v1)
{
	Vector3 result;

	result.x = v1.x * c;
	result.y = v1.y * c;
	result.z = v1.z * c;

	return result;
}

bool utility::operator==(const Vector3& v1, const Vector3& v2)
{
	if (v1.x < v2.x - 1e-10 && v1.x > v2.x + 1e-10) {
		return false;
	}

	if (v1.y < v2.y - 1e-10 && v1.y > v2.y + 1e-10) {
		return false;
	}

	if (v1.z < v2.z - 1e-10 && v1.z > v2.z + 1e-10) {
		return false;
	}

	return true;
}