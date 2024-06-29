#include "StaticAnalysis.h"

using namespace opensees;

StaticAnalysis::StaticAnalysis(std::string sourceModelName, std::shared_ptr<LoadPattern> loadPattern) : Analysis(sourceModelName)
{
	m_analysisID = counter++;
	m_analysisType = opensees::AnalysisType::STATIC;
	m_loadPattern = loadPattern;
}

std::shared_ptr<LoadPattern> StaticAnalysis::getLoadPattern() const
{
	return m_loadPattern;
}

std::string StaticAnalysis::getOpenseesCommand() const
{
	std::string command;

	return command;
}