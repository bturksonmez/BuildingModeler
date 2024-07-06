#include "OpenseesConverter.h"
#include "Utilities/VectorUtilities.h"
#include "BuildingModelerExceptions.h"

#include <algorithm> 

using namespace buildingModeler;

void OpenseesConverter::toNodeMassConstraint(const physicalModel::Joint* joint)
{
    auto nodeTag = joint->getJointTag();
    if (nodeExists(nodeTag)) {
        // To do: exception
    }
    
    opensees::OpenseesModel::getInstance().m_nodes[nodeTag] = std::make_unique<opensees::Node>(nodeTag, joint->getCoords());

    bool isValid;
    if (joint->getFloorNo() == -1) {
        isValid = true;
    }
    else {
        isValid = !physicalModel::Building::getInstance().getFloor(joint->getFloorNo())->floorMassConfinedOnDiaphragmNode();
    }

    auto translationalMass = joint->getTranslationalMass();
    auto rotationalMass = joint->getRotationalMass();
    if ((translationalMass != std::nullopt || rotationalMass != std::nullopt) && isValid) {
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
            = std::make_shared<opensees::ElasticMaterial>(materialTag, material->getE(), material->getG(), material->getRho());
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
            = std::make_shared<opensees::ElasticSection>(sectionTag, materialAnalytical, section->getA().value(), section->getIyy().value(), section->getIzz().value(), section->getJ().value());
        break;
    case physicalModel::SectionType::ELASTIC2D: {
        auto section2D = dynamic_cast<const physicalModel::ElasticSection2D*>(section);
        opensees::OpenseesModel::getInstance().m_sections[sectionTag]
            = std::make_shared<opensees::ElasticMembranePlateSection>(sectionTag, materialAnalytical, section2D->getThickness());
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

    for (int i = 0; i < element->getSegmentLengths().size(); ++i) {

        if (element->getLineElementType() == physicalModel::LineElementType::COLUMN && i != 0) {
            elementTag = opensees::utilities::TagGenerator::getInstance().getNextColumnTag();
        }
        else if (element->getLineElementType() == physicalModel::LineElementType::BEAM && i != 0) {
            elementTag = opensees::utilities::TagGenerator::getInstance().getNextBeamTag();
        }
        if (beamColumnElementExists(elementTag)) {
            // To do: exception
        }

        auto section = opensees::OpenseesModel::getInstance().m_sections[element->getSection(i)->getSectionTag()];
        auto sectionModifiers = element->getSectionModifiers(i);
        auto modifiers = std::vector<double>{ sectionModifiers->m_modifierA, sectionModifiers->m_modifierIyy, sectionModifiers->m_modifierIzz, sectionModifiers->m_modifierJ };

        switch (element->getLineElementFormulation())
        {
        case physicalModel::LineElementFormulation::LINEAR_EULER_BERNOULLI:
            opensees::OpenseesModel::getInstance().m_beamColumnElements[elementTag]
                = std::make_unique<opensees::ElasticBeamColumnElement>(elementTag, std::vector<int>{nodes[i], nodes[i + 1]}, section, modifiers, transf);
            break;
        default:
            break;
        }

        element->addAnalyticalElementTag(elementTag);
        opensees::OpenseesModel::getInstance().addOutputElement(elementTag, opensees::ElementType::BEAMCOLUMN);
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
            nodesIJ = opensees::OpenseesModel::getInstance().getDivisionsBetweenNodes(elementNodes[0], elementNodes[1]);
            if (nodesIJ.empty()) {
                nodesIJ = opensees::OpenseesModel::getInstance().getDivisionsBetweenNodes(elementNodes[1], elementNodes[0]);

                if (nodesIJ.empty()) {
                    nodesIJ = createNodesBetweenTwoJoints(elementNodes[0], elementNodes[1], element->getMeshDivisions().first);
                    opensees::OpenseesModel::getInstance().addDivisionsBetweenNodes(elementNodes[0], elementNodes[1], nodesIJ);
                }
                else {
                    std::reverse(nodesIJ.begin(), nodesIJ.end());
                }
                
            }
            else {
                if ((int)nodesIJ.size() - 1 != element->getMeshDivisions().first) {
                    throw InvalidInputException("Area elements sharing an edge must have the same mesh size along that edge!.");
                }
            }

            nodesLK = opensees::OpenseesModel::getInstance().getDivisionsBetweenNodes(elementNodes[3], elementNodes[2]);
            if (nodesLK.empty()) {
                nodesLK = opensees::OpenseesModel::getInstance().getDivisionsBetweenNodes(elementNodes[2], elementNodes[3]);

                if (nodesLK.empty()) {
                    nodesLK = createNodesBetweenTwoJoints(elementNodes[3], elementNodes[2], element->getMeshDivisions().first);
                    opensees::OpenseesModel::getInstance().addDivisionsBetweenNodes(elementNodes[3], elementNodes[2], nodesLK);
                }
                else {
                    std::reverse(nodesLK.begin(), nodesLK.end());
                }
            }
            else {
                if ((int)nodesLK.size() - 1 != element->getMeshDivisions().first) {
                    throw InvalidInputException("Area elements sharing an edge must have the same mesh size along that edge!.");
                }
            }
        }

        std::vector<int> nodesIL;
        std::vector<int> nodesJK;
        // We do not check if surroundingElementTags[3] is also -1 as we do that check in BuildingModelerAPI.cpp
        if (surroundingElementTags[1] != -1)
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
            nodesIL = opensees::OpenseesModel::getInstance().getDivisionsBetweenNodes(elementNodes[0], elementNodes[3]);
            if (nodesIL.empty()) {
                nodesIL = opensees::OpenseesModel::getInstance().getDivisionsBetweenNodes(elementNodes[3], elementNodes[0]);

                if (nodesIL.empty()) {
                    nodesIL = createNodesBetweenTwoJoints(elementNodes[0], elementNodes[3], element->getMeshDivisions().second);
                    opensees::OpenseesModel::getInstance().addDivisionsBetweenNodes(elementNodes[0], elementNodes[3], nodesIL);
                }
                else {
                    std::reverse(nodesIL.begin(), nodesIL.end());
                }
            }
            else {
                if ((int)nodesIL.size() - 1 != element->getMeshDivisions().second) {
                    throw InvalidInputException("Area elements sharing an edge must have the same mesh size along that edge!.");
                }
            }

            nodesJK = opensees::OpenseesModel::getInstance().getDivisionsBetweenNodes(elementNodes[1], elementNodes[2]);
            if (nodesJK.empty()) {
                nodesJK = opensees::OpenseesModel::getInstance().getDivisionsBetweenNodes(elementNodes[2], elementNodes[1]);

                if (nodesJK.empty()) {
                    nodesJK = createNodesBetweenTwoJoints(elementNodes[1], elementNodes[2], element->getMeshDivisions().second);
                    opensees::OpenseesModel::getInstance().addDivisionsBetweenNodes(elementNodes[1], elementNodes[2], nodesJK);
                }
                else {
                    std::reverse(nodesJK.begin(), nodesJK.end());
                }
            }
            else {
                if ((int)nodesJK.size() - 1 != element->getMeshDivisions().second) {
                    throw InvalidInputException("Area elements sharing an edge must have the same mesh size along that edge!.");
                }
            }

        }

        nodes = createNodesForMesh(nodesIJ, nodesLK, nodesIL, nodesJK);
    }
    else {
        nodes.resize(2);
        nodes[0].push_back(elementNodes[0]);
        nodes[0].push_back(elementNodes[1]);
        nodes[1].push_back(elementNodes[3]);
        nodes[1].push_back(elementNodes[2]);
    }
    
    createMeshForQuadElement(element, nodes);
}

void OpenseesConverter::toRigidDiaphragm(physicalModel::Floor* floor)
{
    auto floorNumber = floor->getFloorNumber();

    if (floor->isRigid()) {
        auto masterNodeTag = floor->getMassCenterJointTag();
        auto masterJoint = physicalModel::Building::getInstance().getJoint(masterNodeTag);
        
        auto translationalMass = masterJoint->getTranslationalMass();
        auto rotationalMass = masterJoint->getRotationalMass();
        if ((translationalMass != std::nullopt || rotationalMass != std::nullopt)) {
            opensees::OpenseesModel::getInstance().m_masses[masterNodeTag] = std::make_unique<opensees::Mass>(masterNodeTag);
            if (translationalMass != std::nullopt) {
                opensees::OpenseesModel::getInstance().m_masses[masterNodeTag]->addTranslationalMass(translationalMass.value());
            }
            if (rotationalMass != std::nullopt) {
                opensees::OpenseesModel::getInstance().m_masses[masterNodeTag]->addRotationalMass(rotationalMass.value());
            }
        }

        auto slaveNodeTags = floor->getJoints();
        opensees::OpenseesModel::getInstance().m_contraintsDiaphragm[floorNumber] = std::make_unique<opensees::DiaphragmConstraint>(masterNodeTag, slaveNodeTags, 3);
    }
}

void OpenseesConverter::toAnalysisObjectFromLoadCase(physicalModel::LoadCase* loadCase)
{
    if (auto staticLoadCase = dynamic_cast<physicalModel::StaticLoadCase*>(loadCase)) {
        opensees::OpenseesModel::getInstance().m_loadPatterns[staticLoadCase->getLoadCaseTag()] = std::make_shared<opensees::LoadPattern>(staticLoadCase->getLoadCaseTag(), opensees::TimeSeriesType::LINEAR);

        auto loadPattern = opensees::OpenseesModel::getInstance().getLoadPattern(staticLoadCase->getLoadCaseTag());

        auto pointLoads = staticLoadCase->getPointLoads();
        for (const auto& pointLoad : pointLoads) {

            std::shared_ptr<opensees::Load> load = std::make_shared<opensees::NodalLoad>(std::dynamic_pointer_cast<physicalModel::PointLoad>(pointLoad)->getJointTag(), pointLoad->getLoadVector());
            loadPattern->addLoad(load);
        }

        auto lineLoads = staticLoadCase->getDistributedLineLoads();
        for (const auto& lineLoad : lineLoads) {

            auto lineElement = physicalModel::Building::getInstance().getLineElement(std::dynamic_pointer_cast<physicalModel::DistributedLineLoad>(lineLoad)->getBeamElementTag());

            for (const auto& analyticalElementTag : lineElement->getAnalyticalElementTags()) {

                std::shared_ptr<opensees::Load> load = std::make_shared<opensees::ElementLoad>(analyticalElementTag, lineLoad->getLoadVector());
                loadPattern->addLoad(load);
            }
        }

        auto areaLoads = staticLoadCase->getDistributedAreaLoads();
        for (const auto& areaLoad : areaLoads) {

            auto areaElement = physicalModel::Building::getInstance().getAreaElement(std::dynamic_pointer_cast<physicalModel::DistributedAreaLoad>(areaLoad)->getAreaElementTag());
            auto analyticalNodeTags = areaElement->getAnalyticalNodeTags();
            auto analyticalNodeCoords = areaElement->getAnalyticalNodeCoords();
            auto load = areaLoad->getLoadVector();

            for (int i = 0; i < analyticalNodeTags.size(); ++i) {

                auto subArea = utility::VectorUtilities::calculateQuadArea(analyticalNodeCoords[i][0], analyticalNodeCoords[i][1], analyticalNodeCoords[i][2], analyticalNodeCoords[i][3]);

                // To do: This part only considers slabs loading in z directions. It should be more general!
                std::vector<double> loadVec{ 0, 0, 0, 0, 0, 0 };
                loadVec[2] = subArea * load[0] / 4.0;

                for (const auto& nodeTag : analyticalNodeTags[i]) {

                    std::shared_ptr<opensees::Load> load = std::make_shared<opensees::NodalLoad>(nodeTag, loadVec);
                    loadPattern->addLoad(load);
                }
            }
        }
        std::shared_ptr<opensees::Output> output = std::make_shared<opensees::StaticOutput>(staticLoadCase->getLoadCaseTag());

        std::shared_ptr<opensees::Analysis> analysis = std::make_shared<opensees::StaticAnalysis>(opensees::OpenseesModel::getInstance().getModelName(), staticLoadCase->getLoadCaseTag(), output, loadPattern);
        opensees::OpenseesModel::getInstance().m_analyses.insert(analysis);
    }
    else if (auto modalLoadCase = dynamic_cast<physicalModel::ModalLoadCase*>(loadCase)) {
        // To do: throw exception if floor is not rigid!
        std::vector<int> masterNodeTags;
        int floorNo = 1;
        while (true) {
            auto floor = physicalModel::Building::getInstance().getFloor(floorNo);

            if (floor == nullptr) {
                break;
            }

            masterNodeTags.push_back(floor->getMassCenterJointTag());
            floorNo++;
        }
        std::shared_ptr<opensees::Output> output = std::make_shared<opensees::ModalOutput>(modalLoadCase->getLoadCaseTag(), masterNodeTags);

        std::shared_ptr<opensees::Analysis> analysis = std::make_shared<opensees::ModalAnalysis>(opensees::OpenseesModel::getInstance().getModelName(), modalLoadCase->getLoadCaseTag(), output, modalLoadCase->getNumberOfModes(), masterNodeTags);
        opensees::OpenseesModel::getInstance().m_analyses.insert(analysis);    
    }
    
}

void OpenseesConverter::toAnalysisObjectFromStaticLoadCombination(physicalModel::StaticLoadCombination* loadCombination)
{
    opensees::OpenseesModel::getInstance().m_loadPatterns[loadCombination->getLoadCombinationTag()] = std::make_shared<opensees::LoadPattern>(loadCombination->getLoadCombinationTag(), opensees::TimeSeriesType::LINEAR);
    auto loadPattern = opensees::OpenseesModel::getInstance().getLoadPattern(loadCombination->getLoadCombinationTag());

    for (const auto [loadCase, factor] : loadCombination->getLoadCases()) {
        
        if (auto staticLoadCase = std::dynamic_pointer_cast<physicalModel::StaticLoadCase>(loadCase)) {
            auto pointLoads = staticLoadCase->getPointLoads();
            for (const auto& pointLoad : pointLoads) {

                std::shared_ptr<opensees::Load> load = std::make_shared<opensees::NodalLoad>(std::dynamic_pointer_cast<physicalModel::PointLoad>(pointLoad)->getJointTag(), pointLoad->getLoadVector());
                (*load) *= factor;
                loadPattern->addLoad(load);
            }

            auto lineLoads = staticLoadCase->getDistributedLineLoads();
            for (const auto& lineLoad : lineLoads) {

                auto lineElement = physicalModel::Building::getInstance().getLineElement(std::dynamic_pointer_cast<physicalModel::DistributedLineLoad>(lineLoad)->getBeamElementTag());

                for (const auto& analyticalElementTag : lineElement->getAnalyticalElementTags()) {

                    std::shared_ptr<opensees::Load> load = std::make_shared<opensees::ElementLoad>(analyticalElementTag, lineLoad->getLoadVector());
                    (*load) *= factor;
                    loadPattern->addLoad(load);
                }
            }

            auto areaLoads = staticLoadCase->getDistributedAreaLoads();
            for (const auto& areaLoad : areaLoads) {

                auto areaElement = physicalModel::Building::getInstance().getAreaElement(std::dynamic_pointer_cast<physicalModel::DistributedAreaLoad>(areaLoad)->getAreaElementTag());
                auto analyticalNodeTags = areaElement->getAnalyticalNodeTags();
                auto analyticalNodeCoords = areaElement->getAnalyticalNodeCoords();
                auto load = areaLoad->getLoadVector();

                for (int i = 0; i < analyticalNodeTags.size(); ++i) {

                    auto subArea = utility::VectorUtilities::calculateQuadArea(analyticalNodeCoords[i][0], analyticalNodeCoords[i][1], analyticalNodeCoords[i][2], analyticalNodeCoords[i][3]);

                    // To do: This part only considers slabs loading in z directions. It should be more general!
                    std::vector<double> loadVec{ 0, 0, 0, 0, 0, 0 };
                    loadVec[2] = factor * subArea * load[0] / 4.0;

                    for (const auto& nodeTag : analyticalNodeTags[i]) {

                        std::shared_ptr<opensees::Load> load = std::make_shared<opensees::NodalLoad>(nodeTag, loadVec);
                        loadPattern->addLoad(load);
                    }
                }
            }
        }
        std::shared_ptr<opensees::Output> output = std::make_shared<opensees::StaticOutput>(loadCombination->getLoadCombinationTag());

        std::shared_ptr<opensees::Analysis> analysis = std::make_shared<opensees::StaticAnalysis>(opensees::OpenseesModel::getInstance().getModelName(), loadCombination->getLoadCombinationTag(), output, loadPattern);
        opensees::OpenseesModel::getInstance().m_analyses.insert(analysis);
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

bool OpenseesConverter::loadPatternExists(std::string loadingName)
{
    if (opensees::OpenseesModel::getInstance().m_loadPatterns.find(loadingName) != opensees::OpenseesModel::getInstance().m_loadPatterns.end()) {
        return true;
    }

    return false;
}

std::vector<int> OpenseesConverter::createNodesForLineElement(const physicalModel::LineElement* element)
{
    auto startCoord = opensees::OpenseesModel::getInstance().m_nodes[element->getIJointTag()]->getCoords();
    auto endCoord = opensees::OpenseesModel::getInstance().m_nodes[element->getJJointTag()]->getCoords();
    auto direction = (endCoord - startCoord) / element->getLength();

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
    if ((constraintA && constraintB) && (*constraintA == *constraintB)) {
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

std::vector<std::vector<int>> OpenseesConverter::createNodesForMesh(const std::vector<int>& nodesIJ, const std::vector<int>& nodesLK, const std::vector<int>& nodesIL, const std::vector<int>& nodesJK)
{
    auto [u, v, w] = getLocalCoordinateSystem(nodesIJ, nodesIL);
    auto zeroCoord = opensees::OpenseesModel::getInstance().m_nodes[nodesIJ.front()]->getCoords();

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
            auto startPlocal = utility::VectorUtilities::projectVectorOn2DLocalBasis((startCoordP - zeroCoord), u, v);
            auto endCoordP = opensees::OpenseesModel::getInstance().m_nodes[nodesLK[j]]->getCoords();
            auto endPlocal = utility::VectorUtilities::projectVectorOn2DLocalBasis((endCoordP - zeroCoord), u, v);
            auto startCoordQ = opensees::OpenseesModel::getInstance().m_nodes[nodesIL[i]]->getCoords();
            auto startQlocal = utility::VectorUtilities::projectVectorOn2DLocalBasis((startCoordQ - zeroCoord), u, v);
            auto endCoordQ = opensees::OpenseesModel::getInstance().m_nodes[nodesJK[i]]->getCoords();
            auto endQlocal = utility::VectorUtilities::projectVectorOn2DLocalBasis((endCoordQ - zeroCoord), u, v);

            utility::Vector3 nodeCoord;
            utility::Vector2 nodeCoordLocal;
            auto intersects = utility::VectorUtilities::intersectsVector2(startPlocal, endPlocal, startQlocal, endQlocal, nodeCoordLocal);
            if (!intersects) {
                // To do: exception
            }
            nodeCoord = zeroCoord + nodeCoordLocal.x * u + nodeCoordLocal.y * v;

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

            if (element->getAreaElementType() == physicalModel::AreaElementType::SLAB && (i != 0 || j != 0)) {
                elementTag = opensees::utilities::TagGenerator::getInstance().getNextSlabTag();
            }
            else if (element->getAreaElementType() == physicalModel::AreaElementType::SHEARWALL && (i != 0 || j != 0)) {
                elementTag = opensees::utilities::TagGenerator::getInstance().getNextShearWallTag();
            }
            if (quadrilateralElementExists(elementTag)) {
                // To do: exception
            }

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
            opensees::OpenseesModel::getInstance().addOutputElement(elementTag, opensees::ElementType::QUADRILATERAL);

            
        }
    }
}

std::tuple<utility::Vector3, utility::Vector3, utility::Vector3> OpenseesConverter::getLocalCoordinateSystem(std::vector<int> lineIJ, std::vector<int> lineIL)
{
    auto pointI = opensees::OpenseesModel::getInstance().m_nodes[lineIJ.front()]->getCoords();
    auto pointJ = opensees::OpenseesModel::getInstance().m_nodes[lineIJ.back()]->getCoords();
    auto pointL = opensees::OpenseesModel::getInstance().m_nodes[lineIL.back()]->getCoords();
    
    auto u = pointJ - pointI;
    auto uv = pointL - pointI;

    auto w = utility::VectorUtilities::crossProduct(u, uv);
    u = u / u.norm2();
    w = w / w.norm2();
    auto v = utility::VectorUtilities::crossProduct(w, u);

    return std::make_tuple(u, v, w);
}