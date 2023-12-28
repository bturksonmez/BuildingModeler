#include "Node.h"

using namespace opensees;

Node::Node(int nodeTag, std::vector<double> coords, std::vector<double> massValues) :
		m_nodeTag(nodeTag), m_coords(coords), m_massValues(massValues)
{
}

int Node::getNodeTag() const
{
	return m_nodeTag;
}

const std::vector<double>& Node::getCoords() const
{
	return m_coords;
}

const std::vector<double>& Node::getMassValues() const
{
	return m_massValues;
}

void Node::setMassValues(std::vector<double> massValues)
{
	m_massValues = massValues;
}

std::string Node::getOpenseesCommand() const
{
	std::string command;

	command = "node " + std::to_string(m_nodeTag);
	
	for (auto val : m_coords)
		command += (" " + std::to_string(val));

	if (!m_massValues.empty())
	{
		command += (" -mass");
		for (auto val : m_massValues)
			command += (" " + std::to_string(val));
	}

	command += ("\n");

	return command;
}