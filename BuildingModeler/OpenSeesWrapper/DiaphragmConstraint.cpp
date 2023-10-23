#include "DiaphragmConstraint.h"

using namespace std;
using namespace opensees;

DiaphragmConstraint::DiaphragmConstraint(int masterNodeTag, vector<int> slaveNodeTag, int planeDirection)
{
	m_constraintType = ConstraintType::Diaphragm;

	m_masterNodeTag = masterNodeTag;
	m_slaveNodeTag = slaveNodeTag;
	m_planeDirection = planeDirection;
}

int DiaphragmConstraint::getMasterNodeTag() const
{
	return m_masterNodeTag;
}

const vector<int>& DiaphragmConstraint::getSlaveNodeTag() const
{
	return m_slaveNodeTag;
}

int DiaphragmConstraint::getPlaneDirection() const
{
	return m_planeDirection;
}

string DiaphragmConstraint::getOpenseesCommand() const
{
	string commandLine;

	commandLine = "rigidDiaphragm " + to_string(m_planeDirection) + " " + to_string(m_masterNodeTag);
	for (int i = 0; i < m_slaveNodeTag.size(); i++)
		commandLine = commandLine + " " + to_string(m_slaveNodeTag[i]);

	commandLine = commandLine + "\n";

	return commandLine;
}