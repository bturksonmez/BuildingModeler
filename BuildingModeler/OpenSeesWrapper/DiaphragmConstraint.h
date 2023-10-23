#pragma once

#include "Constraint.h"

namespace opensees
{
	class DiaphragmConstraint : public Constraint
	{
	private:
		int m_masterNodeTag;
		std::vector<int> m_slaveNodeTag;
		int m_planeDirection;

	public:
		DiaphragmConstraint(int masterNodeTag, std::vector<int> slaveNodeTag, int planeDirection);
		~DiaphragmConstraint() {}

		int getMasterNodeTag() const;
		const std::vector<int>& getSlaveNodeTag() const;
		int getPlaneDirection() const;

		std::string getOpenseesCommand() const override;
	};
}
