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
    
    physicalModel::Building::getInstance().m_joints[jointTag]->setFloorNo(floorNumber);
    physicalModel::Building::getInstance().m_floors[floorNumber]->addJoint(jointTag);
}

void BuildingModelerAPI::includeMassFromMembers(bool includeMassFromMembers)
{
    physicalModel::Building::getInstance().m_includeMassFromMembers = includeMassFromMembers;
}

void BuildingModelerAPI::addElasticMaterial(int materialTag, double E, double G, double rho)
{
    if (materialExists(materialTag)) {
        throw EntityFoundException("Material with tag " + std::to_string(materialTag) + " already exists.");
    }
    
    physicalModel::Building::getInstance().m_materials[materialTag] = std::make_unique<physicalModel::ElasticMaterial>(materialTag, E, G, rho);
}

void BuildingModelerAPI::addElasticSection1D(int sectionTag, int materialTag, double A, double Iyy, double Izz, double J)
{
    if (sectionExists(sectionTag)) {
        throw EntityFoundException("Section with tag " + std::to_string(sectionTag) + " already exists.");
    }

    if (!materialExists(materialTag)) {
        throw EntityNotFoundException("Material with tag " + std::to_string(materialTag) + " does not exist.");
    }
    
    std::shared_ptr<physicalModel::Material> material = physicalModel::Building::getInstance().m_materials[materialTag];
    physicalModel::Building::getInstance().m_sections[sectionTag] = std::make_unique<physicalModel::ElasticSection1D>(sectionTag, material, A, Iyy, Izz, J);
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
    physicalModel::Building::getInstance().m_sections[sectionTag] = std::make_unique<physicalModel::ElasticSection2D>(sectionTag, material, thickness);
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

const std::vector<utility::Vector3>& BuildingModelerAPI::getNodeCoordinatesOfLineElement(int elementTag)
{
    if (!lineElementExists(elementTag)) {
        throw EntityNotFoundException("Line element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return physicalModel::Building::getInstance().m_lineElements[elementTag]->getAnalyticalNodeCoords();
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

    if (!jointExists(jointTags[0]) || !jointExists(jointTags[1]) || !jointExists(jointTags[1]) || !jointExists(jointTags[2])) {
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

    if (!jointExists(jointTags[0]) || !jointExists(jointTags[1]) || !jointExists(jointTags[1]) || !jointExists(jointTags[2])) {
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
}

void BuildingModelerAPI::meshAreaElement(int elementTag, std::optional<int> n1, std::optional<int> n2)
{
    if (!areaElementExists(elementTag)) {
        throw EntityNotFoundException("Area element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    auto areaElement = physicalModel::Building::getInstance().getAreaElement(elementTag);
    auto surroundingElementTags = areaElement->getSurroundingLineElementTags();

    if ((surroundingElementTags[0] != -1 || surroundingElementTags[2] != -1) && n1 != std::nullopt) {
        throw InvalidInputException("n1 cannot be assigned while there are surrounding elements in that direction.");
    }

    if ((surroundingElementTags[1] != -1 || surroundingElementTags[3] != -1) && n2 != std::nullopt) {
        throw InvalidInputException("n2 cannot be assigned while there are surrounding elements in that direction.");
    }
    
    if ((surroundingElementTags[0] * surroundingElementTags[2] < 0) && n1 == std::nullopt)
    {
        throw InvalidInputException("There must be surrounding elements on the opposite sides when n1 is not assigned.");
    }
    
    if ((surroundingElementTags[1] * surroundingElementTags[3] < 0) && n2 == std::nullopt)
    {
        throw InvalidInputException("There must be surrounding elements on the opposite sides when n2 is not assigned.");
    }

    bool meshIJ = true;
    if (n1 == std::nullopt)
    {
        if (surroundingElementTags[0] < 0 || surroundingElementTags[2] < 0) {
            throw InvalidInputException("There must be surrounding elements on the opposite sides when n1 is not assigned.");
        }

        auto element1 = physicalModel::Building::getInstance().getLineElement(surroundingElementTags[0]);
        auto element2 = physicalModel::Building::getInstance().getLineElement(surroundingElementTags[2]);

        if (element1 == nullptr || element2 == nullptr) {
            throw EntityNotFoundException("Surrounding line elements with tags " + std::to_string(surroundingElementTags[0]) + " or "
                + std::to_string(surroundingElementTags[1])  + " does not exist.");
        }

        if (element1->getSegmentLengths().size() != element2->getSegmentLengths().size()) {
            throw InvalidInputException("Surrounding elements on the opposite sides must have the same number of elements.");
        }

        if (element1->getSegmentLengths().size() == 1) {
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
        if (surroundingElementTags[1] < 0 || surroundingElementTags[3] < 0) {
            throw InvalidInputException("There must be surrounding elements on the opposite sides when n2 is not assigned.");
        }

        auto element1 = physicalModel::Building::getInstance().getLineElement(surroundingElementTags[1]);
        auto element2 = physicalModel::Building::getInstance().getLineElement(surroundingElementTags[3]);

        if (element1 == nullptr || element2 == nullptr) {
            throw EntityNotFoundException("Surrounding line elements with tags " + std::to_string(surroundingElementTags[0]) + " or "
                + std::to_string(surroundingElementTags[1]) + " does not exist.");
        }

        if (element1->getSegmentLengths().size() != element2->getSegmentLengths().size()) {
            throw InvalidInputException("Surrounding elements on the opposite sides must have the same number of elements.");
        }
        if (element1->getSegmentLengths().size() == 1) {
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

const std::vector<std::vector<utility::Vector3>>& BuildingModelerAPI::getNodeCoordinatesOfAreaElement(int elementTag)
{
    if (!areaElementExists(elementTag)) {
        throw EntityNotFoundException("Area element with tag " + std::to_string(elementTag) + " does not exist.");
    }

    return physicalModel::Building::getInstance().getAreaElement(elementTag)->getAnalyticalNodeCoords();
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
}

void BuildingModelerAPI::makeRigid(int floorNumber, int masterJointTag)
{
    if (jointExists(masterJointTag)) {
        // To do: exception
    }

    if (!floorExists(floorNumber)) {
        // To do: exception
    }

    auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);
    if (floor->isRigid()) {
        // To do: exception
    }

    if (floor->getMassCenterJointTag() != -1) {
        // To do: exception
    }
    
    auto massCenter = floor->getMassCenter();
    if (massCenter == std::nullopt)
    {
        if (!floor->updateMassCenter()) {
            //To do: exception
        }
        massCenter = floor->getMassCenter();
    }

    utility::Vector3 coords;
    coords.x = massCenter.value().x;
    coords.y = massCenter.value().y;
    coords.z = floor->getFloorHeight();

    physicalModel::Building::getInstance().m_joints[masterJointTag] = std::make_unique<physicalModel::Joint>(masterJointTag, coords);
    physicalModel::Building::getInstance().m_floors[floorNumber]->makeRigid(masterJointTag);
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
        auto floor = physicalModel::Building::getInstance().getFloor(floorNumber);
        auto masterJointTag = floor->getMassCenterJointTag();
        floor->makeFlexible();
        physicalModel::Building::getInstance().deleteJoint(masterJointTag);
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

    auto totalSignBit = 0;
    for (int i = 0; i < 4; ++i) {
        auto startInd = i;
        auto endInd = (i == 3) ? 0 : i + 1;

        auto crossProduct = utility::VectorUtilities::crossProduct(vecs[startInd], vecs[endInd]);
        totalSignBit += std::signbit(crossProduct.z);
    }

    if (totalSignBit == 4 || totalSignBit == 0) {
        return true;
    }

    return false;
}

void BuildingModelerAPI::createInputFile()
{
    physicalModel::Building::getInstance().toAnalyticalModel();
    opensees::OpenseesModel::getInstance().toTclFile();
}