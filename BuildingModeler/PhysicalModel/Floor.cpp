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
	m_masterJoint = -1;
	m_massCenter = std::nullopt;
}

bool Floor::updateMassCenter()
{
	double sumMassX = 0.0;
	double sumMassY = 0.0;
	double sumCoordsX = 0.0;
	double sumCoordsY = 0.0;
	double sumMassMomentX = 0.0;
	double sumMassMomentY = 0.0;

	for (auto jointTag : m_jointTags) {
		auto joint = Building::getInstance().getJoint(jointTag);
		auto coords = joint->getCoords();

		auto mass = joint->getTranslationalMass();
		if (mass != std::nullopt) {
			sumMassX += mass.value().x;
			sumMassY += mass.value().y;
			sumCoordsX += coords.x;
			sumCoordsY += coords.y;
			sumMassMomentX += (mass.value().x * coords.y);
			sumMassMomentY += (mass.value().y * coords.x);
		}
	}

	if (sumMassX < 1e-10 && sumMassY < 1e-10) {
		return false;
	}
	else if (sumMassX < 1e-10) {
		m_massCenter = { sumMassMomentY / sumMassY,  sumCoordsY / m_jointTags.size() };
		m_diaphragmMass = { 0, sumMassY, 0 };
	}
	else if (sumMassY < 1e-10) {
		m_massCenter = { sumCoordsX / m_jointTags.size(), sumMassMomentX / sumMassX };
		m_diaphragmMass = { sumMassX, 0, 0 };
	}
	else {
		m_massCenter = { sumMassMomentY / sumMassY, sumMassMomentX / sumMassX };

		double rotationalInertia = 0.0;
		for (auto jointTag : m_jointTags) {
			auto joint = Building::getInstance().getJoint(jointTag);
			auto coords = joint->getCoords();

			auto mass = joint->getTranslationalMass();
			if (mass != std::nullopt) {
				rotationalInertia += (mass.value().x * (m_massCenter.value().y - coords.y) * (m_massCenter.value().y - coords.y));
				rotationalInertia += (mass.value().y * (m_massCenter.value().x - coords.x) * (m_massCenter.value().x - coords.x));
			}
		}

		m_diaphragmMass = { sumMassX, sumMassY, rotationalInertia };
	}

	return true;
}

void Floor::updateStiffnessCenter()
{
	// To be implemented
}

void Floor::confineFloorMassOnDiaphragmNode(bool confineFloorMassOnDiaphragmNode)
{
	m_confineFloorMassOnDiaphragmNode = confineFloorMassOnDiaphragmNode;
}

void Floor::setLiveLoadPerArea(double liveLoadPerArea)
{
	m_liveLoadPerArea = liveLoadPerArea;
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

std::optional<utility::Vector3> Floor::getDiaphragmMass() const
{
	return m_diaphragmMass;
}

std::optional<utility::Vector2> Floor::getMassCenter() const
{
	return m_massCenter;
}

std::optional<utility::Vector2> Floor::getStiffnessCenter() const
{
	return m_stiffnessCenter;
}

bool Floor::floorMassConfinedOnDiaphragmNode() const
{
	return m_confineFloorMassOnDiaphragmNode;
}

const std::optional<double> Floor::getLiveLoadPerArea() const
{
	return m_liveLoadPerArea;
}
