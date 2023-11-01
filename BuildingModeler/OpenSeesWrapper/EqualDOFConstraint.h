#pragma once

#include "Constraint.h"

namespace opensees
{
	class EqualDOFConstraint : public Constraint
	{
	private:
		std::shared_ptr<Node> m_masterNode;
		std::shared_ptr<Node> m_slaveNode;
		std::vector<int> m_constrainedDOFs;

	public:
		EqualDOFConstraint(std::shared_ptr<Node> masterNode, std::shared_ptr<Node> slaveNode, std::vector<int> constrainedDOFs);
		EqualDOFConstraint() = delete;
		~EqualDOFConstraint() {}

		const std::shared_ptr<Node> getMasterNode() const;
		const std::shared_ptr<Node> getSlaveNode() const;
		const std::vector<int>& getConstrainedDOFs() const;

		std::string getOpenseesCommand() const override;
	};
}

