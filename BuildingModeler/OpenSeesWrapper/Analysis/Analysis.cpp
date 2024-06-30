#include "Analysis.h"

using namespace opensees;

size_t Analysis::counter = 0;

Analysis::Analysis(std::string sourceModelName, std::string analysisName, std::shared_ptr<Output> output) : m_sourceModelName(sourceModelName), m_analysisName(analysisName), m_output(output)
{
	m_analysisID = counter++;
}

size_t Analysis::getAnalysisID() const
{
	return m_analysisID;
}

std::string Analysis::getSourceModelName() const
{
	return m_sourceModelName;
}

std::string Analysis::getAnalysisName() const
{
	return m_analysisName;
}

AnalysisType Analysis::getAnalysisType() const
{
	return m_analysisType;
}

std::shared_ptr<Output> Analysis::getOutput() const
{
	return m_output;
}