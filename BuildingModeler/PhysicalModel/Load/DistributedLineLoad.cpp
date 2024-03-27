#include "DistributedLineLoad.h"

using namespace physicalModel;

DistributedLineLoad::DistributedLineLoad(int beamElementTag, double wz, double wy, double wx) : Load()
{
	m_uniqueID = counter++;
	m_beamElementTag = beamElementTag;
	m_loadType = LoadType::DISTRIBUTED_LINE_LOAD;

	m_loadVector.resize(3);
	m_loadVector[0] = wz;
	m_loadVector[1] = wy;
	m_loadVector[2] = wx;
}

int DistributedLineLoad::getBeamElementTag() const
{
	return m_beamElementTag;
}