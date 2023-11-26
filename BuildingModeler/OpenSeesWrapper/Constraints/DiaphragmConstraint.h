#pragma once

#include "Constraint.h"

namespace opensees
{
	class DiaphragmConstraint : public Constraint
	{
	private:
		int m_masterNodeTag;
		std::vector<int> m_slaveNodeTags;
		int m_planeDirection;

	public:
		DiaphragmConstraint(int masterNodeTag, std::vector<int> slaveNodeTags, int planeDirection);
		DiaphragmConstraint() = delete;
		~DiaphragmConstraint() {}

		int getMasterNodeTag() const;
		const std::vector<int>& getSlaveNodeTags() const;
		int getPlaneDirection() const;

		std::string getOpenseesCommand() const override;
	};
}
