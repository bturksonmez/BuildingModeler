#include "Load.h"

using namespace physicalModel;

LoadType Load::getLoadType() const
{
	return m_loadType;
}

const std::vector<double>& Load::getLoadVector() const
{
	return m_loadVector;
}