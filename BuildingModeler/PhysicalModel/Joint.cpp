#include "Joint.h"

using namespace physicalModel;
using namespace utility;

Joint::Joint(int jointTag, Vector3 coords, Vector3 massTranslational, Vector3 massRotational) :
	m_jointTag(jointTag), m_coords(coords), m_massTranslational(massTranslational), m_massRotational(massRotational)
{
}

void Joint::setTranslationalMass(Vector3 massValues)
{
	m_massTranslational = massValues;
}

void Joint::setRotationalMass(Vector3 massValues)
{
	m_massRotational = massValues;
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

Vector3 Joint::getTranslationalMass() const
{
	return m_massTranslational;
}

Vector3 Joint::getRotationalMass() const
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

const std::vector<int>& Joint::getConstraintVector() const
{
	return m_constraintVector;
}

int Joint::getFloorNo() const
{
	return m_floorNo;
}