#include "Floor.h"
#include "Building.h"

using namespace physicalModel;
using namespace utility;

Floor::Floor(int floorNumber, double height) : m_floorNumber(floorNumber), m_height(height)
{
	m_isRigid = false;
	m_massCenter.value().x = -1;
	m_massCenter.value().y = -1;
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
	m_masterJoint = -1;
	m_massCenter.value().x = -1;
	m_massCenter.value().y = -1;
}

bool Floor::updateMassCenter()
{
	double sumMassX = 0.0;
	double sumMassY = 0.0;
	double sumMassMomentX = 0.0;
	double sumMassMomentY = 0.0;

	for (auto jointTag : m_jointTags) {
		auto joint = Building::getInstance().getJoint(jointTag);
		auto coords = joint->getCoords();

		auto mass = joint->getTranslationalMass();
		if (mass != std::nullopt) {
			sumMassX += mass.value().x;
			sumMassY += mass.value().y;
			sumMassMomentX += (mass.value().x * coords.x);
			sumMassMomentY += (mass.value().y * coords.y);
		}
	}

	if (sumMassX < 1e-10 || sumMassY < 1e-10) {
		return false;
	}

	m_massCenter.value().x = sumMassMomentX / sumMassX;
	m_massCenter.value().y = sumMassMomentY / sumMassY;

	return true;
}

void Floor::updateStiffnessCenter()
{
	// To be implemented
}

int Floor::getFloorNumber() const
{
	return m_floorNumber;
}

double Floor::getFloorHeight() const
{
	return m_height;
}

double Floor::getFloorMass() const
{
	return m_mass;
}

bool Floor::isRigid() const
{
	return m_isRigid;
}

int Floor::getMassCenterJointTag() const
{
	return m_masterJoint;
}

const std::vector<int>& Floor::getJoints() const
{
	return m_jointTags;
}

std::optional<utility::Vector2> Floor::getMassCenter() const
{
	return m_massCenter;
}

std::optional<utility::Vector2> Floor::getStiffnessCenter() const
{
	return m_stiffnessCenter;
}

