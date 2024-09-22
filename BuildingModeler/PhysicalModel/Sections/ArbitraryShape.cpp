#include "ArbitraryShape.h"

using namespace physicalModel;

ArbitraryShape::ArbitraryShape(double A, double Iyy, double Izz, double J) : Shape()
{
	m_shapeType = ShapeType::ARBITRARY;
	m_A = A;
	m_Iyy = Iyy;
	m_Izz = Izz;
	m_J = J;
}

