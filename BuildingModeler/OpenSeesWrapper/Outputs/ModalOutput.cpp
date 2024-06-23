#include "ModalOutput.h"

using namespace opensees;

ModalOutput::ModalOutput(std::string loadTag) : Output(loadTag)
{
	m_outputType = OutputType::MODAL;
}

const std::unordered_map<int, std::vector<double>>& ModalOutput::getPeriods() const
{
	return m_periods;
}