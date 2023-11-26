#pragma once

#include "../OpenseesObject.h"

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

	class Constraint : public OpenseesObject
	{
	protected:
		ConstraintType m_constraintType;

		Constraint() {}

	public:
		~Constraint() {}

		ConstraintType getConstraintType() const;
	};
}

