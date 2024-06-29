#include "StaticLoadCase.h"

using namespace physicalModel;

StaticLoadCase::StaticLoadCase(std::string loadCaseTag, StaticLoadCaseType staticLoadCaseType) : LoadCase(loadCaseTag)
{
	m_loadCaseType = LoadCaseType::STATIC;
	m_staticLoadCaseType = staticLoadCaseType;
}

void StaticLoadCase::addPointLoad(std::shared_ptr<Load> load)
{
	m_pointLoads.push_back(load);
}

void StaticLoadCase::addDistributedLineLoad(std::shared_ptr<Load> load)
{
	m_distributedLineLoads.push_back(load);
}

void StaticLoadCase::addDistributedAreaLoad(std::shared_ptr<Load> load)
{
	m_distributedAreaLoads.push_back(load);
}

StaticLoadCaseType StaticLoadCase::getStaticLoadCaseType() const
{
	return m_staticLoadCaseType;
}

const std::vector<std::shared_ptr<Load>>& StaticLoadCase::getPointLoads() const
{
	return m_pointLoads;
}

const std::vector<std::shared_ptr<Load>>& StaticLoadCase::getDistributedLineLoads() const
{
	return m_distributedLineLoads;
}

const std::vector<std::shared_ptr<Load>>& StaticLoadCase::getDistributedAreaLoads() const
{
	return m_distributedAreaLoads;
}