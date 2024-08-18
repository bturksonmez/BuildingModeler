#include "DistributedAreaLoad.h"

using namespace physicalModel;

DistributedAreaLoad::DistributedAreaLoad(int areaElementTag, double wy, double wz, double wx) : Load()
{
	m_uniqueID = counter++;
	m_areaElementTag = areaElementTag;
	m_loadType = LoadType::DISTRIBUTED_AREA_LOAD;

	m_loadVector.resize(3);
	m_loadVector[0] = wy;
	m_loadVector[1] = wz;
	m_loadVector[2] = wx;
}

int DistributedAreaLoad::getAreaElementTag() const
{
	return m_areaElementTag;
}