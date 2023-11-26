#include "EqualDOFConstraint.h"

using namespace std;
using namespace opensees;

EqualDOFConstraint::EqualDOFConstraint(int masterNodeTag, int slaveNodeTag, std::vector<int> constrainedDOFs)
	: m_masterNodeTag(masterNodeTag), m_slaveNodeTag(slaveNodeTag), m_constrainedDOFs(constrainedDOFs)
{
	m_constraintType = ConstraintType::EQUALDOF;
}

int EqualDOFConstraint::getMasterNodeTag() const
{
	return m_masterNodeTag;
}

int EqualDOFConstraint::getSlaveNodeTag() const
{
	return m_slaveNodeTag;
}

const vector<int>& EqualDOFConstraint::getConstrainedDOFs() const
{
	return m_constrainedDOFs;
}

string EqualDOFConstraint::getOpenseesCommand() const
{
	string commandLine;

	commandLine = "equalDOF " + to_string(m_masterNodeTag) + " " + to_string(m_slaveNodeTag);
	for (int i = 0; i < m_constrainedDOFs.size(); i++)
		commandLine = commandLine + " " + to_string(m_constrainedDOFs[i]);

	commandLine = commandLine + "\n";

	return commandLine;
}