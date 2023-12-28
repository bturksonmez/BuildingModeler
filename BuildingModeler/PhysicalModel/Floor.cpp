#include "Floor.h"

using namespace physicalModel;

Floor::Floor(int floorNumber) : m_floorNumber(floorNumber)
{
	m_isRigid = false;
}

int Floor::getFloorNumber() const
{
	return m_floorNumber;
}

const std::vector<int>& Floor::getJoints() const
{
	return m_joints;
}

void Floor::addJoint(int jointTag)
{
	m_joints.push_back(jointTag);
}

void Floor::makeRigid()
{
	m_isRigid = true;
}

void Floor::makeFlexible()
{
	m_isRigid = false;
}