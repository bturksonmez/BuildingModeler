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
		~Vector2() {}

		double norm() const;
		double norm2() const;
	};
}