#pragma once

#include <iostream>
#include <vector>

namespace utility
{
	class Vector2
	{
	public:
		double x = 0.0;
		double y = 0.0;

		Vector2() {}
		Vector2(double x, double y);
		Vector2(const Vector2& other) = default;
		Vector2& operator=(const Vector2& other) = default;
		Vector2(Vector2&& other) = default;
		Vector2& operator=(Vector2&& other) = default;
		~Vector2() {}

		double norm() const;
		double norm2() const;
		friend Vector2 operator+(const Vector2& v1, const Vector2& v2);
		friend Vector2 operator-(const Vector2& v1, const Vector2& v2);
	};
}