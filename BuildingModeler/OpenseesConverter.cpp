#include "OpenseesConverter.h"
#include "Utilities/LineUtilities.h"
#include <algorithm> 

using namespace buildingModeler;

void OpenseesConverter::toNodeMassConstraint(const physicalModel::Joint* joint)
{
    auto nodeTag = joint->getJointTag();
    if (nodeExists(nodeTag)) {
        // To do: exception
    }
    
    opensees::OpenseesModel::getInstance().m_nodes[nodeTag] = std::make_unique<opensees::Node>(nodeTag, joint->getCoords());

    auto translationalMass = joint->getTranslationalMass();
    auto rotationalMass = joint->getRotationalMass();
    if (translationalMass != std::nullopt || rotationalMass != std::nullopt) {
        opensees::OpenseesModel::getInstance().m_masses[nodeTag] = std::make_unique<opensees::Mass>(nodeTag);
        if (translationalMass != std::nullopt) {
            opensees::OpenseesModel::getInstance().m_masses[nodeTag]->addTranslationalMass(translationalMass.value());
        }
        if (rotationalMass != std::nullopt) {
            opensees::OpenseesModel::getInstance().m_masses[nodeTag]->addRotationalMass(rotationalMass.value());
        }
    }

    auto constraintVector = joint->getConstraintVector();
    if (constraintVector != std::nullopt) {
        opensees::OpenseesModel::getInstance().m_contraintsSP[nodeTag] = std::make_unique<opensees::SingleConstraint>(nodeTag, constraintVector.value());
    }
}

void OpenseesConverter::toMaterial(const physicalModel::Material* material)
{
    auto materialTag = material->getMaterialTag();
    if (materialExists(materialTag)) {
        // To do: exception
    }
    
    switch (material->getMaterialType())
    {
    case physicalModel::MaterialType::ELASTIC:
        opensees::OpenseesModel::getInstance().m_materials[materialTag]
            = std::make_unique<opensees::ElasticMaterial>(materialTag, material->getE(), material->getG(), material->getRho());
        break;
    default:
        break;
    }
}

void OpenseesConverter::toSection(const physicalModel::Section* section)
{
    auto sectionTag = section->getSectionTag();
    if (sectionExists(sectionTag)) {
        // To do: exception
    }
    
    auto materialPhysical = section->getMaterial();
    auto materialAnalytical = opensees::OpenseesModel::getInstance().m_materials[materialPhysical->getMaterialTag()];

    switch (section->getSectionType())
    {
    case physicalModel::SectionType::ELASTIC1D:
        opensees::OpenseesModel::getInstance().m_sections[sectionTag]
            = std::make_unique<opensees::ElasticSection>(sectionTag, materialAnalytical, section->getA(), section->getIyy(), section->getIzz(), section->getJ());
        break;
    case physicalModel::SectionType::ELASTIC2D: {
        auto section2D = dynamic_cast<const physicalModel::ElasticSection2D*>(section);
        opensees::OpenseesModel::getInstance().m_sections[sectionTag]
            = std::make_unique<opensees::ElasticMembranePlateSection>(sectionTag, materialAnalytical, section2D->getThickness());
        break;
    }
    default:
        break;
    }
}

void OpenseesConverter::includePDeltaEffects(bool includePDeltaEffects)
{
    if (opensees::OpenseesModel::getInstance().m_geometricTransformation[0]->getTransfType() != opensees::GeometricTransformationType::PDELTA) {
        
        if (includePDeltaEffects) {
            opensees::OpenseesModel::getInstance().m_geometricTransformation.erase(0);
            opensees::OpenseesModel::getInstance().m_geometricTransformation[0] = std::make_shared<opensees::PDeltaGeometricTransformation>(0, std::vector<int>{0, 1, 0});
        }
    }
    else {
        if (!includePDeltaEffects) {
            opensees::OpenseesModel::getInstance().m_geometricTransformation.erase(0);
            opensees::OpenseesModel::getInstance().m_geometricTransformation[0] = std::make_shared<opensees::LinearGeometricTransformation>(0, std::vector<int>{0, 1, 0});
        }
    }
}

void OpenseesConverter::toBeamColumnElement(physicalModel::LineElement* element)
{
    auto elementTag = element->getElementTag();
    if (beamColumnElementExists(elementTag)) {
        // To do: exception
    }
    else if (!nodeExists(element->getIJointTag()) || !nodeExists(element->getJJointTag())) {
        // To do: exception
    }
    for (int i = 0; i < element->getSegmentLengths().size(); ++i)
    {
        if (!sectionExists(element->getSection(i)->getSectionTag())) {
            // To do: exception
        }
    }
    
    auto nodes = createNodesForLineElement(element);
    std::for_each(nodes.begin(), nodes.end(), [&](int node) { element->addAnalyticalNodeTag(node); });

    std::shared_ptr<opensees::GeometricTransformation> transf;
    if (element->getLineElementType() == physicalModel::LineElementType::COLUMN) {
        transf = opensees::OpenseesModel::getInstance().m_geometricTransformation[0];
    }
    else {
        transf = opensees::OpenseesModel::getInstance().m_geometricTransformation[1];
    }

    elementTag = element->getElementTag();
    for (int i = 0; i < element->getSegmentLengths().size(); ++i) {

        auto section = opensees::OpenseesModel::getInstance().m_sections[element->getSection(i)->getSectionTag()];

        switch (element->getLineElementFormulation())
        {
        case physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI:
            opensees::OpenseesModel::getInstance().m_beamColumnElements[elementTag]
                = std::make_unique<opensees::ElasticBeamColumnElement>(elementTag, std::vector<int>{nodes[i], nodes[i + 1]}, section, transf);
            break;
        default:
            break;
        }

        element->addAnalyticalElementTag(elementTag);

        if (element->getLineElementType() == physicalModel::LineElementType::COLUMN) {
            elementTag = opensees::utilities::TagGenerator::getInstance().getNextColumnTag();
        }
        else {
            elementTag = opensees::utilities::TagGenerator::getInstance().getNextBeamTag();
        }
        if (beamColumnElementExists(elementTag)) {
            // To do: exception
        }
    }
}

void OpenseesConverter::toQuadrilateralElement(physicalModel::AreaElement* element)
{
    auto elementTag = element->getElementTag();
    if (quadrilateralElementExists(elementTag)) {
        // To do: exception
    }
    else if (!nodeExists(element->getIJointTag()) || !nodeExists(element->getJJointTag()) || !nodeExists(element->getKJointTag()) || !nodeExists(element->getLJointTag())) {
        // To do: exception
    }
    else if (!sectionExists(element->getSection()->getSectionTag())) {
        // To do: exception
    }

    auto elementNodes = element->getJointTags();
    std::vector<std::vector<int>> nodes;
    if (element->isMeshable()) {
        auto surroundingElementTags = element->getSurroundingLineElementTags();

        std::vector<int> nodesIJ;
        std::vector<int> nodesLK;
        // We do not check if surroundingElementTags[2] is also -1 as we do that check in BuildingModelerAPI.cpp
        if (surroundingElementTags[0] != -1)
        {
            nodesIJ = physicalModel::Building::getInstance().getLineElement(surroundingElementTags[0])->getAnalyticalNodeTags();
            // Check surrounding element and element directions on IJ edge are in the same direction
            if (nodesIJ[0] == elementNodes[1]) {
                std::reverse(nodesIJ.begin(), nodesIJ.end());
            }

            // Check surrounding element and element directions on LK edge are in the same direction
            nodesLK = physicalModel::Building::getInstance().getLineElement(surroundingElementTags[2])->getAnalyticalNodeTags();
            if (nodesLK[0] == elementNodes[2]) {
                std::reverse(nodesLK.begin(), nodesLK.end());
            }
        }
        else
        {
            nodesIJ = createNodesBetweenTwoJoints(elementNodes[0], elementNodes[1], element->getMeshDivisions().first);
            nodesLK = createNodesBetweenTwoJoints(elementNodes[3], elementNodes[2], element->getMeshDivisions().first);
        }

        std::vector<int> nodesIL;
        std::vector<int> nodesJK;
        // We do not check if surroundingElementTags[2] is also -1 as we do that check in BuildingModelerAPI.cpp
        if (surroundingElementTags[0] != -1)
        {
            nodesIL = physicalModel::Building::getInstance().getLineElement(surroundingElementTags[3])->getAnalyticalNodeTags();
            // Check surrounding element and element directions on IJ edge are in the same direction
            if (nodesIL[0] == elementNodes[3]) {
                std::reverse(nodesIL.begin(), nodesIL.end());
            }

            // Check surrounding element and element directions on LK edge are in the same direction
            nodesJK = physicalModel::Building::getInstance().getLineElement(surroundingElementTags[1])->getAnalyticalNodeTags();
            if (nodesJK[0] == elementNodes[2]) {
                std::reverse(nodesJK.begin(), nodesJK.end());
            }
        }
        else
        {
            nodesIL = createNodesBetweenTwoJoints(elementNodes[0], elementNodes[3], element->getMeshDivisions().second);
            nodesJK = createNodesBetweenTwoJoints(elementNodes[1], elementNodes[2], element->getMeshDivisions().second);
        }

        nodes = createNodesForMesh(nodesIJ, nodesLK, nodesIL, nodesJK);
    }
    else {
        nodes.resize(4);
        for (int i = 0; i < 4; ++i) {
            nodes[i].push_back(elementNodes[i]);
        }
    }
    
    createMeshForQuadElement(element, nodes);
}

void OpenseesConverter::toRigidDiaphragm(physicalModel::Floor* floor)
{
    auto floorNumber = floor->getFloorNumber();

    if (floor->isRigid()) {
        auto masterNodeTag = floor->getMassCenterJointTag();
        auto slaveNodeTags = floor->getJoints();
        opensees::OpenseesModel::getInstance().m_contraintsDiaphragm[floorNumber] = std::make_unique<opensees::DiaphragmConstraint>(masterNodeTag, slaveNodeTags, 3);
    }
}

bool OpenseesConverter::nodeExists(int nodeTag)
{
    if (opensees::OpenseesModel::getInstance().m_nodes.find(nodeTag) != opensees::OpenseesModel::getInstance().m_nodes.end()) {
        return true;
    }

    return false;
}

bool OpenseesConverter::materialExists(int materialTag)
{
    if (opensees::OpenseesModel::getInstance().m_materials.find(materialTag) != opensees::OpenseesModel::getInstance().m_materials.end()) {
        return true;
    }

    return false;
}

bool OpenseesConverter::sectionExists(int sectionTag)
{
    if (opensees::OpenseesModel::getInstance().m_sections.find(sectionTag) != opensees::OpenseesModel::getInstance().m_sections.end()) {
        return true;
    }

    return false;
}

bool OpenseesConverter::beamColumnElementExists(int elementTag)
{
    if (opensees::OpenseesModel::getInstance().m_beamColumnElements.find(elementTag) != opensees::OpenseesModel::getInstance().m_beamColumnElements.end()) {
        return true;
    }

    return false;
}

bool OpenseesConverter::quadrilateralElementExists(int elementTag)
{
    if (opensees::OpenseesModel::getInstance().m_quadrilateralElements.find(elementTag) != opensees::OpenseesModel::getInstance().m_quadrilateralElements.end()) {
        return true;
    }

    return false;
}

std::vector<int> OpenseesConverter::createNodesForLineElement(const physicalModel::LineElement* element)
{
    auto startCoord = opensees::OpenseesModel::getInstance().m_nodes[element->getIJointTag()]->getCoords();
    auto endCoord = opensees::OpenseesModel::getInstance().m_nodes[element->getJJointTag()]->getCoords();
    auto direction = (endCoord - startCoord) / element->getElementLength();

    std::vector<int> nodes;
    nodes.push_back(element->getIJointTag());

    auto currentCoord = startCoord;
    auto lengths = element->getSegmentLengths();
    for (int i = 0; i < lengths.size() - 1; ++i) {

        auto nodeTag = opensees::utilities::TagGenerator::getInstance().getNextNodeTag();
        if (nodeExists(nodeTag)) {
            // To do: exception
        }

        currentCoord = currentCoord + (lengths[i] * direction);
        opensees::OpenseesModel::getInstance().m_nodes[nodeTag] = std::make_unique<opensees::Node>(nodeTag, currentCoord);
        nodes.push_back(nodeTag);
    }
    nodes.push_back(element->getJJointTag());
    
    return nodes;
}

std::vector<int> OpenseesConverter::createNodesBetweenTwoJoints(int jointTagA, int jointTagB, int numberOfIntervals)
{
    if (numberOfIntervals < 1) {
        // To do exception
        return {};
    }

    auto constraintA = opensees::OpenseesModel::getInstance().getSPConstraint(jointTagA);
    auto constraintB = opensees::OpenseesModel::getInstance().getSPConstraint(jointTagB);
    auto assignConstraint = false;
    std::vector<int> constraintVector;
    if ((constraintA && constraintB) && (constraintA == constraintB)) {
        assignConstraint = true;
        constraintVector = dynamic_cast<opensees::SingleConstraint*>(constraintA)->getFixedDOFs();
    }

    auto startCoord = opensees::OpenseesModel::getInstance().m_nodes[jointTagA]->getCoords();
    auto endCoord = opensees::OpenseesModel::getInstance().m_nodes[jointTagB]->getCoords();
    auto increment = (endCoord - startCoord) / (double)numberOfIntervals;

    std::vector<int> nodes;
    nodes.push_back(jointTagA);

    auto currentCoord = startCoord;
    for (int i = 0; i < numberOfIntervals - 1; ++i) {

        auto nodeTag = opensees::utilities::TagGenerator::getInstance().getNextNodeTag();
        if (nodeExists(nodeTag)) {
            // To do: exception
        }

        currentCoord = currentCoord + increment;
        opensees::OpenseesModel::getInstance().m_nodes[nodeTag] = std::make_unique<opensees::Node>(nodeTag, currentCoord);
        nodes.push_back(nodeTag);

        if (assignConstraint) {
            opensees::OpenseesModel::getInstance().m_contraintsSP[nodeTag] = std::make_unique<opensees::SingleConstraint>(nodeTag, constraintVector);
        }
    }
    nodes.push_back(jointTagB);

    return nodes;
}

std::vector<std::vector<int>> OpenseesConverter::createNodesForMesh(std::vector<int> nodesIJ, std::vector<int> nodesLK, std::vector<int> nodesIL, std::vector<int> nodesJK)
{
    std::vector<std::vector<int>> nodes2D;
    nodes2D.resize(nodesIL.size());
    for (auto& nodes : nodes2D) {
        nodes.resize(nodesIJ.size());
    }

    nodes2D[0] = nodesIJ;
    for (int i = 1; i < nodes2D.size() - 1; ++i) {

        nodes2D[i][0] = nodesIL[i];
        for (int j = 1; j < nodes2D[i].size() - 1; j++) {

            auto startCoordP = opensees::OpenseesModel::getInstance().m_nodes[nodesIJ[j]]->getCoords();
            auto endCoordP = opensees::OpenseesModel::getInstance().m_nodes[nodesLK[i]]->getCoords();
            auto startCoordQ = opensees::OpenseesModel::getInstance().m_nodes[nodesIL[j]]->getCoords();
            auto endCoordQ = opensees::OpenseesModel::getInstance().m_nodes[nodesJK[i]]->getCoords();

            utility::Vector3 nodeCoord;
            auto intersects = utility::LineUtilities::intersectsVector3(startCoordP, endCoordP, startCoordQ, endCoordQ, nodeCoord);
            if (!intersects) {
                // To do: exception
            }
            auto nodeTag = opensees::utilities::TagGenerator::getInstance().getNextNodeTag();
            if (nodeExists(nodeTag)) {
                // To do: exception
            }

            opensees::OpenseesModel::getInstance().m_nodes[nodeTag] = std::make_unique<opensees::Node>(nodeTag, nodeCoord);
            nodes2D[i][j] = nodeTag;
        }
        nodes2D[i][nodes2D[i].size() - 1] = nodesJK[i];
    }
    nodes2D[nodes2D.size() - 1] = nodesLK;

    return nodes2D;
}

void OpenseesConverter::createMeshForQuadElement(physicalModel::AreaElement* element, const std::vector<std::vector<int>>& nodes)
{
    auto elementTag = element->getElementTag();
    auto section = opensees::OpenseesModel::getInstance().m_sections[element->getSection()->getSectionTag()];

    for (int i = 0; i < nodes.size() - 1; ++i) {

        for (int j = 0; j < nodes[i].size() - 1; ++j) {

            std::vector<int> nodeTags;
            nodeTags.push_back(nodes[i][j]);
            nodeTags.push_back(nodes[i][j + 1]);
            nodeTags.push_back(nodes[i + 1][j + 1]);
            nodeTags.push_back(nodes[i + 1][j]);

            switch (element->getAreaElementFormulation())
            {
            case physicalModel::AreaElementFormulation::LINEAR:
                opensees::OpenseesModel::getInstance().m_quadrilateralElements[elementTag]
                    = std::make_unique<opensees::ShellElement>(elementTag, nodeTags, section, opensees::ShellElementType::DKGQ);
                break;
            case physicalModel::AreaElementFormulation::LINEAR_MODIFIED_SHEAR:
                opensees::OpenseesModel::getInstance().m_quadrilateralElements[elementTag]
                    = std::make_unique<opensees::ShellElement>(elementTag, nodeTags, section, opensees::ShellElementType::MITC4);
                break;
            case physicalModel::AreaElementFormulation::NONLINEAR_GEOMETRIC:
                opensees::OpenseesModel::getInstance().m_quadrilateralElements[elementTag]
                    = std::make_unique<opensees::ShellElement>(elementTag, nodeTags, section, opensees::ShellElementType::NLDKGQ);
                break;
            default:
                break;
            }

            element->addAnalyticalNodeTags(nodeTags);
            element->addAnalyticalElementTag(elementTag);

            if (element->getAreaElementType() == physicalModel::AreaElementType::SLAB) {
                elementTag = opensees::utilities::TagGenerator::getInstance().getNextSlabTag();
            }
            else {
                elementTag = opensees::utilities::TagGenerator::getInstance().getNextShearWallTag();
            }
            if (quadrilateralElementExists(elementTag)) {
                // To do: exception
            }
        }
    }
}