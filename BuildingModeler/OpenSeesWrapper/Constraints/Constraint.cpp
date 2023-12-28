#include "Constraint.h"

using namespace opensees;

ConstraintType Constraint::getConstraintType() const
{
	return m_constraintType;
}