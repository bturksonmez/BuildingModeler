#include "Building.h"

using namespace physicalModel;

Building& Building::getInstance()
{
    static Building instance;
    return instance;
}

void Building::deleteJoint(int jointTag)
{
    m_joints.
}

void Building::deleteLineElement(int elementTag);
void Building::deleteAreaElement(int elementTag);
void Building::deleteFloor(int floorNumber);
void Building::deleteMaterial(int materialTag);
void Building::deleteSection(int sectionTag);

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