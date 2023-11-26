#pragma once

#include "Constraint.h"

namespace opensees
{
	class EqualDOFConstraint : public Constraint
	{
	private:
		int m_masterNodeTag;
		int m_slaveNodeTag;
		std::vector<int> m_constrainedDOFs;

	public:
		EqualDOFConstraint(int masterNodeTag, int slaveNodeTag, std::vector<int> constrainedDOFs);
		EqualDOFConstraint() = delete;
		~EqualDOFConstraint() {}

		int getMasterNodeTag() const;
		int getSlaveNodeTag() const;
		const std::vector<int>& getConstrainedDOFs() const;

		std::string getOpenseesCommand() const override;
	};
}

