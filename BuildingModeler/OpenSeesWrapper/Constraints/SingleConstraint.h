#pragma once

#include "Constraint.h"

namespace opensees
{
	class SingleConstraint : public Constraint
	{
	private:
		int m_nodeTag;
		std::vector<int> m_fixedDOFs;

	public:
		SingleConstraint(int nodeTag, std::vector<int> fixedDOFs);
		SingleConstraint() = delete;
		~SingleConstraint() {}

		int getNodeTag() const;
		const std::vector<int>& getFixedDOFs() const;

		std::string getOpenseesCommand() const override;
	};
}
