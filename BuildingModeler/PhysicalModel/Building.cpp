#include "Building.h"
#include "../OpenseesConverter.h"

using namespace physicalModel;

void Building::deleteJoint(int jointTag)
{
    auto it = m_joints.find(jointTag);
    if (it != m_joints.end()) {
        m_joints.erase(jointTag);
    }
}

Building& Building::getInstance()
{
    static Building instance;
    return instance;
}

Joint* Building::getJoint(int jointTag) const
{
    auto it = m_joints.find(jointTag);
    return (it != m_joints.end()) ? it->second.get() : nullptr;
}

LineElement* Building::getLineElement(int elementTag) const
{
    auto it = m_lineElements.find(elementTag);
    return (it != m_lineElements.end()) ? it->second.get() : nullptr;
}

AreaElement* Building::getAreaElement(int elementTag) const
{
    auto it = m_areaElements.find(elementTag);
    return (it != m_areaElements.end()) ? it->second.get() : nullptr;
}

Floor* Building::getFloor(int floorNumber) const
{
    auto it = m_floors.find(floorNumber);
    return (it != m_floors.end()) ? it->second.get() : nullptr;
}

std::shared_ptr<Material> Building::getMaterial(int materialTag) const
{
    auto it = m_materials.find(materialTag);
    return (it != m_materials.end()) ? it->second : nullptr;
}

std::shared_ptr<Section> Building::getSection(int sectionTag) const
{
    auto it = m_sections.find(sectionTag);
    return (it != m_sections.end()) ? it->second : nullptr;
}

void Building::addNodesAndMasses()
{
    for (auto it = m_joints.begin(); it != m_joints.end(); it++) {
        buildingModeler::OpenseesConverter::toNodeAndMass(it->second.get());
    }
}