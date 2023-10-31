#pragma once

#include "Constraint.h"

namespace opensees
{
	class SingleConstraint : public Constraint
	{
	private:
		int m_nodeTag;
		std::vector<int> m_fixedDOF;

	public:
		SingleConstraint(int nodeTag, int fixDOF1, int fixDOF2, int fixDOF3, int fixDOF4, int fixDOF5, int fixDOF6);
		SingleConstraint() = delete;
		~SingleConstraint() {}

		int getNodeTag() const;
		const std::vector<int>& getConstraintVector() const;

		std::string getOpenseesCommand() const override;
	};
}
