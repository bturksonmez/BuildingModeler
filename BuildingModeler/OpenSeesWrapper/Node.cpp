#include "Node.h"

using namespace opensees;
using namespace utility;

Node::Node(int nodeTag, Vector3 coords) : m_nodeTag(nodeTag), m_coords(coords)
{
}

int Node::getNodeTag() const
{
	return m_nodeTag;
}

const Vector3& Node::getCoords() const
{
	return m_coords;
}

std::string Node::getOpenseesCommand() const
{
	std::string command;

	command = "node " + std::to_string(m_nodeTag);
	
	command += (" " + std::to_string(m_coords.x));
	command += (" " + std::to_string(m_coords.y));
	command += (" " + std::to_string(m_coords.z));

	command += ("\n");

	return command;
}