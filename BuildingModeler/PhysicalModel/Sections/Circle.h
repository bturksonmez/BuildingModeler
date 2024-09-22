#pragma once

#include "Shape.h"

namespace physicalModel
{
	class Circle : public Shape
	{
	private:
		double m_r;

		void calculateA();
		void calculateIyy();
		void calculateIzz();
		void calculateJ();

	public:
		Circle(double r);
		Circle() = delete;
		~Circle() {}
	};
}
