#include "Joint.h"

using namespace std;
using namespace physicalModel;

Joint::Joint(int JointTag, vector<double> coords, vector<double> massValues) :
	m_jointTag(JointTag), m_coords(coords), m_massValues(massValues)
{
}

int Joint::getJointTag() const
{
	return m_jointTag;
}

const vector<double>& Joint::getCoords() const
{
	return m_coords;
}

const vector<double>& Joint::getMassValues() const
{
	return m_massValues;
}

const vector<int>& Joint::getConnectedBeamTags() const
{
	return m_connectedBeamTags;
}

const vector<int>& Joint::getConnectedColumnTags() const
{
	return m_connectedColumnTags;
}

const vector<int>& Joint::getConnectedSlabElementTags() const
{
	return m_connectedSlabElementTags;
}

const vector<int>& Joint::getConnectedWallTags() const
{
	return m_connectedWallTags;
}

int Joint::getFloorNo() const
{
	return m_floorNo;
}

void Joint::setMassValues(vector<double> massValues)
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

void Joint::setFloorNo(int floorNo)
{
	m_floorNo = floorNo;
}

