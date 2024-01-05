#include "BuildingModeler.h"

using namespace buildingModeler;

void BuildingModeler::addJoint(int jointTag, utility::Vector3 coords, utility::Vector3 massTranslational, utility::Vector3 massRotational)
{
    auto building = physicalModel::Building::getInstance();

    if (building.m_joints.find(jointTag) == building.m_joints.end()) {
        building.m_joints[jointTag] = std::make_unique<physicalModel::Joint>(jointTag, coords, massTranslational, massRotational);
    }
    else {
        // To do: exception
    }
}

void BuildingModeler::setTranslationalMass(int jointTag, utility::Vector3 massValues)
{
    auto building = physicalModel::Building::getInstance();

    if (building.m_joints.find(jointTag) != building.m_joints.end()) {
        building.m_joints[jointTag]->setTranslationalMass(massValues);
    }
    else {
        // To do: exception
    }
}

void BuildingModeler::setRotationalMass(int jointTag, utility::Vector3 massValues)
{
    auto building = physicalModel::Building::getInstance();

    if (building.m_joints.find(jointTag) != building.m_joints.end()) {
        building.m_joints[jointTag]->setRotationalMass(massValues);
    }
    else {
        // To do: exception
    }
}

void BuildingModeler::setConstraintVector(int jointTag, std::vector<int> constraintVector)
{
    auto building = physicalModel::Building::getInstance();

    if (building.m_joints.find(jointTag) != building.m_joints.end()) {
        building.m_joints[jointTag]->setConstraintVector(constraintVector);
    }
    else {
        // To do: exception
    }
}

void BuildingModeler::setFloorNo(int jointTag, int floorNo)
{
    auto building = physicalModel::Building::getInstance();

    if (building.m_joints.find(jointTag) != building.m_joints.end()) {
        building.m_joints[jointTag]->setFloorNo(floorNo);
    }
    else {
        // To do: exception
    }
}