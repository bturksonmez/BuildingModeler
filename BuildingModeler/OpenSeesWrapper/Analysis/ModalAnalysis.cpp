#include "ModalAnalysis.h"

using namespace opensees;

ModalAnalysis::ModalAnalysis(std::string sourceModelName, std::string analysisName, size_t numberOfModes) : Analysis(sourceModelName, analysisName)
{
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