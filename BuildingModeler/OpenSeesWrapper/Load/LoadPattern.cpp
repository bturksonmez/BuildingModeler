#include "LoadPattern.h"

using namespace opensees;

int LoadPattern::counter = 0;

int LoadPattern::getPatternTag() const
{
	return m_patternTag;
}

std::string LoadPattern::getLoadingName() const
{
	return m_loadingName;
}

TimeSeriesType LoadPattern::getTimeSeriesType() const
{
	return m_timeSeriesType;
}

const std::vector<std::shared_ptr<Load>>& LoadPattern::getLoadVector() const
{
	return m_loadVector;
}

void LoadPattern::addLoad(std::shared_ptr<Load> load)
{
	m_loadVector.push_back(load);
}

std::string LoadPattern::getOpenseesCommand() const
{
	if (m_loadVector.empty())
		return "# No load defined for pattern: " + std::to_string(m_patternTag);

	std::string command;

	std::string tsType;
	switch (m_timeSeriesType) {
	case TimeSeriesType::CONSTANT:
		tsType = "Constant";
		break;
	case TimeSeriesType::LINEAR:
		tsType = "Linear";
		break;
	default:
		tsType = "Linear";
	}

	command = "pattern Plain " + std::to_string(m_patternTag) + " " + tsType + " {\n";

	for (auto& load : m_loadVector) {
		command += ("\t" + load->getOpenseesCommand());
	}

	command += ("}\n");

	return command;
}