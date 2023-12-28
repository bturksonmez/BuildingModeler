#include "NodalLoad.h"

using namespace opensees;

NodalLoad::NodalLoad(int nodeTag, std::vector<double> loadVector) : Load(loadVector)
{
	m_nodeTag = nodeTag;
	m_loadType = LoadType::NODAL_LOAD;
}

int NodalLoad::getNodeTag() const
{
	return m_nodeTag;
}

std::string NodalLoad::getOpenseesCommand() const
{
	std::string command;

    command = "load " + std::to_string(m_nodeTag) + " " + std::to_string(m_loadVector[0]) + " " + std::to_string(m_loadVector[1]) +  " " + std::to_string(m_loadVector[2]) 
		+ " " + std::to_string(m_loadVector[3]) + " " + std::to_string(m_loadVector[4]) + " " + std::to_string(m_loadVector[5]);

    command += ("\n");

    return command;
}