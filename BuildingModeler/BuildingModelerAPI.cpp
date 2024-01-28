#include "BuildingModelerAPI.h"

using namespace buildingModeler;

void BuildingModelerAPI::addJoint(int jointTag, utility::Vector3 coords)
{
    if (jointExists(jointTag)) {
        // To do: exception
    }
    else {
        physicalModel::Building::getInstance().m_joints[jointTag] = std::make_unique<physicalModel::Joint>(jointTag, coords);
    }

}

void BuildingModelerAPI::addTranslationalMass(int jointTag, utility::Vector3 massValues)
{
    if (!jointExists(jointTag)) {
        // To do: exception
    }
    else {
        physicalModel::Building::getInstance().m_joints[jointTag]->addTranslationalMass(massValues);
    }
}

void BuildingModelerAPI::addRotationalMass(int jointTag, utility::Vector3 massValues)
{
    if (!jointExists(jointTag)) {
        // To do: exception
    }
    else {
        physicalModel::Building::getInstance().m_joints[jointTag]->addRotationalMass(massValues);
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

void BuildingModelerAPI::setFloorNo(int jointTag, int floorNumber)
{
    if (!jointExists(jointTag)) {
        // To do: exception
    }
    else if (floorExists(floorNumber)) {
        // To do: exception
    }
    else {
        physicalModel::Building::getInstance().m_joints[jointTag]->setFloorNo(floorNumber);
        physicalModel::Building::getInstance().m_floors[floorNumber]->addJoint(jointTag);
    }
}

void BuildingModelerAPI::includeMassFromMembers(bool includeMassFromMembers)
{
    physicalModel::Building::getInstance().m_includeMassFromMembers = includeMassFromMembers;
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
        physicalModel::Building::getInstance().m_joints[jointTags[0]]->addConnectedBeam(elementTag);
        physicalModel::Building::getInstance().m_joints[jointTags[1]]->addConnectedBeam(elementTag);
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
        physicalModel::Building::getInstance().m_joints[jointTags[0]]->addConnectedColumn(elementTag);
        physicalModel::Building::getInstance().m_joints[jointTags[1]]->addConnectedColumn(elementTag);
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
    
    physicalModel::Building::getInstance().m_lineElements[elementTag]->setSectionModifiers(segmentNo, std::make_shared<physicalModel::SectionModifiers>(modifierA, modifierIyy, modifierIzz, modifierJ));
}

void BuildingModelerAPI::addShearWall(int elementTag, std::vector<int> jointTags, int sectionTag,
    physicalModel::AreaElementFormulation areaElementFormulation)
{
    if (areaElementExists(elementTag)) {
        // To do: exception
    }
    else if (!jointExists(jointTags[0]) || !jointExists(jointTags[1]) || !jointExists(jointTags[1]) || !jointExists(jointTags[2])) {
        // To do: exception
    }
    else if (!sectionExists(sectionTag)) {
        // To do: exception
    }
    
    std::shared_ptr<physicalModel::Section> section = physicalModel::Building::getInstance().m_sections[sectionTag];
    physicalModel::Building::getInstance().m_areaElements[elementTag] = std::make_unique<physicalModel::ShearWallElement>(elementTag, jointTags, section, areaElementFormulation);
    physicalModel::Building::getInstance().m_joints[jointTags[0]]->addConnectedWall(elementTag);
    physicalModel::Building::getInstance().m_joints[jointTags[1]]->addConnectedWall(elementTag);
    physicalModel::Building::getInstance().m_joints[jointTags[2]]->addConnectedWall(elementTag);
    physicalModel::Building::getInstance().m_joints[jointTags[3]]->addConnectedWall(elementTag);
}

void BuildingModelerAPI::addSlab(int elementTag, std::vector<int> jointTags, int sectionTag,
    physicalModel::AreaElementFormulation areaElementFormulation)
{
    if (areaElementExists(elementTag)) {
        // To do: exception
    }
    else if (!jointExists(jointTags[0]) || !jointExists(jointTags[1]) || !jointExists(jointTags[1]) || !jointExists(jointTags[2])) {
        // To do: exception
    }
    else if (!sectionExists(sectionTag)) {
        // To do: exception
    }
    
    std::shared_ptr<physicalModel::Section> section = physicalModel::Building::getInstance().m_sections[sectionTag];
    physicalModel::Building::getInstance().m_areaElements[elementTag] = std::make_unique<physicalModel::SlabElement>(elementTag, jointTags, section, areaElementFormulation);
    physicalModel::Building::getInstance().m_joints[jointTags[0]]->addConnectedSlab(elementTag);
    physicalModel::Building::getInstance().m_joints[jointTags[1]]->addConnectedSlab(elementTag);
    physicalModel::Building::getInstance().m_joints[jointTags[2]]->addConnectedSlab(elementTag);
    physicalModel::Building::getInstance().m_joints[jointTags[3]]->addConnectedSlab(elementTag);
}

void BuildingModelerAPI::meshAreaElement(int elementTag, bool meshable, int n1, int n2)
{
    if (!areaElementExists(elementTag)) {
        // To do: exception
    }

    physicalModel::Building::getInstance().m_areaElements[elementTag]->mesh(meshable, n1, n2);
}

void BuildingModelerAPI::addFloor(int floorNumber, double height)
{
    if (floorNumber < 0) {
        // To do: exception
    }
    else if (floorExists(floorNumber)) {
        // To do: exception
    }
    else {
        if (floorNumber == 0) {
            physicalModel::Building::getInstance().m_floors[floorNumber] = std::make_unique<physicalModel::Floor>(floorNumber);
        }
        else {
            physicalModel::Building::getInstance().m_floors[floorNumber] = std::make_unique<physicalModel::Floor>(floorNumber, height);
        }
    }
}

void BuildingModelerAPI::makeRigid(int floorNumber, int masterJointTag, utility::Vector3 coords)
{
    if (jointExists(masterJointTag)) {
        // To do: exception
    }
    else if (!floorExists(floorNumber)) {
        // To do: exception
    }
    else if (physicalModel::Building::getInstance().m_floors[floorNumber]->isRigid()) {
        // To do: exception
    }
    else {
        physicalModel::Building::getInstance().m_joints[masterJointTag] = std::make_unique<physicalModel::Joint>(masterJointTag, coords);
        physicalModel::Building::getInstance().m_floors[floorNumber]->makeRigid(masterJointTag);
    }
}

void BuildingModelerAPI::makeFlexible(int floorNumber)
{
    if (!floorExists(floorNumber)) {
    // To do: exception
    }
    else if (!physicalModel::Building::getInstance().m_floors[floorNumber]->isRigid()) {
    // To do: exception
    }
    else {
        physicalModel::Building::getInstance().m_floors[floorNumber]->makeFlexible();
    }
}

void BuildingModelerAPI::includePDeltaEffects(bool includePDeltaEffects)
{
    physicalModel::Building::getInstance().m_includePDeltaEffects = includePDeltaEffects;
}

void BuildingModelerAPI::updateAreaElementProperties()
{
    physicalModel::Building::getInstance().updateSurroundingLineElements();
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

void BuildingModelerAPI::createInputFile()
{
    physicalModel::Building::getInstance().toAnalyticalModel();
    opensees::OpenseesModel::getInstance().toTclFile();
}