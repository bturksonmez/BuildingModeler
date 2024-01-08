#include "BuildingModelerAPI.h"

using namespace buildingModeler;

void BuildingModelerAPI::addJoint(int jointTag, utility::Vector3 coords, utility::Vector3 massTranslational, utility::Vector3 massRotational)
{
    if (jointExists(jointTag)) {
        // To do: exception
    }
    else {
        physicalModel::Building::getInstance().m_joints[jointTag] = std::make_unique<physicalModel::Joint>(jointTag, coords, massTranslational, massRotational);
    }
}

void BuildingModelerAPI::setTranslationalMass(int jointTag, utility::Vector3 massValues)
{
    if (!jointExists(jointTag)) {
        // To do: exception
    }
    else {
        physicalModel::Building::getInstance().m_joints[jointTag]->setTranslationalMass(massValues);
    }
}

void BuildingModelerAPI::setRotationalMass(int jointTag, utility::Vector3 massValues)
{
    if (!jointExists(jointTag)) {
        // To do: exception
    }
    else {
        physicalModel::Building::getInstance().m_joints[jointTag]->setRotationalMass(massValues);
    }
}

void BuildingModelerAPI::setConstraintVector(int jointTag, std::vector<int> constraintVector)
{
    if (!jointExists(jointTag)) {
        // To do: exception
    }
    else {
        physicalModel::Building::getInstance().m_joints[jointTag]->setConstraintVector(constraintVector);
    }
}

void BuildingModelerAPI::setFloorNo(int jointTag, int floorNo)
{
    if (!jointExists(jointTag)) {
        // To do: exception
    }
    else {
        physicalModel::Building::getInstance().m_joints[jointTag]->setFloorNo(floorNo);
    }
}

void BuildingModelerAPI::addElasticMaterial(int materialTag, double E, double G, double rho)
{
    if (materialExists(materialTag)) {
        // To do: exception
    }
    else {
        physicalModel::Building::getInstance().m_materials[materialTag] = std::make_unique<physicalModel::ElasticMaterial>(materialTag, E, G, rho);
    }
}
//
void BuildingModelerAPI::addElasticSection1D(int sectionTag, int materialTag, double A, double Iyy, double Izz, double J)
{
    if (sectionExists(sectionTag)) {
        // To do: exception
    }
    else if (!materialExists(materialTag)) {
        // To do: exception
    }
    else {
        std::shared_ptr<physicalModel::Material> material = physicalModel::Building::getInstance().m_materials[materialTag];
        physicalModel::Building::getInstance().m_sections[sectionTag] = std::make_unique<physicalModel::ElasticSection1D>(sectionTag, material, A, Iyy, Izz, J);
    }
}

void BuildingModelerAPI::addElasticSection2D(int sectionTag, int materialTag, double thickness)
{
    if (sectionExists(sectionTag)) {
        // To do: exception
    }
    else if (!materialExists(materialTag)) {
        // To do: exception
    }
    else {
        std::shared_ptr<physicalModel::Material> material = physicalModel::Building::getInstance().m_materials[materialTag];
        physicalModel::Building::getInstance().m_sections[sectionTag] = std::make_unique<physicalModel::ElasticSection2D>(sectionTag, material, thickness);
    }
}

void BuildingModelerAPI::addBeam(int elementTag, std::vector<int> jointTags, int sectionTag,
    physicalModel::LineElementFormulation lineElementFormulation)
{
    if (lineElementExists(elementTag)) {
        // To do: exception
    }
    else if (!jointExists(jointTags[0]) || !jointExists(jointTags[1])) {
        // To do: exception
    }
    else if (!sectionExists(sectionTag)) {
        // To do: exception
    }
    else {
        std::shared_ptr<physicalModel::Section> section = physicalModel::Building::getInstance().m_sections[sectionTag];
        physicalModel::Building::getInstance().m_lineElements[elementTag] = std::make_unique<physicalModel::BeamElement>(elementTag, jointTags, section, lineElementFormulation);
    }
}

void BuildingModelerAPI::addColumn(int elementTag, std::vector<int> jointTags, int sectionTag,
    physicalModel::LineElementFormulation lineElementFormulation)
{
    if (lineElementExists(elementTag)) {
        // To do: exception
    }
    else if (!jointExists(jointTags[0]) || !jointExists(jointTags[1])) {
        // To do: exception
    }
    else if (!sectionExists(sectionTag)) {
        // To do: exception
    }
    else {
        std::shared_ptr<physicalModel::Section> section = physicalModel::Building::getInstance().m_sections[sectionTag];
        physicalModel::Building::getInstance().m_lineElements[elementTag] = std::make_unique<physicalModel::ColumnElement>(elementTag, jointTags, section, lineElementFormulation);
    }
}

void BuildingModelerAPI::setSegmentRatios(int elementTag, std::vector<double> segmentRatios)
{
    if (!lineElementExists(elementTag)) {
        // To do: exception
    }
    else {
        physicalModel::Building::getInstance().m_lineElements[elementTag]->setSegmentRelativeLengths(segmentRatios);
    }
}

void BuildingModelerAPI::setSection(int elementTag, int segmentNo, int sectionTag)
{
    if (!lineElementExists(elementTag)) {
        // To do: exception
    }
    else if (!sectionExists(sectionTag)) {
        // To do: exception
    }
    else {
        std::shared_ptr<physicalModel::Section> section = physicalModel::Building::getInstance().m_sections[sectionTag];
        physicalModel::Building::getInstance().m_lineElements[elementTag]->setSection(segmentNo, section);
    }
}

void BuildingModelerAPI::setSectionModifiers(int elementTag, int segmentNo, double modifierA, double modifierIyy, double modifierIzz, double modifierJ)
{
    if (!lineElementExists(elementTag)) {
        // To do: exception
    }
    else {
        physicalModel::Building::getInstance().m_lineElements[elementTag]->setSectionModifiers(segmentNo, std::make_shared<physicalModel::SectionModifiers>(modifierA, modifierIyy, modifierIzz, modifierJ));
    }
}

bool BuildingModelerAPI::jointExists(int jointTag)
{
    if (physicalModel::Building::getInstance().m_joints.find(jointTag) != physicalModel::Building::getInstance().m_joints.end()) {
        return true;
    }

    return false;
}

bool BuildingModelerAPI::lineElementExists(int elementTag)
{
    if (physicalModel::Building::getInstance().m_lineElements.find(elementTag) != physicalModel::Building::getInstance().m_lineElements.end()) {
        return true;
    }

    return false;
}

bool BuildingModelerAPI::areaElementExists(int elementTag)
{
    if (physicalModel::Building::getInstance().m_areaElements.find(elementTag) != physicalModel::Building::getInstance().m_areaElements.end()) {
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