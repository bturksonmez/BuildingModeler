#include "Floor.h"
#include "Building.h"

using namespace physicalModel;
using namespace utility;

Floor::Floor(int floorNumber, double height) : m_floorNumber(floorNumber), m_height(height)
{
	m_isRigid = false;
}

void Floor::addJoint(int jointTag)
{
	m_jointTags.push_back(jointTag);
}

void Floor::makeRigid(int masterJoint)
{
	m_isRigid = true;
	m_masterJoint = masterJoint;
}

void Floor::makeFlexible()
{
	m_isRigid = false;
	Building::getInstance().deleteJoint(m_masterJoint);
	m_masterJoint = -1;
}

void Floor::updateProperties()
{
	updateMassCenter();
	updateStiffnessCenter();
}

int Floor::getFloorNumber() const
{
	return m_floorNumber;
}

bool Floor::isRigid() const
{
	return m_isRigid;
}

const std::vector<int>& Floor::getJoints() const
{
	return m_jointTags;
}

const Vector2& Floor::getMassCenter() const
{
	return m_massCenter;
}

const Vector2& Floor::getStiffnessCenter() const
{
	return m_stiffnessCenter;
}

void Floor::updateMassCenter()
{
	// To be implemented
}

void Floor::updateStiffnessCenter()
{
	// To be implemented
}