#include "EqualDOFConstraint.h"

using namespace std;
using namespace opensees;

EqualDOFConstraint::EqualDOFConstraint(shared_ptr<Node> masterNode, shared_ptr<Node> slaveNode, std::vector<int> constrainedDOFs)
	: m_masterNode(masterNode), m_slaveNode(slaveNode), m_constrainedDOFs(constrainedDOFs)
{
	m_constraintType = ConstraintType::EQUALDOF;
}

const shared_ptr<Node> EqualDOFConstraint::getMasterNode() const
{
	return m_masterNode;
}

const shared_ptr<Node> EqualDOFConstraint::getSlaveNode() const
{
	return m_slaveNode;
}

const vector<int>& EqualDOFConstraint::getConstrainedDOFs() const
{
	return m_constrainedDOFs;
}

string EqualDOFConstraint::getOpenseesCommand() const
{
	string commandLine;

	commandLine = "equalDOF " + to_string(m_masterNode->getNodeTag()) + " " + to_string(m_slaveNode->getNodeTag());
	for (int i = 0; i < m_constrainedDOFs.size(); i++)
		commandLine = commandLine + " " + to_string(m_constrainedDOFs[i]);

	commandLine = commandLine + "\n";

	return commandLine;
}