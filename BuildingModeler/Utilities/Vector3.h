#pragma once

#include <iostream>
#include <vector>
#include <cmath>

namespace utility
{
	class Vector3
	{
	public:
		double x = 0.0;
		double y = 0.0;
		double z = 0.0;

		Vector3() {}
		Vector3(double x, double y, double z);
		Vector3(const Vector3& other) = default;
		Vector3& operator=(const Vector3& other) = default;
		Vector3(Vector3&& other) = default;
		Vector3& operator=(Vector3&& other) = default;
		~Vector3() {}

		double norm() const;
		double norm2() const;
		friend Vector3 operator+(const Vector3& v1, const Vector3& v2);
		friend Vector3 operator-(const Vector3& v1, const Vector3& v2);
		friend Vector3 operator/(const Vector3& v1, const double c);
		friend Vector3 operator*(const double c, const Vector3& v1);
		friend bool operator== (const Vector3& v1, const Vector3& v2);
	};
}