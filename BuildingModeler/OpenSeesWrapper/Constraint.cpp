#include "Constraint.h"

using namespace std;
using namespace opensees;

ConstraintType Constraint::getConstraintType() const
{
	return m_constraintType;
}