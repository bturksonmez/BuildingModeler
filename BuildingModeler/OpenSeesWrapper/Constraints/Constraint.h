#pragma once

#include "../Object.h"
#include "../Node.h"

namespace opensees
{
	enum class ConstraintType
	{
		SINGLE,
		EQUALDOF,
		DIAPHRAGM,
		RIGIDBAR,
		RIGIDBEAM
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

