#include "PointLoad.h"

using namespace physicalModel;

PointLoad::PointLoad(int jointTag, double fx, double fy, double fz, double mx, double my, double mz) : Load()
{
	m_uniqueID = counter++;
	m_jointTag = jointTag;
	m_loadType = LoadType::POINT_LOAD;

	m_loadVector.resize(6);
	m_loadVector[0] = fx;
	m_loadVector[1] = fy;
	m_loadVector[2] = fz;
	m_loadVector[3] = mx;
	m_loadVector[4] = my;
	m_loadVector[5] = mz;
}

int PointLoad::getJointTag() const
{
	return m_jointTag;
}