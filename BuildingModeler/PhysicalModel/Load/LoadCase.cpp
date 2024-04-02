#include "LoadCase.h"

using namespace physicalModel;

LoadCase::LoadCase(std::string loadCaseTag, LoadCaseType loadCaseType) : m_loadCaseTag(loadCaseTag), m_loadCaseType(loadCaseType)
{
}

void LoadCase::setActive(bool active)
{
	m_active = active;
}

void LoadCase::addPointLoad(std::shared_ptr<Load> load)
{
	m_pointLoads.push_back(load);
}

void LoadCase::addDistributedLineLoad(std::shared_ptr<Load> load)
{
	m_distributedLineLoads.push_back(load);
}

void LoadCase::addDistributedAreaLoad(std::shared_ptr<Load> load)
{
	m_distributedAreaLoads.push_back(load);
}

bool LoadCase::isActive() const
{
	return m_active;
}

std::string LoadCase::getLoadCaseTag() const
{
	return m_loadCaseTag;
}

LoadCaseType LoadCase::getLoadCaseType() const
{
	return m_loadCaseType;
}

const std::vector<std::shared_ptr<Load>>& LoadCase::getPointLoads() const
{
	return m_pointLoads;
}

const std::vector<std::shared_ptr<Load>>& LoadCase::getDistributedLineLoads() const
{
	return m_distributedLineLoads;
}

const std::vector<std::shared_ptr<Load>>& LoadCase::getDistributedAreaLoads() const
{
	return m_distributedAreaLoads;
}