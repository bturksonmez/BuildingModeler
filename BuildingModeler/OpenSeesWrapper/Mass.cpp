#include "Mass.h"

using namespace std;
using namespace opensees;

Mass::Mass(int nodeTag, vector<double> massValues) : m_nodeTag(nodeTag), m_massValues(massValues)
{
}

int Mass::getNodeTag() const
{
	return m_nodeTag;
}

const vector<double>& Mass::getMassValues() const
{
	return m_massValues;
}

string Mass::getOpenseesCommand() const
{
	string command;

	command = "mass " + to_string(m_nodeTag);

	for (auto val : m_massValues)
		command += (" " + to_string(val));

	command += ("\n");

	return command;
}