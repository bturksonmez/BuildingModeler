#include "ElementLoad.h"

using namespace std;
using namespace opensees;

ElementLoad::ElementLoad(int elementTag, vector<double> loadVector) : Load(loadVector)
{
	m_elementTag = elementTag;
	m_loadType = LoadType::NODALLOAD;
}

int ElementLoad::getElementTag() const
{
	return m_elementTag;
}

string ElementLoad::getOpenseesCommand() const
{
	string command;

	command = "eleLoad -ele " + to_string(m_elementTag) + " -type -beamUniform " + to_string(m_loadVector[0])
		+ " " + to_string(m_loadVector[1]) + ((m_loadVector.size() == 2) ? "" : (" " + to_string(m_loadVector[2])));

	command += ("\n");

	return command;
}