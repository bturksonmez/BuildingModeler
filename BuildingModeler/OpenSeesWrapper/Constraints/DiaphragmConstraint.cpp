#include "DiaphragmConstraint.h"

using namespace std;
using namespace opensees;

DiaphragmConstraint::DiaphragmConstraint(int masterNodeTag, vector<int> slaveNodeTags, int planeDirection)
	: m_masterNodeTag(masterNodeTag), m_slaveNodeTags(slaveNodeTags), m_planeDirection(planeDirection)
{
	m_constraintType = ConstraintType::DIAPHRAGM;
}

int DiaphragmConstraint::getMasterNodeTag() const
{
	return m_masterNodeTag;
}

const vector<int>& DiaphragmConstraint::getSlaveNodeTags() const
{
	return m_slaveNodeTags;
}

int DiaphragmConstraint::getPlaneDirection() const
{
	return m_planeDirection;
}

string DiaphragmConstraint::getOpenseesCommand() const
{
	string commandLine;

	commandLine = "rigidDiaphragm " + to_string(m_planeDirection) + " " + to_string(m_masterNodeTag);
	for (int i = 0; i < m_slaveNodeTags.size(); i++)
		commandLine = commandLine + " " + to_string(m_slaveNodeTags[i]);

	commandLine = commandLine + "\n";

	return commandLine;
}