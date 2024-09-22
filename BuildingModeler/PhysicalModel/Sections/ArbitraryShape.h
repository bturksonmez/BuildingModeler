#pragma once

#include "Shape.h"

namespace physicalModel
{
	class ArbitraryShape : public Shape
	{
	public:
		ArbitraryShape(double A, double Iyy, double Izz, double J);
		ArbitraryShape() = delete;
		~ArbitraryShape() {}
	};
}
