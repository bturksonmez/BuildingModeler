#include "DiaphragmConstraint.h"

using namespace opensees;

DiaphragmConstraint::DiaphragmConstraint(int masterNodeTag, std::vector<int> slaveNodeTags, int planeDirection)
	: m_masterNodeTag(masterNodeTag), m_slaveNodeTags(slaveNodeTags), m_planeDirection(planeDirection)
{
	m_constraintType = ConstraintType::DIAPHRAGM;
}

bool DiaphragmConstraint::isEqual(const Constraint& other) const
{
	return m_planeDirection == static_cast<const DiaphragmConstraint&>(other).getPlaneDirection();
}

int DiaphragmConstraint::getMasterNodeTag() const
{
	return m_masterNodeTag;
}

const std::vector<int>& DiaphragmConstraint::getSlaveNodeTags() const
{
	return m_slaveNodeTags;
}

int DiaphragmConstraint::getPlaneDirection() const
{
	return m_planeDirection;
}

std::string DiaphragmConstraint::getOpenseesCommand() const
{
	std::string commandLine;

	commandLine = "rigidDiaphragm " + std::to_string(m_planeDirection) + " " + std::to_string(m_masterNodeTag);
	for (int i = 0; i < m_slaveNodeTags.size(); i++)
		commandLine = commandLine + " " + std::to_string(m_slaveNodeTags[i]);

	commandLine = commandLine + "\n";

	return commandLine;
}