#include "Floor.h"

using namespace std;
using namespace physicalModel;

Floor::Floor(int floorNumber) : m_floorNumber(floorNumber)
{
}

void Floor::setFloorPlan(FloorPlan floorPlan)
{
	m_floorPlan = floorPlan;
}

int Floor::getFloorNumber() const
{
	return m_floorNumber;
}

const FloorPlan& Floor::getFloorPlan() const
{
	return m_floorPlan;
}