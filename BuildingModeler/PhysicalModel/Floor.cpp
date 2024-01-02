#include "Floor.h"

using namespace physicalModel;
using namespace utility;

Floor::Floor(int floorNumber, double height) : m_floorNumber(floorNumber), m_height(height)
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

const Vector2& Floor::getMassCenter() const
{
	return m_massCenter;
}

const Vector2& Floor::getStiffnessCenter() const
{
	return m_stiffnessCenter;
}

void Floor::addJoint(int jointTag)
{
	m_joints.push_back(jointTag);
}

void Floor::makeRigid(int masterJoint)
{
	m_isRigid = true;
	m_masterJoint = masterJoint;
}

void Floor::makeFlexible()
{
	m_isRigid = false;
}

void Floor::updateProperties()
{
	updateMassCenter();
	updateStiffnessCenter();
}

void Floor::updateMassCenter()
{
	// To be implemented
}

void Floor::updateStiffnessCenter()
{
	// To be implemented
}