#include "Building.h"
#include "../OpenseesConverter.h"

using namespace physicalModel;

void Building::deleteJoint(int jointTag)
{
    auto it = m_joints.find(jointTag);
    if (it != m_joints.end()) {
        m_joints.erase(jointTag);
    }

    // To do: delete also the elements connected to a joint, and remove that joint from its floor member
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

void Building::updateSurroundingLineElements()
{
    for (auto it = m_areaElements.begin(); it != m_areaElements.end(); it++) {
        auto joints = it->second->getJointTags();

        for (int i = 0; i < joints.size(); ++i) {
            auto jointTagI = joints[i];
            auto jointTagJ = (i + 1 < joints.size()) ? i + 1 : 0;

            bool found = false;
            for (const auto& beamTagI : m_joints[jointTagI]->getConnectedBeamTags()) {

                for (const auto& beamTagJ : m_joints[jointTagJ]->getConnectedBeamTags()) {
                    if (beamTagI == beamTagJ) {
                        it->second->addSurroundingLineElement(i, beamTagI);
                        found = true;
                        break;
                    }
                }

                if (found) {
                    break;
                }
            }

            found = false;
            for (const auto& columnTagI : m_joints[jointTagI]->getConnectedColumnTags()) {

                for (const auto& columnTagJ : m_joints[jointTagJ]->getConnectedColumnTags()) {
                    if (columnTagI == columnTagJ) {
                        it->second->addSurroundingLineElement(i, columnTagI);
                        found = true;
                        break;
                    }
                }

                if (found) {
                    break;
                }
            }
        }
    }
}

void Building::toAnalyticalModel()
{
    convertJoints();
    convertMaterials();
    convertSections();
    convertLineElements();
    convertAreaElements();
}


void Building::convertJoints()
{
    for (auto it = m_joints.begin(); it != m_joints.end(); it++) {
        buildingModeler::OpenseesConverter::toNodeMassConstraint(it->second.get());
    }
}

void Building::convertMaterials()
{
    for (auto it = m_materials.begin(); it != m_materials.end(); it++) {
        buildingModeler::OpenseesConverter::toMaterial(it->second.get());
    }
}

void Building::convertSections()
{
    for (auto it = m_sections.begin(); it != m_sections.end(); it++) {
        buildingModeler::OpenseesConverter::toSection(it->second.get());
    }
}

void Building::convertLineElements()
{
    buildingModeler::OpenseesConverter::includePDeltaEffects(m_includePDeltaEffects);

    for (auto it = m_lineElements.begin(); it != m_lineElements.end(); it++) {
        buildingModeler::OpenseesConverter::toBeamColumnElement(it->second.get());
    }
}

void Building::convertAreaElements()
{
    for (auto it = m_areaElements.begin(); it != m_areaElements.end(); it++) {
        buildingModeler::OpenseesConverter::toQuadrilateralElement(it->second.get());
    }
}