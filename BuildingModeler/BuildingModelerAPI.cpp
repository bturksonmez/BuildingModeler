#include "BuildingModelerAPI.h"

using namespace buildingModeler;

void BuildingModelerAPI::addJoint(int jointTag, utility::Vector3 coords, utility::Vector3 massTranslational, utility::Vector3 massRotational)
{
    if (!jointExists(jointTag)) {
        physicalModel::Building::getInstance().m_joints[jointTag] = std::make_unique<physicalModel::Joint>(jointTag, coords, massTranslational, massRotational);
    }
    else {
        // To do: exception
    }
}

void BuildingModelerAPI::setTranslationalMass(int jointTag, utility::Vector3 massValues)
{
    if (jointExists(jointTag)) {
        physicalModel::Building::getInstance().m_joints[jointTag]->setTranslationalMass(massValues);
    }
    else {
        // To do: exception
    }
}

void BuildingModelerAPI::setRotationalMass(int jointTag, utility::Vector3 massValues)
{
    if (jointExists(jointTag)) {
        physicalModel::Building::getInstance().m_joints[jointTag]->setRotationalMass(massValues);
    }
    else {
        // To do: exception
    }
}

void BuildingModelerAPI::setConstraintVector(int jointTag, std::vector<int> constraintVector)
{
    if (jointExists(jointTag)) {
        physicalModel::Building::getInstance().m_joints[jointTag]->setConstraintVector(constraintVector);
    }
    else {
        // To do: exception
    }
}

void BuildingModelerAPI::setFloorNo(int jointTag, int floorNo)
{
    if (jointExists(jointTag)) {
        physicalModel::Building::getInstance().m_joints[jointTag]->setFloorNo(floorNo);
    }
    else {
        // To do: exception
    }
}

void BuildingModelerAPI::addBeam(int elementTag, std::vector<int> jointTags, int sectionTag,
    physicalModel::LineElementFormulation lineElementFormulation)
{
    if (beamExists(elementTag)) {
        // To do: exception
    }
    else if (!jointExists(jointTags[0])) {
        // To do: exception
    }
    else if (!jointExists(jointTags[1])) {
        // To do: exception
    }
    else if (!sectionExists(sectionTag)) {
        // To do: exception
    }
    else {
        std::shared_ptr<physicalModel::Section> section = physicalModel::Building::getInstance().m_sections[sectionTag];
        physicalModel::Building::getInstance().m_beams[elementTag] = std::make_unique<physicalModel::BeamElement>(elementTag, jointTags, section, lineElementFormulation);
    }
}

bool BuildingModelerAPI::jointExists(int jointTag)
{
    if (physicalModel::Building::getInstance().m_joints.find(jointTag) != physicalModel::Building::getInstance().m_joints.end()) {
        return true;
    }

    return false;
}

bool BuildingModelerAPI::beamExists(int elementTag)
{
    if (physicalModel::Building::getInstance().m_beams.find(elementTag) != physicalModel::Building::getInstance().m_beams.end()) {
        return true;
    }

    return false;
}

bool BuildingModelerAPI::columnExists(int elementTag)
{
    if (physicalModel::Building::getInstance().m_columns.find(elementTag) != physicalModel::Building::getInstance().m_columns.end()) {
        return true;
    }

    return false;
}

bool BuildingModelerAPI::slabExists(int elementTag)
{
    if (physicalModel::Building::getInstance().m_slabs.find(elementTag) != physicalModel::Building::getInstance().m_slabs.end()) {
        return true;
    }

    return false;
}

bool BuildingModelerAPI::shearWallExists(int elementTag)
{
    if (physicalModel::Building::getInstance().m_shearWalls.find(elementTag) != physicalModel::Building::getInstance().m_shearWalls.end()) {
        return true;
    }

    return false;
}

bool BuildingModelerAPI::floorExists(int floorNo)
{
    if (physicalModel::Building::getInstance().m_floors.find(floorNo) != physicalModel::Building::getInstance().m_floors.end()) {
        return true;
    }

    return false;
}

bool BuildingModelerAPI::materialExists(int materialTag)
{
    if (physicalModel::Building::getInstance().m_materials.find(materialTag) != physicalModel::Building::getInstance().m_materials.end()) {
        return true;
    }

    return false;
}

bool BuildingModelerAPI::sectionExists(int sectionTag)
{
    if (physicalModel::Building::getInstance().m_sections.find(sectionTag) != physicalModel::Building::getInstance().m_sections.end()) {
        return true;
    }

    return false;
}