#include "Output.h"

using namespace opensees;

int Output::counter = 0;

Output::Output(std::string loadTag) : m_loadTag(loadTag)
{
	m_outputTag = counter++;
}

int Output::getOutputTag() const
{
	return m_outputTag;
}

std::string Output::getLoadTag() const
{
	return m_loadTag;
}

OutputType Output::getOutputType() const
{
	return m_outputType;
}

const std::unordered_map<int, std::vector<std::vector<double>>>& Output::getNodeDisplacement() const
{
	return m_nodeDispOutput;
}