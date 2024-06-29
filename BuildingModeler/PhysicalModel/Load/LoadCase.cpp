#include "LoadCase.h"

using namespace physicalModel;

LoadCase::LoadCase(std::string loadCaseTag) : m_loadCaseTag(loadCaseTag)
{
}

void LoadCase::setActive(bool active)
{
	m_active = active;
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