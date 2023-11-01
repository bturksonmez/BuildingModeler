#pragma once

#include "Constraint.h"

namespace opensees
{
	class DiaphragmConstraint : public Constraint
	{
	private:
		std::shared_ptr<Node> m_masterNode;
		std::vector<std::shared_ptr<Node>> m_slaveNodes;
		int m_planeDirection;

	public:
		DiaphragmConstraint(std::shared_ptr<Node> masterNode, std::vector<std::shared_ptr<Node>> slaveNodes, int planeDirection);
		DiaphragmConstraint() = delete;
		~DiaphragmConstraint() {}

		const std::shared_ptr<Node> getMasterNode() const;
		const std::vector<std::shared_ptr<Node>>& getSlaveNodes() const;
		int getPlaneDirection() const;

		std::string getOpenseesCommand() const override;
	};
}
