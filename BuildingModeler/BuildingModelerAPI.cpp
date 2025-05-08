#include <algorithm>
#include <cmath>
#include <unordered_set>

#include "BuildingModelerAPI.h"
#include "Utilities/VectorUtilities.h"

using namespace buildingModeler;

void BuildingModelerAPI::addJoint(int jointTag, utility::Vector3 coords)
{
    if (jointExists(jointTag)) {
        throw EntityFoundException("Joint with tag " + std::to_string(jointTag) + " already exists.");
    }
    
    physicalModel::Building::getInstance().m_joints[jointTag] = std::make_unique<physicalModel::Joint>(jointTag, coords);
}

void BuildingModelerAPI::addTranslationalMass(int jointTag, utility::Vector3 massValues)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }
    
    physicalModel::Building::getInstance().m_joints[jointTag]->addTranslationalMass(massValues);
}

void BuildingModelerAPI::addRotationalMass(int jointTag, utility::Vector3 massValues)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }
    
    physicalModel::Building::getInstance().m_joints[jointTag]->addRotationalMass(massValues);
}

void BuildingModelerAPI::setConstraintVector(int jointTag, std::vector<int> constraintVector)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }
    
    physicalModel::Building::getInstance().m_joints[jointTag]->setConstraintVector(constraintVector);
}

void BuildingModelerAPI::setFloorNo(int jointTag, int floorNumber)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }
    
    if (!floorExists(floorNumber)) {
        throw EntityNotFoundException("Floor number " + std::to_string(floorNumber) + " does not exist.");
    }
    
    auto joint = physicalModel::Building::getInstance().getJoint(jointTag);
    auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);

    if (joint->getCoords().z > floor->getFloorHeight() + 1e-10 || joint->getCoords().z < floor->getFloorHeight() - 1e-10) {
        throw InvalidOperationException("Joint cannot be assigned to floor! Floor " + std::to_string(floorNumber) + " and joint with tag "
            + std::to_string(jointTag) + "are not at the same height.");
    }

    joint->setFloorNo(floorNumber);
    floor->addJoint(jointTag);
}

void BuildingModelerAPI::includeMassFromMembers(bool includeMassFromMembers)
{
    physicalModel::Building::getInstance().m_includeMassFromMembers = includeMassFromMembers;
}

utility::Vector3 BuildingModelerAPI::getCoordinates(int jointTag)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }

    auto joint = physicalModel::Building::getInstance().getJoint(jointTag);

    return joint->getCoords();
}

std::vector<int> BuildingModelerAPI::getConstraintVectorFromAnalyticalModel(int jointTag)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }

    auto joint = physicalModel::Building::getInstance().getJoint(jointTag);

    return joint->getConstraintVectorFromAnalyticalNode();
}

std::optional<std::vector<int>> BuildingModelerAPI::getConstraintVectorFromPhysicalModel(int jointTag)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }

    auto joint = physicalModel::Building::getInstance().getJoint(jointTag);

    return joint->getConstraintVector();
}

std::vector<std::vector<int>> BuildingModelerAPI::getConstraintVectorForNodesBetween(int jointTagA, int jointTagB)
{
    if (!jointExists(jointTagA)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTagA) + " does not exist.");
    }

    if (!jointExists(jointTagB)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTagB) + " does not exist.");
    }

    auto nodesAB = opensees::OpenseesModel::getInstance().getDivisionsBetweenNodes(jointTagA, jointTagB);
    if (nodesAB.empty()) {
        nodesAB = opensees::OpenseesModel::getInstance().getDivisionsBetweenNodes(jointTagB, jointTagA);

        if (nodesAB.empty()) {
            nodesAB = { jointTagA, jointTagB };
        }
        else {
            std::reverse(nodesAB.begin(), nodesAB.end());
        }
    }

    std::vector<std::vector<int>> constraintVectors;
    for (auto nodeTag : nodesAB) {

        auto constraint = opensees::OpenseesModel::getInstance().getSPConstraint(nodeTag);

        if (constraint != nullptr) {

            auto constraintSP = dynamic_cast<opensees::SingleConstraint*>(constraint);
            if (constraintSP != nullptr) {
                constraintVectors.push_back(constraintSP->getFixedDOFs());
            }
            else {
                constraintVectors.push_back({});
            }
        }
        else {
            constraintVectors.push_back({});
        }
    }
    
    return constraintVectors;
}

utility::Vector3 BuildingModelerAPI::getTranslationalMassForJointFromAnalyticalModel(int jointTag)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }

    auto joint = physicalModel::Building::getInstance().getJoint(jointTag);

    return joint->getTranslationalMassFromAnalyticalNode();
}

std::optional<utility::Vector3> BuildingModelerAPI::getTranslationalMassForJointFromPhysicalModel(int jointTag)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }

    auto joint = physicalModel::Building::getInstance().getJoint(jointTag);

    return joint->getTranslationalMass();
}

utility::Vector3 BuildingModelerAPI::getRotationalMassForJointFromAnalyticalModel(int jointTag)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }

    auto joint = physicalModel::Building::getInstance().getJoint(jointTag);

    return joint->getRotationalMassFromAnalyticalNode();
}

std::optional<utility::Vector3> BuildingModelerAPI::getRotationalMassForJointFromPhysicalModel(int jointTag)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }

    auto joint = physicalModel::Building::getInstance().getJoint(jointTag);

    return joint->getRotationalMass();
}

void BuildingModelerAPI::addElasticMaterial(int materialTag, double E, double G, double rho)
{
    if (materialExists(materialTag)) {
        throw EntityFoundException("Material with tag " + std::to_string(materialTag) + " already exists.");
    }
    
    physicalModel::Building::getInstance().m_materials[materialTag] = std::make_shared<physicalModel::ElasticMaterial>(materialTag, E, G, rho);
}

void BuildingModelerAPI::addElasticSection1D(int sectionTag, int materialTag, physicalModel::Shape* shape)
{
    if (sectionExists(sectionTag)) {
        throw EntityFoundException("Section with tag " + std::to_string(sectionTag) + " already exists.");
    }

    if (!materialExists(materialTag)) {
        throw EntityNotFoundException("Material with tag " + std::to_string(materialTag) + " does not exist.");
    }
    
    std::shared_ptr<physicalModel::Material> material = physicalModel::Building::getInstance().m_materials[materialTag];
    physicalModel::Building::getInstance().m_sections[sectionTag] = std::make_shared<physicalModel::ElasticSection1D>(sectionTag, material, shape);
}

void BuildingModelerAPI::addElasticSection2D(int sectionTag, int materialTag, double thickness)
{
    if (sectionExists(sectionTag)) {
        throw EntityFoundException("Section with tag " + std::to_string(sectionTag) + " already exists.");
    }

    if (!materialExists(materialTag)) {
        throw EntityNotFoundException("Material with tag " + std::to_string(materialTag) + " does not exist.");
    }
    
    std::shared_ptr<physicalModel::Material> material = physicalModel::Building::getInstance().m_materials[materialTag];
    physicalModel::Building::getInstance().m_sections[sectionTag] = std::make_shared<physicalModel::ElasticSection2D>(sectionTag, material, thickness);
}

void BuildingModelerAPI::addBeam(int elementTag, std::vector<int> jointTags, int sectionTag,
    physicalModel::LineElementFormulation lineElementFormulation)
{
    if (lineElementExists(elementTag)) {
        throw EntityFoundException("Line element with tag " + std::to_string(elementTag) + " already exists.");
    }

    if (jointTags.size() != 2) {
        throw InvalidInputException("Number of joints must be two to create a line element with tag " + std::to_string(elementTag) + ".");
    }

    if (!jointExists(jointTags[0]) || !jointExists(jointTags[1])) {
        throw InvalidInputException("Joints do not exists to create line element with tag " + std::to_string(elementTag) + " .");
    }

    if (!sectionExists(sectionTag)) {
        throw EntityNotFoundException("Section with tag " + std::to_string(sectionTag) + " does not exist.");
    }

    std::shared_ptr<physicalModel::Section> section = physicalModel::Building::getInstance().m_sections[sectionTag];
    physicalModel::Building::getInstance().m_lineElements[elementTag] = std::make_unique<physicalModel::BeamElement>(elementTag, jointTags, section, lineElementFormulation);
    physicalModel::Building::getInstance().m_joints[jointTags[0]]->addConnectedBeam(elementTag);
    physicalModel::Building::getInstance().m_joints[jointTags[1]]->addConnectedBeam(elementTag);
}

void BuildingModelerAPI::addColumn(int elementTag, std::vector<int> jointTags, int sectionTag,
    physicalModel::LineElementFormulation lineElementFormulation)
{
    if (lineElementExists(elementTag)) {
        throw EntityFoundException("Line element with tag " + std::to_string(elementTag) + " already exists.");
    }

    if (jointTags.size() != 2) {
        throw InvalidInputException("Number of joints must be two to create a line element with tag " + std::to_string(elementTag) + ".");
    }

    if (!jointExists(jointTags[0]) || !jointExists(jointTags[1])) {
        throw InvalidInputException("Joints do not exists to create line element with tag " + std::to_string(elementTag) + " .");
    }

    if (!sectionExists(sectionTag)) {
        throw EntityNotFoundException("Section with tag " + std::to_string(sectionTag) + " does not exist.");
    }

    std::shared_ptr<physicalModel::Section> section = physicalModel::Building::getInstance().m_sections[sectionTag];
    physicalModel::Building::getInstance().m_lineElements[elementTag] = std::make_unique<physicalModel::ColumnElement>(elementTag, jointTags, section, lineElementFormulation);
    physicalModel::Building::getInstance().m_joints[jointTags[0]]->addConnectedColumn(elementTag);
    physicalModel::Building::getInstance().m_joints[jointTags[1]]->addConnectedColumn(elementTag);
}

void BuildingModelerAPI::setSegmentRatios(int elementTag, std::vector<double> segmentRatios)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    double totalRatio = 0.0;
    for (const auto& ratio : segmentRatios) {
        totalRatio += ratio;
    }

    if (totalRatio > 1.0 + 1e-7 || totalRatio < 1.0 - 1e-7) {
        throw InvalidInputException("Segment ratios should sum up to 1.0");
    }
    
    physicalModel::Building::getInstance().m_lineElements[elementTag]->setSegmentRelativeLengths(segmentRatios);
    invalidateAreaMeshAlongLineElement(elementTag);
}

void BuildingModelerAPI::setSection(int elementTag, int segmentNo, int sectionTag)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }
    else if (!sectionExists(sectionTag)) {
        throw EntityNotFoundException("Section with tag " + std::to_string(sectionTag) + " does not exist.");
    }
    
    std::shared_ptr<physicalModel::Section> section = physicalModel::Building::getInstance().m_sections[sectionTag];
    physicalModel::Building::getInstance().m_lineElements[elementTag]->setSection(segmentNo, section);
}

void BuildingModelerAPI::setSectionModifiers(int elementTag, int segmentNo, double modifierA, double modifierIyy, double modifierIzz, double modifierJ)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }
    
    physicalModel::Building::getInstance().m_lineElements[elementTag]->setSectionModifiers(segmentNo, std::make_shared<physicalModel::SectionModifiers>(modifierA, modifierIyy, modifierIzz, modifierJ));
}

std::vector<int> BuildingModelerAPI::getLineElementJointTags(int elementTag)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return { physicalModel::Building::getInstance().getLineElement(elementTag)->getIJointTag(), 
             physicalModel::Building::getInstance().getLineElement(elementTag)->getJJointTag() };
}

const std::vector<utility::Vector3>& BuildingModelerAPI::getNodeCoordinatesOfLineElement(int elementTag)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return physicalModel::Building::getInstance().m_lineElements[elementTag]->getAnalyticalNodeCoords();
}

double BuildingModelerAPI::getLength(int elementTag)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return physicalModel::Building::getInstance().m_lineElements[elementTag]->getLength();
}

double BuildingModelerAPI::getArea(int elementTag, int segmentNo)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return physicalModel::Building::getInstance().m_lineElements[elementTag]->getSection(segmentNo)->getA().value();
        
}

double BuildingModelerAPI::getMomentOfInertiaYY(int elementTag, int segmentNo)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return physicalModel::Building::getInstance().m_lineElements[elementTag]->getSection(segmentNo)->getIyy().value();  
}

double BuildingModelerAPI::getMomentOfInertiaZZ(int elementTag, int segmentNo)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return physicalModel::Building::getInstance().m_lineElements[elementTag]->getSection(segmentNo)->getIzz().value();
}

double BuildingModelerAPI::getTorsionalConstant(int elementTag, int segmentNo)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return physicalModel::Building::getInstance().m_lineElements[elementTag]->getSection(segmentNo)->getJ().value();
}

void BuildingModelerAPI::addShearWall(int elementTag, std::vector<int> jointTags, int sectionTag,
    physicalModel::AreaElementFormulation areaElementFormulation)
{
    if (areaElementExists(elementTag)) {
        throw EntityFoundException("Area element with tag " + std::to_string(elementTag) + " already exists.");
    }

    if (jointTags.size() != 4) {
        throw InvalidInputException("Number of joints must be four to create an area element with tag " + std::to_string(elementTag) + ".");
    }

    if (!jointExists(jointTags[0]) || !jointExists(jointTags[1]) || !jointExists(jointTags[2]) || !jointExists(jointTags[3])) {
        throw InvalidInputException("Joints do not exists to create area element with tag " + std::to_string(elementTag) + " .");
    }

    if (!sectionExists(sectionTag)) {
        throw EntityNotFoundException("Section with tag " + std::to_string(sectionTag) + " does not exist.");
    }

    std::vector<utility::Vector3> joints;
    joints.push_back(physicalModel::Building::getInstance().getJoint(jointTags[0])->getCoords());
    joints.push_back(physicalModel::Building::getInstance().getJoint(jointTags[1])->getCoords());
    joints.push_back(physicalModel::Building::getInstance().getJoint(jointTags[2])->getCoords());
    joints.push_back(physicalModel::Building::getInstance().getJoint(jointTags[3])->getCoords());

    if (!checkIfJointsCoplanar(joints)) {
        throw InvalidOperationException("Joints of area element with tag " + std::to_string(elementTag) + " are not coplanar.");
    }

    if (!checkIfJointsCounterClockwise(joints)) {
        throw InvalidOperationException("Joints of area element with tag " + std::to_string(elementTag) + " are not in counter-clockwise direction.");
    }

    if (!checkIfQuadConvex(joints)) {
        throw InvalidOperationException("Joints of area element with tag " + std::to_string(elementTag) + " do not form convex geometry.");
    }

    if (!checkIfShearWallVertical(joints)) {
        throw InvalidOperationException("Joints of area element with tag " + std::to_string(elementTag) + " do not form vertical shear wall.");
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
        throw EntityFoundException("Area element with tag " + std::to_string(elementTag) + " already exists.");
    }

    if (jointTags.size() != 4) {
        throw InvalidInputException("Number of joints must be four to create an area element with tag " + std::to_string(elementTag) + ".");
    }

    if (!jointExists(jointTags[0]) || !jointExists(jointTags[1]) || !jointExists(jointTags[2]) || !jointExists(jointTags[3])) {
        throw InvalidInputException("Joints do not exists to create area element with tag " + std::to_string(elementTag) + " .");
    }

    if (!sectionExists(sectionTag)) {
        throw EntityNotFoundException("Section with tag " + std::to_string(sectionTag) + " does not exist.");
    }

    auto floorI = physicalModel::Building::getInstance().getJoint(jointTags[0])->getFloorNo();
    auto floorJ = physicalModel::Building::getInstance().getJoint(jointTags[1])->getFloorNo();
    auto floorK = physicalModel::Building::getInstance().getJoint(jointTags[2])->getFloorNo();
    auto floorL = physicalModel::Building::getInstance().getJoint(jointTags[3])->getFloorNo();

    if (floorI != floorJ || floorJ != floorK || floorK != floorL) {
        throw InvalidInputException("Joints do not belong to same floor.");
    }

    std::vector<utility::Vector3> joints;
    joints.push_back(physicalModel::Building::getInstance().getJoint(jointTags[0])->getCoords());
    joints.push_back(physicalModel::Building::getInstance().getJoint(jointTags[1])->getCoords());
    joints.push_back(physicalModel::Building::getInstance().getJoint(jointTags[2])->getCoords());
    joints.push_back(physicalModel::Building::getInstance().getJoint(jointTags[3])->getCoords());

    if (!checkIfJointsCoplanar(joints)) {
        throw InvalidOperationException("Joints of area element with tag " + std::to_string(elementTag) + " are not coplanar.");
    }

    if (!checkIfJointsCounterClockwise(joints)) {
        throw InvalidOperationException("Joints of area element with tag " + std::to_string(elementTag) + " are not in counter-clockwise direction.");
    }

    if (!checkIfQuadConvex(joints)) {
        throw InvalidOperationException("Joints of area element with tag " + std::to_string(elementTag) + " do not form convex geometry.");
    }

    std::shared_ptr<physicalModel::Section> section = physicalModel::Building::getInstance().m_sections[sectionTag];
    physicalModel::Building::getInstance().m_areaElements[elementTag] = std::make_unique<physicalModel::SlabElement>(elementTag, jointTags, section, areaElementFormulation);
    physicalModel::Building::getInstance().m_joints[jointTags[0]]->addConnectedSlab(elementTag);
    physicalModel::Building::getInstance().m_joints[jointTags[1]]->addConnectedSlab(elementTag);
    physicalModel::Building::getInstance().m_joints[jointTags[2]]->addConnectedSlab(elementTag);
    physicalModel::Building::getInstance().m_joints[jointTags[3]]->addConnectedSlab(elementTag);
}

void BuildingModelerAPI::meshAreaElement(int elementTag, std::optional<int> n1, std::optional<int> n2)
{
    if (!areaElementExists(elementTag)) {
        throw EntityNotFoundException("Area element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    auto areaElement = physicalModel::Building::getInstance().getAreaElement(elementTag);
    auto surroundingLineElementTags = areaElement->getSurroundingLineElementTags();
    auto surroundingShearWallTags = areaElement->getSurroundingShearWallTags();
    std::vector<int> surroundingElementDivisions(4, -1);
    for (size_t i = 0; i < surroundingLineElementTags.size(); ++i) {

        if (surroundingLineElementTags[i] != -1) {
            auto element = physicalModel::Building::getInstance().getLineElement(surroundingLineElementTags[i]);
            if (element == nullptr) {
                throw EntityNotFoundException("Surrounding line elements with tag " + std::to_string(surroundingLineElementTags[i]) + " does not exist.");
            }
            surroundingElementDivisions[i] = element->getSegmentLengths().size();
        }
        else if (surroundingShearWallTags[i] != -1) {
            auto element = physicalModel::Building::getInstance().getAreaElement(surroundingShearWallTags[i]);
            if (element == nullptr) {
                throw EntityNotFoundException("Surrounding shear wall with tag " + std::to_string(surroundingShearWallTags[i]) + " does not exist.");
            }
            surroundingElementDivisions[i] = element->getMeshDivisions().first;
        }
    }

    if ((surroundingElementDivisions[0] != -1 || surroundingElementDivisions[2] != -1) && n1 != std::nullopt) {
        throw InvalidInputException("n1 cannot be assigned while there are surrounding elements in that direction.");
    }

    if ((surroundingElementDivisions[1] != -1 || surroundingElementDivisions[3] != -1) && n2 != std::nullopt) {
        throw InvalidInputException("n2 cannot be assigned while there are surrounding elements in that direction.");
    }
    
    if ((surroundingElementDivisions[0] * surroundingElementDivisions[2] < 0) && n1 == std::nullopt)
    {
        throw InvalidInputException("There must be surrounding elements on the opposite sides when n1 is not assigned.");
    }
    
    if ((surroundingElementDivisions[1] * surroundingElementDivisions[3] < 0) && n2 == std::nullopt)
    {
        throw InvalidInputException("There must be surrounding elements on the opposite sides when n2 is not assigned.");
    }

    bool meshIJ = true;
    if (n1 == std::nullopt)
    {
        if (surroundingElementDivisions[0] < 0 || surroundingElementDivisions[2] < 0) {
            throw InvalidInputException("There must be surrounding elements on the opposite sides when n1 is not assigned.");
        }

        if (surroundingElementDivisions[0] != surroundingElementDivisions[2]) {
            throw InvalidInputException("Surrounding elements on the opposite sides must have the same number of elements.");
        }

        if (surroundingElementDivisions[0] == 1) {
            meshIJ = false;
        }
    }
    else if (n1.value() < 1)
    {
        throw InvalidInputException("n1 value cannot be less than 1.");
    }

    bool meshJK = true;
    if (n2 == std::nullopt)
    {
        if (surroundingElementDivisions[1] < 0 || surroundingElementDivisions[3] < 0) {
            throw InvalidInputException("There must be surrounding elements on the opposite sides when n2 is not assigned.");
        }

        if (surroundingElementDivisions[1] != surroundingElementDivisions[3]) {
            throw InvalidInputException("Surrounding elements on the opposite sides must have the same number of elements.");
        }
        if (surroundingElementDivisions[1] == 1) {
            meshJK = false;
        }
    }
    else if (n2.value() < 1)
    {
        throw InvalidInputException("n2 value cannot be less than 1.");
    }

    if (!meshIJ && !meshJK) {
        areaElement->setMeshable(false);
        return;
    }

    if (n1.has_value() && n2.has_value()) {
        if (n1.value() == 1 && n2.value() == 1) {
            areaElement->setMeshable(false);
            return;
        }

        areaElement->mesh(true, n1.value(), n2.value());
    }
    else if (n1.has_value()) {
        areaElement->mesh(true, n1.value(), -1);
    }
    else if (n2.has_value()) {
        areaElement->mesh(true, -1, n2.value());
    }
    else {
        areaElement->mesh(true);
    }
}

void BuildingModelerAPI::disableMeshForAreaElement(int elementTag)
{
    if (!areaElementExists(elementTag)) {
        throw EntityNotFoundException("Area element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    auto areaElement = physicalModel::Building::getInstance().getAreaElement(elementTag);
    areaElement->setMeshable(false);
}

void BuildingModelerAPI::disableSlabElements(bool disableSlabElements)
{
    physicalModel::Building::getInstance().m_disableSlabElements = disableSlabElements;
}

const std::vector<int>& BuildingModelerAPI::getAreaElementJointTags(int elementTag)
{
    if (!areaElementExists(elementTag)) {
        throw EntityNotFoundException("Area element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return physicalModel::Building::getInstance().getAreaElement(elementTag)->getJointTags();
}

const std::vector<std::vector<utility::Vector3>>& BuildingModelerAPI::getNodeCoordinatesOfAreaElement(int elementTag)
{
    if (!areaElementExists(elementTag)) {
        throw EntityNotFoundException("Area element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return physicalModel::Building::getInstance().getAreaElement(elementTag)->getAnalyticalNodeCoords();
}

double BuildingModelerAPI::getSurfaceArea(int elementTag)
{
    if (!areaElementExists(elementTag)) {
        throw EntityNotFoundException("Area element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return physicalModel::Building::getInstance().m_areaElements[elementTag]->getArea();
}

std::vector<int> BuildingModelerAPI::getShearWallElementTags()
{
    std::vector<int> elementTags;

    auto it = physicalModel::Building::getInstance().m_areaElements.begin();

    for (; it != physicalModel::Building::getInstance().m_areaElements.end(); it++) {

        if (physicalModel::AreaElementType::SHEARWALL == it->second.get()->getAreaElementType()) {
            elementTags.push_back(it->first);
        }
    }

    return elementTags;
}

std::vector<int> BuildingModelerAPI::getSlabElementTags()
{
    std::vector<int> elementTags;

    auto it = physicalModel::Building::getInstance().m_areaElements.begin();

    for (; it != physicalModel::Building::getInstance().m_areaElements.end(); it++) {

        if (physicalModel::AreaElementType::SLAB == it->second.get()->getAreaElementType()) {
            elementTags.push_back(it->first);
        }
    }

    return elementTags;
}

void BuildingModelerAPI::addFloor(int floorNumber, double height)
{
    if (floorNumber < 0) {
        throw InvalidInputException("Floor number cannot be less than zero.");
    }

    if (floorExists(floorNumber)) {
        throw EntityFoundException("Floor number " + std::to_string(floorNumber) + " already exists.");
    }
    
    if (floorNumber == 0) {
        physicalModel::Building::getInstance().m_floors[floorNumber] = std::make_unique<physicalModel::Floor>(floorNumber);
    }
    else {
        physicalModel::Building::getInstance().m_floors[floorNumber] = std::make_unique<physicalModel::Floor>(floorNumber, height);
    }

    // To do: do something about floor number and height inconssitency. Higher floor numbers should not be below lower floor numbers.
}

void BuildingModelerAPI::makeRigid(int floorNumber, int masterJointTag)
{
    if (jointExists(masterJointTag)) {
        throw EntityFoundException("Master joint with tag " + std::to_string(masterJointTag) + " for the floor " + std::to_string(floorNumber)
            + " already exists. Please make floor flexible than try making rigid with a specified master joint tag again.");
    }

    if (!floorExists(floorNumber)) {
        throw EntityNotFoundException("Floor number " + std::to_string(floorNumber) + " does not exist.");
    }

    auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);
    if (floor->isRigid()) {
        throw InvalidOperationException("Floor " + std::to_string(floorNumber) + " is already rigid.");
    }

    if (floor->getMassCenterJointTag() != -1) {
        throw InvalidOperationException("Master joint tag for floor " + std::to_string(floorNumber) + "is already assigned.");
    }
    
    auto massCenter = floor->getMassCenter();
    std::optional<utility::Vector3> mass;
    if (massCenter == std::nullopt)
    {
        if (!floor->updateMassCenter()) {
            throw InvalidOperationException("Floor " + std::to_string(floorNumber) + " has no mass.");
        }
        massCenter = floor->getMassCenter();
        mass = floor->getDiaphragmMass();
    }

    utility::Vector3 coords;
    coords.x = massCenter.value().x;
    coords.y = massCenter.value().y;
    coords.z = floor->getFloorHeight();

    physicalModel::Building::getInstance().m_joints[masterJointTag] = std::make_unique<physicalModel::Joint>(masterJointTag, coords);

    auto joint = physicalModel::Building::getInstance().getJoint(masterJointTag);
    joint->setFloorNo(floorNumber);
    joint->setConstraintVector({0, 0, 1, 1, 1, 0});

    if (mass != std::nullopt) {
        utility::Vector3 tranlationalMass{ mass.value().x, mass.value().y, 0 };
        joint->addTranslationalMass(tranlationalMass);

        if (mass.value().z > 1e-10 || mass.value().z < -1e-10) {
            utility::Vector3 rotationalMass{0, 0, mass.value().z };
            joint->addRotationalMass(rotationalMass);
        }
    }

    physicalModel::Building::getInstance().m_floors[floorNumber]->makeRigid(masterJointTag);
}

void BuildingModelerAPI::makeFlexible(int floorNumber)
{
    if (!floorExists(floorNumber)) {
        throw EntityNotFoundException("Floor number " + std::to_string(floorNumber) + " does not exist.");
    }
    else if (!physicalModel::Building::getInstance().m_floors[floorNumber]->isRigid()) {
        throw InvalidOperationException("Floor number " + std::to_string(floorNumber) + " is already flexible.");
    }
    else {
        auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);
        auto masterJointTag = floor->getMassCenterJointTag();
        floor->makeFlexible();
        floor->confineFloorMassOnDiaphragmNode(false);
        physicalModel::Building::getInstance().deleteJoint(masterJointTag);
    }
}

void BuildingModelerAPI::confineFloorMassOnDiaphragmNode(int floorNumber, bool confineFloorMassOnDiaphragmNode)
{
    if (!floorExists(floorNumber)) {
        throw EntityNotFoundException("Floor number " + std::to_string(floorNumber) + " does not exist.");
    }

    auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);
    if (!floor->isRigid() && confineFloorMassOnDiaphragmNode) {
        throw InvalidOperationException("Floor mass can be confined into diaphragm node only in rigid floors. Floor " 
            + std::to_string(floorNumber) + " is not rigid.");
    }

    floor->confineFloorMassOnDiaphragmNode(confineFloorMassOnDiaphragmNode);
}

double BuildingModelerAPI::getFloorMass(int floorNumber)
{
    if (!floorExists(floorNumber)) {
        throw EntityNotFoundException("Floor number " + std::to_string(floorNumber) + " does not exist.");
    }

    auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);

    return floor->getFloorMass();
}

std::optional<utility::Vector3> BuildingModelerAPI::getDiaphragmMass(int floorNumber)
{
    if (!floorExists(floorNumber)) {
        throw EntityNotFoundException("Floor number " + std::to_string(floorNumber) + " does not exist.");
    }

    auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);

    return floor->getDiaphragmMass();
}

std::optional<utility::Vector2> BuildingModelerAPI::getMassCenter(int floorNumber)
{
    if (!floorExists(floorNumber)) {
        throw EntityNotFoundException("Floor number " + std::to_string(floorNumber) + " does not exist.");
    }

    auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);

    return floor->getMassCenter();
}

double BuildingModelerAPI::getFloorHeight(int floorNumber)
{
    if (!floorExists(floorNumber)) {
        throw EntityNotFoundException("Floor number " + std::to_string(floorNumber) + " does not exist.");
    }

    auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);

    return floor->getFloorHeight();
}

const std::vector<int>& BuildingModelerAPI::getFloorJointTags(int floorNumber)
{
    if (!floorExists(floorNumber)) {
        throw EntityNotFoundException("Floor number " + std::to_string(floorNumber) + " does not exist.");
    }

    auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);

    return floor->getJoints();
}

int BuildingModelerAPI::getMasterJointTag(int floorNumber)
{
    if (!floorExists(floorNumber)) {
        throw EntityNotFoundException("Floor number " + std::to_string(floorNumber) + " does not exist.");
    }

    auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);

    return floor->getMassCenterJointTag();
}

std::set<int> BuildingModelerAPI::getBeamTags(int floorNumber)
{
    if (!floorExists(floorNumber)) {
        throw EntityNotFoundException("Floor number " + std::to_string(floorNumber) + " does not exist.");
    }

    std::set<int> beamTags;

    auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);
    auto joints = floor->getJoints();
    for (int i = 0; i < joints.size(); ++i) {

        auto joint = physicalModel::Building::getInstance().getJoint(joints[i]);
        auto tags = joint->getConnectedBeamTags();

        for (auto beamTag : tags) {
            beamTags.insert(beamTag);
        }
    }

    return beamTags;
}

std::vector<int> BuildingModelerAPI::getColumnTags(int floorNumber)
{
    if (!floorExists(floorNumber)) {
        throw EntityNotFoundException("Floor number " + std::to_string(floorNumber) + " does not exist.");
    }

    std::vector<int> columnTags;

    auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);
    auto joints = floor->getJoints();
    for (int i = 0; i < joints.size(); ++i) {

        auto joint = physicalModel::Building::getInstance().getJoint(joints[i]);
        auto tags = joint->getConnectedColumnTags();

        for (auto columnTag : tags) {
            
            auto columnJointTags = getLineElementJointTags(columnTag);
            if (joints[i] == columnJointTags[1]) {

                if (floorNumber - 1 == physicalModel::Building::getInstance().m_joints[columnJointTags[0]]->getFloorNo()) {

                    columnTags.push_back(columnTag);
                }
            }
            else {

                if (floorNumber - 1 == physicalModel::Building::getInstance().m_joints[columnJointTags[1]]->getFloorNo()) {

                    columnTags.push_back(columnTag);
                }
            }

        }
    }

    return columnTags;
}

std::set<int> BuildingModelerAPI::getShearWallTagsInXDir(int floorNumber)
{
    if (!floorExists(floorNumber)) {
        throw EntityNotFoundException("Floor number " + std::to_string(floorNumber) + " does not exist.");
    }

    std::set<int> shearWallTags;

    auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);
    auto joints = floor->getJoints();
    for (int i = 0; i < joints.size(); ++i) {

        auto joint = physicalModel::Building::getInstance().getJoint(joints[i]);
        auto tags = joint->getConnectedWallTags();

        for (auto wallTag : tags) {
            
            auto wallJointTags = getAreaElementJointTags(wallTag);
            if (joints[i] == wallJointTags[2] || joints[i] == wallJointTags[3]) {

                if (floorNumber - 1 == physicalModel::Building::getInstance().m_joints[wallJointTags[1]]->getFloorNo()) {

                    if (getCoordinates(wallJointTags[1]).x - getCoordinates(wallJointTags[0]).x > 1e-10) {

                        shearWallTags.insert(wallTag);
                    }
                }
            }
        }
    }

    return shearWallTags;
}

void BuildingModelerAPI::includePDeltaEffects(bool includePDeltaEffects)
{
    physicalModel::Building::getInstance().m_includePDeltaEffects = includePDeltaEffects;
}

double BuildingModelerAPI::getBuildingWeight()
{
    return physicalModel::Building::getInstance().getTotalWeight();
}

void BuildingModelerAPI::clear()
{
    opensees::OpenseesModel::getInstance().clear();
    physicalModel::Building::getInstance().clear();
}

void BuildingModelerAPI::clearAnalyticalModel()
{
    opensees::OpenseesModel::getInstance().clear();

    for (auto it = physicalModel::Building::getInstance().m_lineElements.begin(); it != physicalModel::Building::getInstance().m_lineElements.end(); it++) {
        it->second->resetAnalyticalProperties();
    }
    
    for (auto it = physicalModel::Building::getInstance().m_areaElements.begin(); it != physicalModel::Building::getInstance().m_areaElements.end(); it++) {
        it->second->resetAnalyticalProperties();
    }
}

void BuildingModelerAPI::includeDeadLoadFromMembers(bool includeDeadLoadFromMembers)
{
    physicalModel::Building::getInstance().m_includeDeadLoadFromMembers = includeDeadLoadFromMembers;
}

void BuildingModelerAPI::applyGravityLoadThroughLineElements(bool gravityThroughLineElements)
{
    physicalModel::Building::getInstance().m_gravityThroughLineElements = gravityThroughLineElements;
}

void BuildingModelerAPI::setLiveLoadForFloor(int floorNumber, double liveLoadPerArea)
{
    if (!floorExists(floorNumber)) {
        throw EntityNotFoundException("Floor number " + std::to_string(floorNumber) + " does not exist.");
    }

    auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);

    floor->setLiveLoadPerArea(liveLoadPerArea);
}

void BuildingModelerAPI::setLiveLoadMassContributionForFloor(int floorNumber, double liveLoadMassContributionFactor)
{
    if (!floorExists(floorNumber)) {
        throw EntityNotFoundException("Floor number " + std::to_string(floorNumber) + " does not exist.");
    }

    auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);

    floor->setLiveLoadMassContributionFactor(liveLoadMassContributionFactor);
}

void BuildingModelerAPI::addStaticLoadCase(std::string loadCaseTag, physicalModel::StaticLoadCaseType loadCaseType)
{
    if (loadCaseExists(loadCaseTag)) {
        throw EntityFoundException("Load case: " + loadCaseTag + " already exists.");
    }

    if (staticLoadCombinationExists(loadCaseTag)) {
        throw EntityFoundException("Static load combination: " + loadCaseTag + " cannot have the same name with Load case: " + loadCaseTag);
    }

    physicalModel::Building::getInstance().m_loadCases[loadCaseTag] = std::make_shared<physicalModel::StaticLoadCase>(loadCaseTag, loadCaseType);
}

void BuildingModelerAPI::addModalLoadCase(std::string loadCaseTag, size_t numberOfModes)
{
    if (loadCaseExists(loadCaseTag)) {
        throw EntityFoundException("Load case: " + loadCaseTag + " already exists.");
    }

    if (staticLoadCombinationExists(loadCaseTag)) {
        throw EntityFoundException("Static load combination: " + loadCaseTag + " cannot have the same name with Load case: " + loadCaseTag);
    }

    physicalModel::Building::getInstance().m_loadCases[loadCaseTag] = std::make_shared<physicalModel::ModalLoadCase>(loadCaseTag, numberOfModes);
}

void BuildingModelerAPI::addPointLoad(std::string loadCaseTag, int jointTag, double fx, double fy, double fz, double mx, double my, double mz)
{
    if (!loadCaseExists(loadCaseTag)) {
        throw EntityNotFoundException("Load case: " + loadCaseTag + " does not exist.");
    }

    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }

    auto loadCase = physicalModel::Building::getInstance().m_loadCases[loadCaseTag];
    if (auto staticLoadCase = std::dynamic_pointer_cast<physicalModel::StaticLoadCase>(loadCase)) {
        std::shared_ptr<physicalModel::Load> load = std::make_shared<physicalModel::PointLoad>(jointTag, fx, fy, fz, mx, my, mz);

        physicalModel::Building::getInstance().m_pointLoads[load->getUniqueID()] = load;

        staticLoadCase->addPointLoad(load);
    }
    else {
        throw InvalidInputException("Load case with tag " + loadCaseTag + " is not a static load case.Point loads can be added only to static load cases!");
    }
}

void BuildingModelerAPI::addDistributedLineLoad(std::string loadCaseTag, int elementTag, double wz, double wy, double wx)
{
    if (!loadCaseExists(loadCaseTag)) {
        throw EntityNotFoundException("Load case: " + loadCaseTag + " does not exist.");
    }

    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    auto loadCase = physicalModel::Building::getInstance().m_loadCases[loadCaseTag];
    if (auto staticLoadCase = std::dynamic_pointer_cast<physicalModel::StaticLoadCase>(loadCase)) {
        std::shared_ptr<physicalModel::Load> load = std::make_shared<physicalModel::DistributedLineLoad>(elementTag, wx, wy, wz);

        physicalModel::Building::getInstance().m_distributedLineLoads[load->getUniqueID()] = load;

        staticLoadCase->addDistributedLineLoad(load);
    }
    else {
        throw InvalidInputException("Load case with tag " + loadCaseTag + " is not a static load case.Distributed line loads can be added only to static load cases!");
    }
}

void BuildingModelerAPI::addDistributedAreaLoad(std::string loadCaseTag, int elementTag, double wz, double wy, double wx)
{
    if (!loadCaseExists(loadCaseTag)) {
        throw EntityNotFoundException("Load case: " + loadCaseTag + " does not exist.");
    }

    if (!areaElementExists(elementTag)) {
        throw EntityNotFoundException("Area element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    auto loadCase = physicalModel::Building::getInstance().m_loadCases[loadCaseTag];
    if (auto staticLoadCase = std::dynamic_pointer_cast<physicalModel::StaticLoadCase>(loadCase)) {
        std::shared_ptr<physicalModel::Load> load = std::make_shared<physicalModel::DistributedAreaLoad>(elementTag, wx, wy, wz);

        physicalModel::Building::getInstance().m_distributedAreaLoads[load->getUniqueID()] = load;

        staticLoadCase->addDistributedAreaLoad(load);
    }
    else {
        throw InvalidInputException("Load case with tag " + loadCaseTag + " is not a static load case.Distributed area loads can be added only to static load cases!");
    }
}

void BuildingModelerAPI::addStaticLoadCombination(std::string staticLoadCombinationTag)
{
    if (staticLoadCombinationExists(staticLoadCombinationTag)) {
        throw EntityFoundException("Static load combination: " + staticLoadCombinationTag + " already exists.");
    }

    if (loadCaseExists(staticLoadCombinationTag)) {
        throw EntityFoundException("Static load combination: " + staticLoadCombinationTag + " cannot have the same name with Load case: " + staticLoadCombinationTag);
    }

    physicalModel::Building::getInstance().m_staticLoadCombinations[staticLoadCombinationTag] = std::make_shared<physicalModel::StaticLoadCombination>(staticLoadCombinationTag);
}

void BuildingModelerAPI::addLoadCaseToStaticLoadCombination(std::string staticLoadCombinationTag, std::string loadCaseTag, double factor)
{
    if (!staticLoadCombinationExists(staticLoadCombinationTag)) {
        throw EntityNotFoundException("Static load combination: " + staticLoadCombinationTag + " does not exist.");
    }

    if (!loadCaseExists(loadCaseTag)) {
        throw EntityNotFoundException("Load case: " + loadCaseTag + " does not exist.");
    }

    auto loadCase = physicalModel::Building::getInstance().getLoadCase(loadCaseTag);

    if (loadCase->getLoadCaseType() == physicalModel::LoadCaseType::STATIC) {
        physicalModel::Building::getInstance().m_staticLoadCombinations[staticLoadCombinationTag]->addLoadCase(loadCase, factor);
    }
    else {
        throw InvalidInputException("Load case with tag " + loadCaseTag + " is not a static load case.Only static load cases can be added to static load combinations!");
    }
}

void BuildingModelerAPI::setLoadCaseActive(std::string loadCaseTag, bool active)
{
    if (!loadCaseExists(loadCaseTag)) {
        throw EntityNotFoundException("Load case: " + loadCaseTag + " does not exist.");
    }

    physicalModel::Building::getInstance().m_loadCases[loadCaseTag]->setActive(active);
}

void BuildingModelerAPI::setStaticLoadCombinationActive(std::string staticLoadCombinationTag, bool active)
{
    if (!staticLoadCombinationExists(staticLoadCombinationTag)) {
        throw EntityNotFoundException("Static load combination: " + staticLoadCombinationTag + " does not exist.");
    }

    physicalModel::Building::getInstance().m_staticLoadCombinations[staticLoadCombinationTag]->setActive(active);
}

const std::vector<std::shared_ptr<physicalModel::Load>>& BuildingModelerAPI::getPointLoads(std::string loadCaseTag)
{
    if (!loadCaseExists(loadCaseTag)) {
        throw EntityNotFoundException("Load case: " + loadCaseTag + " does not exist.");
    }

    auto loadCase = physicalModel::Building::getInstance().m_loadCases[loadCaseTag];
    if (auto staticLoadCase = std::dynamic_pointer_cast<physicalModel::StaticLoadCase>(loadCase)) {
        return staticLoadCase->getPointLoads();
    }
    else {
        throw InvalidInputException("Load case with tag " + loadCaseTag + " is not a static load case.Only static load cases can have point loads!");
    }
}

const std::vector<std::shared_ptr<physicalModel::Load>>& BuildingModelerAPI::getDistributedLineLoads(std::string loadCaseTag)
{
    if (!loadCaseExists(loadCaseTag)) {
        throw EntityNotFoundException("Load case: " + loadCaseTag + " does not exist.");
    }

    auto loadCase = physicalModel::Building::getInstance().m_loadCases[loadCaseTag];
    if (auto staticLoadCase = std::dynamic_pointer_cast<physicalModel::StaticLoadCase>(loadCase)) {
        return staticLoadCase->getDistributedLineLoads();
    }
    else {
        throw InvalidInputException("Load case with tag " + loadCaseTag + " is not a static load case.Only static load cases can have distributed line loads!");
    }
}

const std::vector<std::shared_ptr<physicalModel::Load>>& BuildingModelerAPI::getDistributedAreaLoads(std::string loadCaseTag)
{
    if (!loadCaseExists(loadCaseTag)) {
        throw EntityNotFoundException("Load case: " + loadCaseTag + " does not exist.");
    }

    auto loadCase = physicalModel::Building::getInstance().m_loadCases[loadCaseTag];
    if (auto staticLoadCase = std::dynamic_pointer_cast<physicalModel::StaticLoadCase>(loadCase)) {
        return staticLoadCase->getDistributedAreaLoads();
    }
    else {
        throw InvalidInputException("Load case with tag " + loadCaseTag + " is not a static load case.Only static load cases can have distributed area loads!");
    }
}

const std::vector<std::shared_ptr<opensees::Load>>& BuildingModelerAPI::getLoadsFromAnalyticalModel(std::string loadPatternTag)
{
    if (!loadCaseExists(loadPatternTag) && !staticLoadCombinationExists(loadPatternTag)) {
        throw EntityNotFoundException("Load pattern: " + loadPatternTag + " does not exist.");
    }

    return  opensees::OpenseesModel::getInstance().getLoadPattern(loadPatternTag)->getLoadVector();
}

void BuildingModelerAPI::updateMassSourceFromMembers()
{
    if (physicalModel::Building::getInstance().m_includeMassFromMembers) {
        physicalModel::Building::getInstance().addMemberMasses();
    }
}

void BuildingModelerAPI::updateAreaElementProperties()
{
    physicalModel::Building::getInstance().updateSurroundingLineElements();
}

void BuildingModelerAPI::updateDeadAndLiveLoads()
{
    if (!physicalModel::Building::getInstance().m_gravityThroughLineElements && physicalModel::Building::getInstance().m_disableSlabElements) {
        throw InvalidOperationException("Self weight cannot be transferred through slab elements while they are disabled. Either enable slab elements or transfer the load through line elements");
    }

    // Update dead loads
    if (physicalModel::Building::getInstance().m_includeDeadLoadFromMembers) {

        // dead load
        auto loadCaseTag = "dead";
        auto staticLoadCaseType = physicalModel::StaticLoadCaseType::DEAD;
        if (physicalModel::Building::getInstance().m_loadCases.find(loadCaseTag) == physicalModel::Building::getInstance().m_loadCases.end()) {
            physicalModel::Building::getInstance().m_loadCases[loadCaseTag] = std::make_shared<physicalModel::StaticLoadCase>(loadCaseTag, staticLoadCaseType);
        }
        else {
            physicalModel::Building::getInstance().m_loadCases[loadCaseTag]->clear();
        }

        auto staticLoadCase = std::dynamic_pointer_cast<physicalModel::StaticLoadCase>(physicalModel::Building::getInstance().m_loadCases[loadCaseTag]);
        
        // line elements
        for (auto it = physicalModel::Building::getInstance().m_lineElements.begin(); it != physicalModel::Building::getInstance().m_lineElements.end(); it++) {

            auto elementTag = it->second->getElementTag();
            auto gamma = it->second->getWeight() / it->second->getLength();
            std::shared_ptr<physicalModel::Load> load;

            if (it->second->getLineElementType() == physicalModel::LineElementType::COLUMN) {
                load = std::make_shared<physicalModel::DistributedLineLoad>(elementTag, 0, 0, -gamma);
            }
            else {
                load  = std::make_shared<physicalModel::DistributedLineLoad>(elementTag, 0, -gamma, 0);
            }

            physicalModel::Building::getInstance().m_distributedLineLoads[load->getUniqueID()] = load;

            staticLoadCase->addDistributedLineLoad(load);
        }

        // area elements
        for (auto it = physicalModel::Building::getInstance().m_areaElements.begin(); it != physicalModel::Building::getInstance().m_areaElements.end(); it++) {

            if (it->second->getAreaElementType() == physicalModel::AreaElementType::SLAB && physicalModel::Building::getInstance().m_gravityThroughLineElements) {

                auto beamTags = it->second->getSurroundingLineElementTags();
                auto shearWallTags = it->second->getSurroundingShearWallTags();
                auto gamma = it->second->getWeight() / it->second->getArea();
                auto lineLength = it->second->getTributaryLineLength();

                for (int i = 0; i < beamTags.size(); ++i) {

                    if (beamTags[i] > -1) {

                        std::shared_ptr<physicalModel::Load> load;

                        if (i % 2 == 0) {
                            load = std::make_shared<physicalModel::DistributedLineLoad>(beamTags[i], 0, -gamma * lineLength.first, 0);
                        }
                        else {
                            load = std::make_shared<physicalModel::DistributedLineLoad>(beamTags[i], 0, -gamma * lineLength.second, 0);
                        }

                        physicalModel::Building::getInstance().m_distributedLineLoads[load->getUniqueID()] = load;

                        staticLoadCase->addDistributedLineLoad(load);
                    }
                    else {
                        std::shared_ptr<physicalModel::Load> loadI;
                        std::shared_ptr<physicalModel::Load> loadJ;
                        double magnitude;

                        if (i % 2 == 0) {
                            magnitude = -gamma * lineLength.first * it->second->getEdgeLength(i) / 2.0;
                        }
                        else {
                            magnitude = -gamma * lineLength.second * it->second->getEdgeLength(i) / 2.0;
                        }

                        size_t jointI = i;
                        size_t jointJ = i == 3 ? 0 : i + 1;

                        loadI = std::make_shared<physicalModel::PointLoad>(it->second->getJointTags()[jointI], 0.0, 0.0, magnitude);
                        loadJ = std::make_shared<physicalModel::PointLoad>(it->second->getJointTags()[jointJ], 0.0, 0.0, magnitude);

                        physicalModel::Building::getInstance().m_distributedLineLoads[loadI->getUniqueID()] = loadI;
                        physicalModel::Building::getInstance().m_distributedLineLoads[loadJ->getUniqueID()] = loadJ;

                        staticLoadCase->addPointLoad(loadI);
                        staticLoadCase->addPointLoad(loadJ);
                    }
                }
            }
            else {
                auto elementTag = it->second->getElementTag();
                auto gamma = it->second->getWeight() / it->second->getArea();
                std::shared_ptr<physicalModel::Load> load = std::make_shared<physicalModel::DistributedAreaLoad>(elementTag, 0, -gamma, 0);

                physicalModel::Building::getInstance().m_distributedAreaLoads[load->getUniqueID()] = load;

                staticLoadCase->addDistributedAreaLoad(load);
            }
        }
    }

    // Update live loads
    auto loadCaseTag = "live";
    auto staticLoadCaseType = physicalModel::StaticLoadCaseType::LIVE;
    if (physicalModel::Building::getInstance().m_loadCases.find(loadCaseTag) == physicalModel::Building::getInstance().m_loadCases.end()) {
        physicalModel::Building::getInstance().m_loadCases[loadCaseTag] = std::make_shared<physicalModel::StaticLoadCase>(loadCaseTag, staticLoadCaseType);
    }
    else {
        physicalModel::Building::getInstance().m_loadCases[loadCaseTag]->clear();
    }

    auto staticLoadCase = std::dynamic_pointer_cast<physicalModel::StaticLoadCase>(physicalModel::Building::getInstance().m_loadCases[loadCaseTag]);

    for (auto it = physicalModel::Building::getInstance().m_floors.begin(); it != physicalModel::Building::getInstance().m_floors.end(); it++) {

        auto liveLoad = it->second->getLiveLoadPerArea();
        if (liveLoad != std::nullopt) {

            std::unordered_set<int> slabTags;
            for (auto jointTag : it->second->getJoints()) {

                for (auto slabTag : physicalModel::Building::getInstance().getJoint(jointTag)->getConnectedSlabTags()) {
                    slabTags.insert(slabTag);
                }
            }

            for (auto slabTag : slabTags) {

                auto slab = physicalModel::Building::getInstance().getAreaElement(slabTag);

                if (physicalModel::Building::getInstance().m_gravityThroughLineElements) {

                    auto beamTags = slab->getSurroundingLineElementTags();
                    auto lineLength = slab->getTributaryLineLength();

                    for (int i = 0; i < beamTags.size(); ++i) {

                        if (beamTags[i] > -1) {

                            std::shared_ptr<physicalModel::Load> load;

                            if (i % 2 == 0) {
                                load = std::make_shared<physicalModel::DistributedLineLoad>(beamTags[i], 0, -liveLoad.value() * lineLength.first, 0);
                            }
                            else {
                                load = std::make_shared<physicalModel::DistributedLineLoad>(beamTags[i], 0, -liveLoad.value() * lineLength.second, 0);
                            }

                            physicalModel::Building::getInstance().m_distributedLineLoads[load->getUniqueID()] = load;

                            staticLoadCase->addDistributedLineLoad(load);
                        }
                        else {
                            std::shared_ptr<physicalModel::Load> loadI;
                            std::shared_ptr<physicalModel::Load> loadJ;
                            double magnitude;

                            if (i % 2 == 0) {
                                magnitude = -liveLoad.value() * lineLength.first * slab->getEdgeLength(i) / 2.0;
                            }
                            else {
                                magnitude = -liveLoad.value() * lineLength.second * slab->getEdgeLength(i) / 2.0;
                            }

                            size_t jointI = i;
                            size_t jointJ = i == 3 ? 0 : i + 1;

                            loadI = std::make_shared<physicalModel::PointLoad>(slab->getJointTags()[jointI], 0.0, 0.0, magnitude);
                            loadJ = std::make_shared<physicalModel::PointLoad>(slab->getJointTags()[jointJ], 0.0, 0.0, magnitude);

                            physicalModel::Building::getInstance().m_distributedLineLoads[loadI->getUniqueID()] = loadI;
                            physicalModel::Building::getInstance().m_distributedLineLoads[loadJ->getUniqueID()] = loadJ;

                            staticLoadCase->addPointLoad(loadI);
                            staticLoadCase->addPointLoad(loadJ);
                        }
                    }
                }
                else {
                    auto elementTag = slab->getElementTag();
                    std::shared_ptr<physicalModel::Load> load = std::make_shared<physicalModel::DistributedAreaLoad>(elementTag, 0, -liveLoad.value(), 0);

                    physicalModel::Building::getInstance().m_distributedAreaLoads[load->getUniqueID()] = load;

                    staticLoadCase->addDistributedAreaLoad(load);
                }
            }
        }
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

bool BuildingModelerAPI::loadCaseExists(std::string loadCaseTag)
{
    if (physicalModel::Building::getInstance().m_loadCases.find(loadCaseTag) != physicalModel::Building::getInstance().m_loadCases.end()) {
        return true;
    }

    return false;
}

bool BuildingModelerAPI::staticLoadCombinationExists(std::string loadCombinationTag)
{
    if (physicalModel::Building::getInstance().m_staticLoadCombinations.find(loadCombinationTag) != physicalModel::Building::getInstance().m_staticLoadCombinations.end()) {
        return true;
    }

    return false;
}

void BuildingModelerAPI::invalidateAreaMeshAlongLineElement(int elementTag)
{
    auto lineElement = physicalModel::Building::getInstance().getLineElement(elementTag);
    auto jointI = physicalModel::Building::getInstance().getJoint(lineElement->getIJointTag());
    auto jointJ = physicalModel::Building::getInstance().getJoint(lineElement->getJJointTag());

    for (const auto& tagI : jointI->getConnectedSlabTags()) {

        for (const auto& tagJ : jointJ->getConnectedSlabTags()) {
            if (tagI == tagJ) {
                disableMeshForAreaElement(tagI);
            }
        }
    }

    for (const auto& tagI : jointI->getConnectedWallTags()) {

        for (const auto& tagJ : jointJ->getConnectedWallTags()) {
            if (tagI == tagJ) {
                disableMeshForAreaElement(tagI);
            }
        }
    }
}

bool BuildingModelerAPI::checkIfJointsCoplanar(const std::vector<utility::Vector3>& joints)
{
    auto vec1 = joints[1] - joints[0];
    auto vec2 = joints[3] - joints[0];
    auto vec3 = joints[2] - joints[0];
    
    auto normal = utility::VectorUtilities::crossProduct(vec1, vec2);
    normal = normal / normal.norm2();
    auto eps = utility::VectorUtilities::dotProduct(normal, vec3);

    if (eps < 1e-6) {
        return true;
    }

    return false;
}

bool BuildingModelerAPI::checkIfJointsCounterClockwise(const std::vector<utility::Vector3>& joints)
{
    auto u = joints[1] - joints[0];
    auto uv = joints[3] - joints[0];

    auto w = utility::VectorUtilities::crossProduct(u, uv);
    u = u / u.norm2();
    w = w / w.norm2();
    auto v = utility::VectorUtilities::crossProduct(w, u);

    auto origin = joints[0];
    std::vector<utility::Vector2> joints2D;
    for (const auto& vec : joints) {
        joints2D.push_back(utility::VectorUtilities::projectVectorOn2DLocalBasis((vec - origin), u, v));
    }

    auto signedArea = 0.5 * (joints2D[3].x * joints2D[0].y - joints2D[0].x * joints2D[3].y);
    for (int i = 0; i < 3; ++i) {
        signedArea += (0.5 * (joints2D[i].x * joints2D[i + 1].y - joints2D[i + 1].x * joints2D[i].y));
    }

    if (signedArea > 0) {
        return true;
    }

    return false;
}

bool BuildingModelerAPI::checkIfQuadConvex(const std::vector<utility::Vector3>& joints)
{
    std::vector<utility::Vector3> vecs;

    vecs.push_back(joints[1] - joints[0]);
    vecs.push_back(joints[2] - joints[1]);
    vecs.push_back(joints[3] - joints[2]);
    vecs.push_back(joints[0] - joints[3]);

    std::vector<utility::Vector3> normals;
    for (int i = 0; i < 4; ++i) {
        auto startInd = i;
        auto endInd = (i == 3) ? 0 : i + 1;
        normals.push_back(utility::VectorUtilities::crossProduct(vecs[startInd], vecs[endInd]));
    }

    auto referenceNormal = utility::VectorUtilities::normalize(normals[0]);
    for (int i = 1; i < 4; ++i) {
        auto normalizedNormal = utility::VectorUtilities::normalize(normals[i]);
        auto dotProduct = utility::VectorUtilities::dotProduct(referenceNormal, normalizedNormal);

        if (dotProduct < 0) {
            return false;
        }
    }

    return true;
}

bool BuildingModelerAPI::checkIfShearWallVertical(const std::vector<utility::Vector3>& joints)
{
    // This function checks if shear wall vertical and node numbering for bottom left is 1
    // and it is either on X or Y plane not combined
    if (joints[0].z < joints[1].z - 1e-7 || joints[0].z > joints[1].z + 1e-7) {
        return false;
    }

    if (joints[0].z > joints[2].z - 1e-7) {
        return false;
    }

    if (joints[0].y > joints[1].y - 1e-7 && joints[0].y < joints[1].y + 1e-7) {

        if (joints[0].x > joints[1].x) {
            return false;
        }
    }
    else if (joints[0].x > joints[1].x - 1e-7 && joints[0].x < joints[1].x + 1e-7) {

        if (joints[0].y < joints[1].y) {
            return false;
        }
    }
    else {
        return false;
    }

    return true;
}

std::vector<double> BuildingModelerAPI::getDisplacements(int jointTag, std::string analysisTag, size_t timeStep)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }

    auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
    auto output = analysis->getOutput();

    auto disp = output->getNodeDisplacement();
    return disp[jointTag][timeStep];
}

double BuildingModelerAPI::getFloorDR(int floorNumber, std::string analysisTag, size_t dof, size_t timeStep)
{
    if (!floorExists(floorNumber)) {
        throw EntityNotFoundException("Floor number " + std::to_string(floorNumber) + " does not exist.");
    }

    if (floorNumber < 1) {
        throw InvalidInputException("Drift ratio can be recorded starting from fist storey!");
    }

    if (dof < 1 || dof > 3) {
        throw InvalidInputException("Drift ratio can be measured in translational directions, check your input dof!");
    }

    auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);
    if (!floor->isRigid()) {
        return -1;
        throw InvalidOperationException("Both floors must be rigid for DR recording!");
    }

    auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
    auto output = analysis->getOutput();
    auto disp = output->getNodeDisplacement();

    double storeyHeight;
    double previousFloorDisp = 0.0;
    if (floorNumber != (++physicalModel::Building::getInstance().m_floors.begin())->first) {
        auto it = physicalModel::Building::getInstance().m_floors.find(floorNumber);
        it--;

        if (!floor->isRigid()) {
            return -1;
            throw InvalidOperationException("Both floors must be rigid for DR recording!");
        }

        storeyHeight = floor->getFloorHeight() - it->second->getFloorHeight();
        previousFloorDisp = disp[it->second->getMassCenterJointTag()][timeStep][dof - 1];
    }
    else {
        storeyHeight = floor->getFloorHeight();
    }

    auto masterNodeTag = floor->getMassCenterJointTag();
   
    return (disp[masterNodeTag][timeStep][dof-1] - previousFloorDisp) / storeyHeight;
}

double BuildingModelerAPI::getBuildingDR(std::string analysisTag, size_t dof, size_t timeStep)
{
    if (physicalModel::Building::getInstance().m_floors.empty()) {
        throw EntityNotFoundException("Building must have floors for DR recording.");
    }

    if (dof < 1 || dof > 3) {
        throw InvalidInputException("Drift ratio can be measured in translational directions, check your input dof!");
    }

    auto floorNumber = (--physicalModel::Building::getInstance().m_floors.end())->first;
    auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);
    if (!floor->isRigid()) {
        return -1;
        throw InvalidOperationException("Floor must be rigid for DR recording!");
    }

    auto masterNodeTag = floor->getMassCenterJointTag();
    auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
    auto output = analysis->getOutput();

    auto disp = output->getNodeDisplacement();
    return disp[masterNodeTag][timeStep][dof - 1] / floor->getFloorHeight();
}

double BuildingModelerAPI::getTranslationalDispX(int jointTag, std::string analysisTag, size_t timeStep)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }

    auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
    auto output = analysis->getOutput();

    auto disp = output->getNodeDisplacement();
    return disp[jointTag][timeStep][0];
}

double BuildingModelerAPI::getTranslationalDispY(int jointTag, std::string analysisTag, size_t timeStep)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }

    auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
    auto output = analysis->getOutput();

    auto disp = output->getNodeDisplacement();
    return disp[jointTag][timeStep][1];
}

double BuildingModelerAPI::getTranslationalDispZ(int jointTag, std::string analysisTag, size_t timeStep)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }

    auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
    auto output = analysis->getOutput();

    auto disp = output->getNodeDisplacement();
    return disp[jointTag][timeStep][2];
}

double BuildingModelerAPI::getRotationalDispX(int jointTag, std::string analysisTag, size_t timeStep)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }

    auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
    auto output = analysis->getOutput();

    auto disp = output->getNodeDisplacement();
    return disp[jointTag][timeStep][3];
}

double BuildingModelerAPI::getRotationalDispY(int jointTag, std::string analysisTag, size_t timeStep)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }

    auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
    auto output = analysis->getOutput();

    auto disp = output->getNodeDisplacement();
    return disp[jointTag][timeStep][4];
}

double BuildingModelerAPI::getRotationalDispZ(int jointTag, std::string analysisTag, size_t timeStep)
{
    if (!jointExists(jointTag)) {
        throw EntityNotFoundException("Joint with tag " + std::to_string(jointTag) + " does not exist.");
    }

    auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
    auto output = analysis->getOutput();

    auto disp = output->getNodeDisplacement();
    return disp[jointTag][timeStep][5];
}

double BuildingModelerAPI::getLineElementForceX(int elementTag, std::string analysisTag, size_t timeStep, bool atIJoint)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return physicalModel::Building::getInstance().m_lineElements[elementTag]->calculateForceX(analysisTag, atIJoint, timeStep);
}

double BuildingModelerAPI::getLineElementForceY(int elementTag, std::string analysisTag, size_t timeStep, bool atIJoint)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return physicalModel::Building::getInstance().m_lineElements[elementTag]->calculateForceY(analysisTag, atIJoint, timeStep);
}

double BuildingModelerAPI::getLineElementForceZ(int elementTag, std::string analysisTag, size_t timeStep, bool atIJoint)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return physicalModel::Building::getInstance().m_lineElements[elementTag]->calculateForceZ(analysisTag, atIJoint, timeStep);
}

double BuildingModelerAPI::getLineElementMomentXX(int elementTag, std::string analysisTag, size_t timeStep, bool atIJoint)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return physicalModel::Building::getInstance().m_lineElements[elementTag]->calculateMomentXX(analysisTag, atIJoint, timeStep);
}

double BuildingModelerAPI::getLineElementMomentYY(int elementTag, std::string analysisTag, size_t timeStep, bool atIJoint)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return physicalModel::Building::getInstance().m_lineElements[elementTag]->calculateMomentYY(analysisTag, atIJoint, timeStep);
}

double BuildingModelerAPI::getLineElementMomentZZ(int elementTag, std::string analysisTag, size_t timeStep, bool atIJoint)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return physicalModel::Building::getInstance().m_lineElements[elementTag]->calculateMomentZZ(analysisTag, atIJoint, timeStep);
}

double BuildingModelerAPI::getLineElementDR(int elementTag, std::string analysisTag, size_t dof, size_t timeStep)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    if (dof < 1 || dof > 3) {
        throw InvalidInputException("Drift ratio can be measured in translational directions, check your input dof!");
    }

    return physicalModel::Building::getInstance().m_lineElements[elementTag]->calculateDR(analysisTag, dof - 1, timeStep);
}

double BuildingModelerAPI::getLineElementCR(int elementTag, std::string analysisTag, size_t dofRot, size_t timeStep, bool fromIJoint)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    // To do: chord rotation measuring planes are defined by gloal directions. However, local coordinate system would
    // be more correct for skewed members. Since all our members' directions align with global coordinate system
    // this part will remain as it is for now.
    if (dofRot < 4 || dofRot > 5) {
        throw InvalidInputException("Chord rotation can be measured in rotational directions X and Y, check your input rotation dof!");
    }

    return physicalModel::Building::getInstance().m_lineElements[elementTag]->calculateChordRotation(analysisTag, dofRot - 1, fromIJoint, timeStep);
}

double BuildingModelerAPI::getLineElementDisplacement(int elementTag, size_t segmentNode, std::string analysisTag, size_t dof, size_t timeStep)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    if (segmentNode > physicalModel::Building::getInstance().m_lineElements[elementTag]->getAnalyticalNodeTags().size() - 1) {
        throw InvalidInputException("Segment number exceeds the number of segments of line element for displacement recording!");
    }

    if (dof < 1 || dof > 6) {
        throw InvalidInputException("Check your input dof!");
    }

    return physicalModel::Building::getInstance().m_lineElements[elementTag]->calculateDisplacement(analysisTag, segmentNode, dof - 1, timeStep);
}

double BuildingModelerAPI::getShearWallForceX(int elementTag, std::string analysisTag, size_t timeStep, bool atBottom)
{
    if (!areaElementExists(elementTag)) {
        throw EntityNotFoundException("Area element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    physicalModel::ShearWallElement* element = dynamic_cast<physicalModel::ShearWallElement*>(physicalModel::Building::getInstance().getAreaElement(elementTag));
    if (element == nullptr) {
        throw InvalidInputException("Area element with tag " + std::to_string(elementTag) + " is not a shear wall element.");
    }

    return element->calculateShearForceGlobalX(analysisTag, timeStep, atBottom);
}

double BuildingModelerAPI::getShearWallForceY(int elementTag, std::string analysisTag, size_t timeStep, bool atBottom)
{
    if (!areaElementExists(elementTag)) {
        throw EntityNotFoundException("Area element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    physicalModel::ShearWallElement* element = dynamic_cast<physicalModel::ShearWallElement*>(physicalModel::Building::getInstance().getAreaElement(elementTag));
    if (element == nullptr) {
        throw InvalidInputException("Area element with tag " + std::to_string(elementTag) + " is not a shear wall element.");
    }

    return element->calculateShearForceGlobalY(analysisTag, timeStep, atBottom);
}

double BuildingModelerAPI::getShearWallForceZ(int elementTag, std::string analysisTag, size_t timeStep, bool atBottom)
{
    if (!areaElementExists(elementTag)) {
        throw EntityNotFoundException("Area element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    physicalModel::ShearWallElement* element = dynamic_cast<physicalModel::ShearWallElement*>(physicalModel::Building::getInstance().getAreaElement(elementTag));
    if (element == nullptr) {
        throw InvalidInputException("Area element with tag " + std::to_string(elementTag) + " is not a shear wall element.");
    }

    return element->calculateAxialForceGlobalZ(analysisTag, timeStep, atBottom);
}

double BuildingModelerAPI::getShearWallMomentXX(int elementTag, std::string analysisTag, size_t timeStep, bool atBottom)
{
    if (!areaElementExists(elementTag)) {
        throw EntityNotFoundException("Area element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    physicalModel::ShearWallElement* element = dynamic_cast<physicalModel::ShearWallElement*>(physicalModel::Building::getInstance().getAreaElement(elementTag));
    if (element == nullptr) {
        throw InvalidInputException("Area element with tag " + std::to_string(elementTag) + " is not a shear wall element.");
    }

    return element->calculateMomentGlobalXX(analysisTag, timeStep, atBottom);
}

double BuildingModelerAPI::getShearWallMomentYY(int elementTag, std::string analysisTag, size_t timeStep, bool atBottom)
{
    if (!areaElementExists(elementTag)) {
        throw EntityNotFoundException("Area element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    physicalModel::ShearWallElement* element = dynamic_cast<physicalModel::ShearWallElement*>(physicalModel::Building::getInstance().getAreaElement(elementTag));
    if (element == nullptr) {
        throw InvalidInputException("Area element with tag " + std::to_string(elementTag) + " is not a shear wall element.");
    }

    return element->calculateMomentGlobalYY(analysisTag, timeStep, atBottom);
}

double BuildingModelerAPI::getShearWallMomentZZ(int elementTag, std::string analysisTag, size_t timeStep, bool atBottom)
{
    if (!areaElementExists(elementTag)) {
        throw EntityNotFoundException("Area element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    physicalModel::ShearWallElement* element = dynamic_cast<physicalModel::ShearWallElement*>(physicalModel::Building::getInstance().getAreaElement(elementTag));
    if (element == nullptr) {
        throw InvalidInputException("Area element with tag " + std::to_string(elementTag) + " is not a shear wall element.");
    }

    return element->calculateMomentGlobalZZ(analysisTag, timeStep, atBottom);
}

double BuildingModelerAPI::getShearWallDR(int elementTag, std::string analysisTag, size_t dof, size_t timeStep)
{
    if (!areaElementExists(elementTag)) {
        throw EntityNotFoundException("Area element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    physicalModel::ShearWallElement* element = dynamic_cast<physicalModel::ShearWallElement*>(physicalModel::Building::getInstance().getAreaElement(elementTag));
    if (element == nullptr) {
        throw InvalidInputException("Area element with tag " + std::to_string(elementTag) + " is not a shear wall element.");
    }

    if (dof < 1 || dof > 3) {
        throw InvalidInputException("Drift ratio can be measured in translational directions, check your input dof!");
    }

    return element->calculateDR(analysisTag, dof - 1, timeStep);
}

double BuildingModelerAPI::getShearWallCR(int elementTag, std::string analysisTag, size_t dofRot, size_t timeStep, bool fromBottom)
{
    if (!areaElementExists(elementTag)) {
        throw EntityNotFoundException("Area element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    physicalModel::ShearWallElement* element = dynamic_cast<physicalModel::ShearWallElement*>(physicalModel::Building::getInstance().getAreaElement(elementTag));
    if (element == nullptr) {
        throw InvalidInputException("Area element with tag " + std::to_string(elementTag) + " is not a shear wall element.");
    }

    // To do: chord rotation measuring planes are defined by gloal directions. However, local coordinate system would
    // be more correct for skewed members. Since all our members' directions align with global coordinate system
    // this part will remain as it is for now.
    if (dofRot < 4 || dofRot > 5) {
        throw InvalidInputException("Chord rotation can be measured in rotational directions X and Y, check your input rotation dof!");
    }

    return element->calculateChordRotation(analysisTag, dofRot - 1, timeStep, fromBottom);
}

double BuildingModelerAPI::getPeriod(std::string analysisTag, int modeNumber)
{
    if (!loadCaseExists(analysisTag)) {
        throw EntityNotFoundException("Load case: " + analysisTag + " does not exists.");
    }

    auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
    if (analysis == nullptr) {
        throw EntityNotFoundException("Analysis with tag: " + analysisTag + " has not been performed yet.");
    }

    auto modalAnalysis = std::dynamic_pointer_cast<opensees::ModalAnalysis>(analysis);
    if (modalAnalysis == nullptr) {
        throw EntityNotFoundException("Modal analysis with tag: " + analysisTag + " does not exists.");
    }

    if (modeNumber > modalAnalysis->getNumberOfModes()) {
        throw InvalidInputException("Mode number exceeds number of modes.");
    }

    auto modalOutput = std::dynamic_pointer_cast<opensees::ModalOutput>(analysis->getOutput());
    auto periods = modalOutput->getPeriods();

    return periods[modeNumber - 1];
}

double BuildingModelerAPI::getFundamentalPeriod(std::string analysisTag, size_t dof)
{
    if (!loadCaseExists(analysisTag)) {
        throw EntityNotFoundException("Load case: " + analysisTag + " does not exists.");
    }

    auto analysis = opensees::OpenseesModel::getInstance().getAnalysis(analysisTag);
    if (analysis == nullptr) {
        throw EntityNotFoundException("Analysis with tag: " + analysisTag + " has not been performed yet.");
    }

    auto modalAnalysis = std::dynamic_pointer_cast<opensees::ModalAnalysis>(analysis);
    if (modalAnalysis == nullptr) {
        throw EntityNotFoundException("Modal analysis with tag: " + analysisTag + " does not exists.");
    }

    if (dof != 1 || dof != 2) {
        throw InvalidInputException("Fundamental period could be calculated in either global x or y direction.");
    }

    auto modalOutput = std::dynamic_pointer_cast<opensees::ModalOutput>(analysis->getOutput());
    auto fundamentalPeriod = modalOutput->getFundamentalPeriod(dof);

    if (fundamentalPeriod <= 0.0) {
        throw InvalidOperationException("There is no fundamental period in the desired dof.");
    }

    return fundamentalPeriod;
}

void BuildingModelerAPI::createAnalyticalModel()
{
    // Clear analytical model - modifications are only allowed in physical model
    physicalModel::Building::getInstance().toAnalyticalModel();
}

void BuildingModelerAPI::createModelAndLoadingFiles()
{
    opensees::OpenseesModel::getInstance().toTclFile();
}

std::unordered_map<std::string, bool> BuildingModelerAPI::analyze()
{
    return opensees::OpenseesModel::getInstance().analyze();
}