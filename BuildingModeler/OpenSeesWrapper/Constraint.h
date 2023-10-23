#pragma once

#include "Object.h"

namespace opensees
{
	enum class ConstraintType
	{
		Single,
		EqualDOF,
		Diaphragm,
		RigidBar,
		RigidBeam
	};

	class Constraint : public Object
	{
	protected:
		ConstraintType m_constraintType;

		Constraint() {}

	public:
		~Constraint() {}

		ConstraintType getConstraintType() const;
	};
}

