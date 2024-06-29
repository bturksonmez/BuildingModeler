#include "ModalAnalysis.h"

using namespace opensees;

ModalAnalysis::ModalAnalysis(std::string sourceModelName, size_t numberOfModes) : Analysis(sourceModelName)
{
	m_analysisID = counter++;
	m_analysisType = opensees::AnalysisType::MODAL;
	m_numberOfModes = numberOfModes;
}

size_t ModalAnalysis::getNumberOfModes() const
{
	return m_numberOfModes;
}

std::string ModalAnalysis::getOpenseesCommand() const
{
	std::string command;

	return command;
}