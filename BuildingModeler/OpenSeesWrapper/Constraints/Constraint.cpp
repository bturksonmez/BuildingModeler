#include "Constraint.h"

using namespace opensees;

ConstraintType Constraint::getConstraintType() const
{
	return m_constraintType;
}

bool Constraint::operator==(const Constraint& other) const
{
    // If the derived types are the same then compare them
    return typeid(*this) == typeid(other) && isEqual(other);
}