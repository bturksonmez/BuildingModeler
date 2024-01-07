#include "Building.h"

using namespace physicalModel;

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
/*
LineElement* Building::getColumn(int elementTag) const
{
    auto it = m_columns.find(elementTag);
    return (it != m_columns.end()) ? it->second.get() : nullptr;
}

LineElement* Building::getBeam(int elementTag) const
{
    auto it = m_beams.find(elementTag);
    return (it != m_beams.end()) ? it->second.get() : nullptr;
}

AreaElement* Building::getSlab(int elementTag) const
{
    auto it = m_slabs.find(elementTag);
    return (it != m_slabs.end()) ? it->second.get() : nullptr;
}

AreaElement* Building::getShearWall(int elementTag) const
{
    auto it = m_shearWalls.find(elementTag);
    return (it != m_shearWalls.end()) ? it->second.get() : nullptr;
}

Floor* Building::getFloor(int floorNumber) const
{
    auto it = m_floors.find(floorNumber);
    return (it != m_floors.end()) ? it->second.get() : nullptr;
}
*/

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