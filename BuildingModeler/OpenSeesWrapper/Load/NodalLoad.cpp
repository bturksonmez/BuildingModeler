#include "NodalLoad.h"

using namespace std;
using namespace opensees;

NodalLoad::NodalLoad(int nodeTag, vector<double> loadVector) : Load(loadVector)
{
	m_nodeTag = nodeTag;
	m_loadType = LoadType::NODALLOAD;
}

int NodalLoad::getNodeTag() const
{
	return m_nodeTag;
}

string NodalLoad::getOpenseesCommand() const
{
    string command;

    command = "load " + to_string(m_nodeTag) + " " + to_string(m_loadVector[0]) + " " + to_string(m_loadVector[1]) +  " " + to_string(m_loadVector[2]) 
		+ " " + to_string(m_loadVector[3]) + " " + to_string(m_loadVector[4]) + " " + to_string(m_loadVector[5]);

    command += ("\n");

    return command;
}