#include "LoadCombination.h"

using namespace physicalModel;

LoadCombination::LoadCombination(std::string loadCombinationTag) : m_loadCombinationTag(loadCombinationTag)
{
}

void LoadCombination::setActive(bool active)
{
	m_active = active;
}

void LoadCombination::addLoadCase(std::shared_ptr<LoadCase> load, double factor)
{
	m_loadCases.push_back(std::make_pair(load, factor));
}

bool LoadCombination::isActive() const
{
	return m_active;
}

std::string LoadCombination::getLoadCombinationTag() const
{
	return m_loadCombinationTag;
}

const std::vector<std::pair<std::shared_ptr<LoadCase>, double>>& LoadCombination::getLoadCases() const
{
	return m_loadCases;
}