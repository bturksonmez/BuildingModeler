#include "EqualDOFConstraint.h"

using namespace opensees;

EqualDOFConstraint::EqualDOFConstraint(int masterNodeTag, int slaveNodeTag, std::vector<int> constrainedDOFs)
	: m_masterNodeTag(masterNodeTag), m_slaveNodeTag(slaveNodeTag), m_constrainedDOFs(constrainedDOFs)
{
	m_constraintType = ConstraintType::EQUALDOF;
}

bool EqualDOFConstraint::isEqual(const Constraint& other) const
{
	return m_constrainedDOFs == static_cast<const EqualDOFConstraint&>(other).getConstrainedDOFs();
}

int EqualDOFConstraint::getMasterNodeTag() const
{
	return m_masterNodeTag;
}

int EqualDOFConstraint::getSlaveNodeTag() const
{
	return m_slaveNodeTag;
}

const std::vector<int>& EqualDOFConstraint::getConstrainedDOFs() const
{
	return m_constrainedDOFs;
}

std::string EqualDOFConstraint::getOpenseesCommand() const
{
	std::string commandLine;

	commandLine = "equalDOF " + std::to_string(m_masterNodeTag) + " " + std::to_string(m_slaveNodeTag);
	for (int i = 0; i < m_constrainedDOFs.size(); i++)
		commandLine = commandLine + " " + std::to_string(m_constrainedDOFs[i]);

	commandLine = commandLine + "\n";

	return commandLine;
}