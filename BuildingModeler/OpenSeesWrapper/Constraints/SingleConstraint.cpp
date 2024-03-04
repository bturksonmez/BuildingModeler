#include "SingleConstraint.h"

using namespace opensees;

SingleConstraint::SingleConstraint(int nodeTag, std::vector<int> fixedDOFs) : m_nodeTag(nodeTag), m_fixedDOFs(fixedDOFs)
{
	m_constraintType = ConstraintType::SINGLE;
}

bool SingleConstraint::isEqual(const Constraint& other) const
{
	return m_fixedDOFs == static_cast<const SingleConstraint&>(other).getFixedDOFs();
}


int SingleConstraint::getNodeTag() const
{
	return m_nodeTag;
}

const std::vector<int>& SingleConstraint::getFixedDOFs() const
{
	return m_fixedDOFs;
}

std::string SingleConstraint::getOpenseesCommand() const
{
	std::string command;

	command = "fix " + std::to_string(m_nodeTag) + " " + std::to_string(m_fixedDOFs[0]) + " " + std::to_string(m_fixedDOFs[1]) + " " + std::to_string(m_fixedDOFs[2])
		+ " " + std::to_string(m_fixedDOFs[3]) + " " + std::to_string(m_fixedDOFs[4]) + " " + std::to_string(m_fixedDOFs[5]) + "\n";

	return command;
}