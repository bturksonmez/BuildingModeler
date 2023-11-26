#include "Node.h"

using namespace std;
using namespace opensees;

Node::Node(int nodeTag, vector<double> coords, vector<double> massValues) :
		m_nodeTag(nodeTag), m_coords(coords), m_massValues(massValues)
{
}

int Node::getNodeTag() const
{
	return m_nodeTag;
}

const vector<double>& Node::getCoords() const
{
	return m_coords;
}

const vector<double>& Node::getMassValues() const
{
	return m_massValues;
}

void Node::setMassValues(vector<double> massValues)
{
	m_massValues = massValues;
}

string Node::getOpenseesCommand() const
{
	string command;

	command = "node " + to_string(m_nodeTag);
	
	for (auto val : m_coords)
		command += (" " + to_string(val));

	if (!m_massValues.empty())
	{
		command += (" -mass");
		for (auto val : m_massValues)
			command += (" " + to_string(val));
	}

	command += ("\n");

	return command;
}