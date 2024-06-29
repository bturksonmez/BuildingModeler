#include "Analysis.h"

using namespace opensees;

size_t Analysis::counter = 0;

Analysis::Analysis(std::string sourceModelName) : m_sourceModelName(sourceModelName)
{

}

size_t Analysis::getAnalysisID() const
{
	return m_analysisID;
}

std::string Analysis::getSourceModelName() const
{
	return m_sourceModelName;
}

AnalysisType Analysis::getAnalysisType() const
{
	return m_analysisType;
}