#include "Shape.h"

using namespace physicalModel;

ShapeType Shape::getShapeType() const
{
	return m_shapeType;
}


double Shape::getA() const
{
	return m_A;
}

double Shape::getIyy() const
{
	return m_Iyy;
}

double Shape::getIzz() const
{
	return m_Izz;
}

double Shape::getJ() const
{
	return m_J;
}