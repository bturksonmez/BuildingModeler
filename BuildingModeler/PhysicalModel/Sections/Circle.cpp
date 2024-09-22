#include "Circle.h"

#include <cmath>

using namespace physicalModel;

Circle::Circle(double r) : Shape()
{
	m_r = r;
	m_shapeType = ShapeType::CIRCLE;

	calculateA();
	calculateIyy();
	calculateIzz();
	calculateJ();
}

void Circle::calculateA()
{
	m_A = 3.141593 * std::pow(m_r, 2.0);
}

void Circle::calculateIyy()
{
	m_Iyy = 3.141593 * std::pow(m_r, 4.0) / 4.0;
}

void Circle::calculateIzz()
{
	m_Izz = 3.141593 * std::pow(m_r, 4.0) / 4.0;
}

void Circle::calculateJ()
{
	m_J = 3.141593 * std::pow(m_r, 4.0) / 2.0;
}

