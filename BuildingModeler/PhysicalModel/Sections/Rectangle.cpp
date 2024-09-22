#include "Rectangle.h"

#include <cmath>

using namespace physicalModel;

Rectangle::Rectangle(double by, double bz) : Shape()
{
	m_by = by;
	m_bz = bz;
	m_shapeType = ShapeType::RECTANGLE;

	calculateA();
	calculateIyy();
	calculateIzz();
	calculateJ();
}

void Rectangle::calculateA()
{
	m_A = m_by * m_bz;
}

void Rectangle::calculateIyy()
{
	m_Iyy = (1.0 / 12.0) * m_by * std::pow(m_bz, 3.0);
}

void Rectangle::calculateIzz()
{
	m_Izz = (1.0 / 12.0) * m_bz * std::pow(m_by, 3.0);
}

void Rectangle::calculateJ()
{
	double a = std::max(m_by, m_bz);
	double b = std::min(m_by, m_bz);

	m_J = (a * std::pow(b, 3.0) / 16) * (16 - 3.36 * b / a * (1 - std::pow(b, 4.0) / (12 * std::pow(a, 4.0))));
}

