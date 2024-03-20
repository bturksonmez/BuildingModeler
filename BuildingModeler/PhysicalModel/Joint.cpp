#include "Joint.h"
#include "../OpenSeesWrapper/OpenseesModel.h"

using namespace physicalModel;
using namespace utility;

Joint::Joint(int jointTag, Vector3 coords) : m_jointTag(jointTag), m_coords(coords)
{
}

void Joint::addTranslationalMass(Vector3 massValues)
{
	if (m_massTranslational == std::nullopt) {
		m_massTranslational = massValues;
	}
	else {
		m_massTranslational = m_massTranslational.value() + massValues;
	}
}

void Joint::addRotationalMass(Vector3 massValues)
{
	if (m_massRotational == std::nullopt) {
		m_massRotational = massValues;
	}
	else {
		m_massRotational = m_massRotational.value() + massValues;
	}
}

void Joint::addConnectedBeam(int elementTag)
{
	m_connectedBeamTags.push_back(elementTag);
}

void Joint::addConnectedColumn(int elementTag)
{
	m_connectedColumnTags.push_back(elementTag);
}

void Joint::addConnectedSlab(int elementTag)
{
	m_connectedSlabTags.push_back(elementTag);
}

void Joint::addConnectedWall(int elementTag)
{
	m_connectedWallTags.push_back(elementTag);
}

void Joint::setConstraintVector(std::vector<int> constraintVector)
{
	m_constraintVector = constraintVector;
}

void Joint::setFloorNo(int floorNo)
{
	m_floorNo = floorNo;
}

int Joint::getJointTag() const
{
	return m_jointTag;
}

Vector3 Joint::getCoords() const
{
	return m_coords;
}

std::optional<Vector3> Joint::getTranslationalMass() const
{
	return m_massTranslational;
}

std::optional<Vector3> Joint::getRotationalMass() const
{
	return m_massRotational;
}

const std::vector<int>& Joint::getConnectedBeamTags() const
{
	return m_connectedBeamTags;
}

const std::vector<int>& Joint::getConnectedColumnTags() const
{
	return m_connectedColumnTags;
}

const std::vector<int>& Joint::getConnectedSlabTags() const
{
	return m_connectedSlabTags;
}

const std::vector<int>& Joint::getConnectedWallTags() const
{
	return m_connectedWallTags;
}

std::optional<std::vector<int>> Joint::getConstraintVector() const
{
	return m_constraintVector;
}

int Joint::getFloorNo() const
{
	return m_floorNo;
}

const utility::Vector3& Joint::getTranslationalMassFromAnalyticalNode() const
{
	auto mass = opensees::OpenseesModel::getInstance().getMass(m_jointTag);

	if (mass != nullptr) {
		return mass->getTranslationalMass();
	}

	return {};
}

const utility::Vector3& Joint::getRotationalMassFromAnalyticalNode() const
{
	auto mass = opensees::OpenseesModel::getInstance().getMass(m_jointTag);

	if (mass != nullptr) {
		return mass->getRotationalMass();
	}

	return {};
}