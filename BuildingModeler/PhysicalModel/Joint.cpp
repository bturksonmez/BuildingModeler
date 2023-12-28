#include "Joint.h"

using namespace physicalModel;

Joint::Joint(int JointTag, std::vector<double> coords, std::vector<double> massValues) :
	m_jointTag(JointTag), m_coords(coords), m_massValues(massValues)
{
}

int Joint::getJointTag() const
{
	return m_jointTag;
}

const std::vector<double>& Joint::getCoords() const
{
	return m_coords;
}

const std::vector<double>& Joint::getMassValues() const
{
	return m_massValues;
}

const std::vector<int>& Joint::getConnectedBeamTags() const
{
	return m_connectedBeamTags;
}

const std::vector<int>& Joint::getConnectedColumnTags() const
{
	return m_connectedColumnTags;
}

const std::vector<int>& Joint::getConnectedSlabElementTags() const
{
	return m_connectedSlabElementTags;
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

void Joint::setMassValues(std::vector<double> massValues)
{
	m_massValues = massValues;
}

void Joint::addConnectedBeam(int elementTag)
{
	m_connectedBeamTags.push_back(elementTag);
}

void Joint::addConnectedColumn(int elementTag)
{
	m_connectedColumnTags.push_back(elementTag);
}

void Joint::addConnectedSlabElement(int elementTag)
{
	m_connectedSlabElementTags.push_back(elementTag);
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

