#include "SingleConstraint.h"

using namespace std;
using namespace opensees;

SingleConstraint::SingleConstraint(int nodeTag, vector<int> fixedDOFs) : m_nodeTag(nodeTag), m_fixedDOFs(fixedDOFs)
{
	m_constraintType = ConstraintType::SINGLE;
}

int SingleConstraint::getNodeTag() const
{
	return m_nodeTag;
}

const vector<int>& SingleConstraint::getFixedDOFs() const
{
	return m_fixedDOFs;
}

string SingleConstraint::getOpenseesCommand() const
{
	string command;

	command = "fix " + to_string(m_nodeTag) + " " + to_string(m_fixedDOFs[0]) + " " + to_string(m_fixedDOFs[1]) + " " + to_string(m_fixedDOFs[2])
		+ " " + to_string(m_fixedDOFs[3]) + " " + to_string(m_fixedDOFs[4]) + " " + to_string(m_fixedDOFs[5]) + "\n";

	return command;
}