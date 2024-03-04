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

		virtual bool isEqual(const Constraint& other) const = 0;

	public:
		~Constraint() {}

		ConstraintType getConstraintType() const;
		bool operator==(const Constraint& other) const;
	};
}

