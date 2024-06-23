#include "StaticOutput.h"

using namespace opensees;

StaticOutput::StaticOutput(std::string loadTag) : Output(loadTag)
{
	m_outputType = OutputType::STATIC;
}

const std::unordered_map<int, std::vector<std::vector<double>>>& StaticOutput::getElementForce() const
{
	return m_eleForceOutput;
}