#include "Load.h"

using namespace physicalModel;

int Load::counter = 0;

int Load::getUniqueID() const
{
	return m_uniqueID;
}

LoadType Load::getLoadType() const
{
	return m_loadType;
}

const std::vector<double>& Load::getLoadVector() const
{
	return m_loadVector;
}