#include "StaticLoadCombination.h"

using namespace physicalModel;

StaticLoadCombination::StaticLoadCombination(std::string loadCombinationTag) : m_loadCombinationTag(loadCombinationTag)
{
}

void StaticLoadCombination::setActive(bool active)
{
	m_active = active;
}

void StaticLoadCombination::addLoadCase(std::shared_ptr<LoadCase> load, double factor)
{
	m_loadCases.push_back(std::make_pair(load, factor));
}

bool StaticLoadCombination::isActive() const
{
	return m_active;
}

std::string StaticLoadCombination::getLoadCombinationTag() const
{
	return m_loadCombinationTag;
}

const std::vector<std::pair<std::shared_ptr<LoadCase>, double>>& StaticLoadCombination::getLoadCases() const
{
	return m_loadCases;
}