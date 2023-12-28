#include "Mass.h"

using namespace opensees;

Mass::Mass(int nodeTag, std::vector<double> massValues) : m_nodeTag(nodeTag), m_massValues(massValues)
{
}

int Mass::getNodeTag() const
{
	return m_nodeTag;
}

const std::vector<double>& Mass::getMassValues() const
{
	return m_massValues;
}

std::string Mass::getOpenseesCommand() const
{
	std::string command;

	command = "mass " + std::to_string(m_nodeTag);

	for (auto val : m_massValues)
		command += (" " + std::to_string(val));

	command += ("\n");

	return command;
}