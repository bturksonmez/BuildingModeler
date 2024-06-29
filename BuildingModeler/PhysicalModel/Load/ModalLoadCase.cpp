#include "ModalLoadCase.h"

using namespace physicalModel;

ModalLoadCase::ModalLoadCase(std::string loadCaseTag, size_t numberOfModes) : LoadCase(loadCaseTag)
{
	m_loadCaseType = LoadCaseType::MODAL;
	m_numberOfModes = numberOfModes;
}

size_t ModalLoadCase::getNumberOfModes() const
{
	return m_numberOfModes;
}