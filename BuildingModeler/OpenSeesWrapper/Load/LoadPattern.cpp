#include "LoadPattern.h"

using namespace std;
using namespace opensees;

int LoadPattern::getPatternTag() const
{
	return m_patternTag;
}

TimeSeriesType LoadPattern::getTimeSeriesType() const
{
	return m_timeSeriesType;
}

const vector<shared_ptr<Load>>& LoadPattern::getLoadVector() const
{
	return m_loadVector;
}

void LoadPattern::addLoad(shared_ptr<Load> load)
{
	m_loadVector.push_back(load);
}

string LoadPattern::getOpenseesCommand() const
{
	if (m_loadVector.empty())
		return "# No load defined for pattern: " + to_string(m_patternTag);

	string command;

	string tsType;
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

	command = "pattern Plain " + to_string(m_patternTag) + " " + tsType + " {\n";

	for (auto& load : m_loadVector) {
		command += ("\t" + load->getOpenseesCommand());
	}

	command += ("}\n");

	return command;
}