#include "ModalOutput.h"

using namespace opensees;

ModalOutput::ModalOutput(std::string loadTag, std::vector<int> masterNodeTags) : Output(loadTag)
{
	m_outputType = OutputType::MODAL;
	m_masterNodeTags = masterNodeTags;
}

const std::unordered_map<int, std::vector<double>>& ModalOutput::getPeriods() const
{
	return m_periods;
}

void ModalOutput::readOutput()
{

}