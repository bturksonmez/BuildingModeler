#include "SingleConstraint.h"

using namespace std;
using namespace opensees;

SingleConstraint::SingleConstraint(int nodeTag, int fixDOF1, int fixDOF2, int fixDOF3, int fixDOF4, int fixDOF5, int fixDOF6)
{
	m_constraintType = ConstraintType::Single;

	m_nodeTag = nodeTag;
	m_fixedDOF.push_back(fixDOF1);
	m_fixedDOF.push_back(fixDOF2);
	m_fixedDOF.push_back(fixDOF3);
	m_fixedDOF.push_back(fixDOF4);
	m_fixedDOF.push_back(fixDOF5);
	m_fixedDOF.push_back(fixDOF6);
}

int SingleConstraint::getNodeTag() const
{
	return m_nodeTag;
}

const vector<int>& SingleConstraint::getConstraintVector() const
{
	return m_fixedDOF;
}

string SingleConstraint::getOpenseesCommand() const
{
	string command;

	command = "fix " + to_string(m_nodeTag) + " " + to_string(m_fixedDOF[0]) + " " + to_string(m_fixedDOF[1]) + " " + to_string(m_fixedDOF[2])
		+ " " + to_string(m_fixedDOF[3]) + " " + to_string(m_fixedDOF[4]) + " " + to_string(m_fixedDOF[5]) + "\n";

	return command;
}