#include "Mass.h"

using namespace std;
using namespace opensees;

Mass::Mass(shared_ptr<Node> node, vector<double> massValues) : m_node(node), m_massValues(massValues)
{
}

const shared_ptr<Node> Mass::getNode() const
{
	return m_node;
}

const vector<double>& Mass::getMassValues() const
{
	return m_massValues;
}

string Mass::getOpenseesCommand() const
{
	string command;

	command = "mass " + to_string(m_node->getNodeTag());

	for (auto val : m_massValues)
		command += (" " + to_string(val));

	command += ("\n");

	return command;
}