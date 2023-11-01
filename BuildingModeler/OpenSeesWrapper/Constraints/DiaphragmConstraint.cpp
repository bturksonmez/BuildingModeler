#include "DiaphragmConstraint.h"

using namespace std;
using namespace opensees;

DiaphragmConstraint::DiaphragmConstraint(shared_ptr<Node> masterNode, vector<shared_ptr<Node>> slaveNodes, int planeDirection)
	: m_masterNode(masterNode), m_slaveNodes(slaveNodes), m_planeDirection(planeDirection)
{
	m_constraintType = ConstraintType::DIAPHRAGM;
}

const shared_ptr<Node> DiaphragmConstraint::getMasterNode() const
{
	return m_masterNode;
}

const vector<shared_ptr<Node>>& DiaphragmConstraint::getSlaveNodes() const
{
	return m_slaveNodes;
}

int DiaphragmConstraint::getPlaneDirection() const
{
	return m_planeDirection;
}

string DiaphragmConstraint::getOpenseesCommand() const
{
	string commandLine;

	commandLine = "rigidDiaphragm " + to_string(m_planeDirection) + " " + to_string(m_masterNode->getNodeTag());
	for (int i = 0; i < m_slaveNodes.size(); i++)
		commandLine = commandLine + " " + to_string(m_slaveNodes[i]->getNodeTag());

	commandLine = commandLine + "\n";

	return commandLine;
}