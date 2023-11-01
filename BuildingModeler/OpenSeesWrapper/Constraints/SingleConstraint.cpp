#include "SingleConstraint.h"

using namespace std;
using namespace opensees;

SingleConstraint::SingleConstraint(shared_ptr<Node> node, vector<int> fixedDOFs) : m_node(node), m_fixedDOFs(fixedDOFs)
{
	m_constraintType = ConstraintType::SINGLE;
}

const shared_ptr<Node> SingleConstraint::getNode() const
{
	return m_node;
}

const vector<int>& SingleConstraint::getFixedDOFs() const
{
	return m_fixedDOFs;
}

string SingleConstraint::getOpenseesCommand() const
{
	string command;

	command = "fix " + to_string(m_node->getNodeTag()) + " " + to_string(m_fixedDOFs[0]) + " " + to_string(m_fixedDOFs[1]) + " " + to_string(m_fixedDOFs[2])
		+ " " + to_string(m_fixedDOFs[3]) + " " + to_string(m_fixedDOFs[4]) + " " + to_string(m_fixedDOFs[5]) + "\n";

	return command;
}