#include "BuildingModelerAPI.h"

using namespace buildingModeler;

void BuildingModelerAPI::addJoint(int jointTag, utility::Vector3 coords, utility::Vector3 massTranslational, utility::Vector3 massRotational)
{
    if (physicalModel::Building::getInstance().m_joints.find(jointTag) == physicalModel::Building::getInstance().m_joints.end()) {
        physicalModel::Building::getInstance().m_joints[jointTag] = std::make_unique<physicalModel::Joint>(jointTag, coords, massTranslational, massRotational);
    }
    else {
        // To do: exception
    }
}

void BuildingModelerAPI::setTranslationalMass(int jointTag, utility::Vector3 massValues)
{
    if (physicalModel::Building::getInstance().m_joints.find(jointTag) != physicalModel::Building::getInstance().m_joints.end()) {
        physicalModel::Building::getInstance().m_joints[jointTag]->setTranslationalMass(massValues);
    }
    else {
        // To do: exception
    }
}

void BuildingModelerAPI::setRotationalMass(int jointTag, utility::Vector3 massValues)
{
    if (physicalModel::Building::getInstance().m_joints.find(jointTag) != physicalModel::Building::getInstance().m_joints.end()) {
        physicalModel::Building::getInstance().m_joints[jointTag]->setRotationalMass(massValues);
    }
    else {
        // To do: exception
    }
}

void BuildingModelerAPI::setConstraintVector(int jointTag, std::vector<int> constraintVector)
{
    if (physicalModel::Building::getInstance().m_joints.find(jointTag) != physicalModel::Building::getInstance().m_joints.end()) {
        physicalModel::Building::getInstance().m_joints[jointTag]->setConstraintVector(constraintVector);
    }
    else {
        // To do: exception
    }
}

void BuildingModelerAPI::setFloorNo(int jointTag, int floorNo)
{
    if (physicalModel::Building::getInstance().m_joints.find(jointTag) != physicalModel::Building::getInstance().m_joints.end()) {
        physicalModel::Building::getInstance().m_joints[jointTag]->setFloorNo(floorNo);
    }
    else {
        // To do: exception
    }
}
