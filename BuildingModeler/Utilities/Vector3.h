#pragma once

#include <iostream>
#include <vector>

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
		~Vector3() {}

		double norm() const;
		double norm2() const;
	};
}