#include "TransientOutput.h"

using namespace opensees;

TransientOutput::TransientOutput(std::string loadTag) : Output(loadTag)
{
	m_outputType = OutputType::TRANSIENT;
}

const std::unordered_map<int, std::vector<std::vector<double>>>& TransientOutput::getElementForce() const
{
	return m_eleForceOutput;
}