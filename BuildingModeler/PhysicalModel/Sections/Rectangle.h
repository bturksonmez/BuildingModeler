#pragma once

#include "Shape.h"

namespace physicalModel
{
	class Rectangle : public Shape
	{
	private:
		double m_by;
		double m_bz;

		void calculateA();
		void calculateIyy();
		void calculateIzz();
		void calculateJ();

	public:
		Rectangle(double by, double bz);
		Rectangle() = delete;
		~Rectangle() {}
	};
}
