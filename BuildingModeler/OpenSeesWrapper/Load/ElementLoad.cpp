#include "ElementLoad.h"

using namespace opensees;

ElementLoad::ElementLoad(int elementTag, std::vector<double> loadVector) : Load(loadVector)
{
	m_elementTag = elementTag;
	m_loadType = LoadType::ELEMENT_LOAD;
}

int ElementLoad::getElementTag() const
{
	return m_elementTag;
}

std::string ElementLoad::getOpenseesCommand() const
{
	std::string command;

	command = "eleLoad -ele " + std::to_string(m_elementTag) + " -type -beamUniform " + std::to_string(m_loadVector[0])
		+ " " + std::to_string(m_loadVector[1]) + ((m_loadVector.size() == 2) ? "" : (" " + std::to_string(m_loadVector[2])));

	command += ("\n");

	return command;
}