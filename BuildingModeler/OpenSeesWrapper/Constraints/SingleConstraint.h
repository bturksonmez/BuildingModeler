#pragma once

#include "Constraint.h"

namespace opensees
{
	class SingleConstraint : public Constraint
	{
	private:
		std::shared_ptr<Node> m_node;
		std::vector<int> m_fixedDOFs;

	public:
		SingleConstraint(std::shared_ptr<Node> nodeTag, std::vector<int> fixedDOFs);
		SingleConstraint() = delete;
		~SingleConstraint() {}

		const std::shared_ptr<Node> getNode() const;
		const std::vector<int>& getFixedDOFs() const;

		std::string getOpenseesCommand() const override;
	};
}
