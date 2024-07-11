#include "OpenseesModel.h"

using namespace opensees;

OpenseesModel::OpenseesModel()
{
    m_geometricTransformation[0] = std::make_shared<LinearGeometricTransformation>(0, std::vector<int>{0, 1, 0}); // for columns
    m_geometricTransformation[1] = std::make_shared<LinearGeometricTransformation>(1, std::vector<int>{0, 0, 1}); // for beams
}

OpenseesModel& OpenseesModel::getInstance()
{
    static OpenseesModel instance;
    return instance;
}

void OpenseesModel::clear()
{
    m_modelName = "model";
    m_nodes.clear();
    m_masses.clear();
    m_contraintsSP.clear();
    m_contraintsDiaphragm.clear();
    m_beamColumnElements.clear();
    m_quadrilateralElements.clear();
    m_materials.clear();
    m_sections.clear();
    m_loadPatterns.clear();
    m_analyses.clear();
    m_divisionsBetweenNodes.clear();
    m_outputElements.clear();
}

std::string OpenseesModel::getModelName() const
{
    return m_modelName;
}

Node* OpenseesModel::getNode(int nodeTag) const
{
    auto it = m_nodes.find(nodeTag);
    return (it != m_nodes.end()) ? it->second.get() : nullptr;
}

Mass* OpenseesModel::getMass(int nodeTag) const
{
    auto it = m_masses.find(nodeTag);
    return (it != m_masses.end()) ? it->second.get() : nullptr;
}

Constraint* OpenseesModel::getSPConstraint(int nodeTag) const
{
    auto it = m_contraintsSP.find(nodeTag);
    return (it != m_contraintsSP.end()) ? it->second.get() : nullptr;
}

Constraint* OpenseesModel::getDiaphragmConstraint(int nodeTag) const
{
    auto it = m_contraintsDiaphragm.find(nodeTag);
    return (it != m_contraintsDiaphragm.end()) ? it->second.get() : nullptr;
}

Element* OpenseesModel::getBeamColumnElement(int elementTag) const
{
    auto it = m_beamColumnElements.find(elementTag);
    return (it != m_beamColumnElements.end()) ? it->second.get() : nullptr;
}

Element* OpenseesModel::getQuadrilateralElement(int elementTag) const
{
    auto it = m_quadrilateralElements.find(elementTag);
    return (it != m_quadrilateralElements.end()) ? it->second.get() : nullptr;
}

std::shared_ptr<Material> OpenseesModel::getMaterial(int materialTag) const
{
    auto it = m_materials.find(materialTag);
    return (it != m_materials.end()) ? it->second : nullptr;
}

std::shared_ptr<Section> OpenseesModel::getSection(int sectionTag) const
{
    auto it = m_sections.find(sectionTag);
    return (it != m_sections.end()) ? it->second : nullptr;
}

std::shared_ptr<LoadPattern> OpenseesModel::getLoadPattern(std::string loadingName) const
{
    auto it = m_loadPatterns.find(loadingName);
    return (it != m_loadPatterns.end()) ? it->second : nullptr;
}

std::vector<int> OpenseesModel::getDivisionsBetweenNodes(int nodeA, int nodeB) const
{
    if (m_divisionsBetweenNodes.count(std::make_pair(nodeA, nodeB))) {
        return m_divisionsBetweenNodes.find(std::make_pair(nodeA, nodeB))->second;
    }

    return {};
}

std::vector<int> OpenseesModel::getOutputNodes() const
{
    std::vector<int> outputNodes;

    for (auto it = m_nodes.begin(); it != m_nodes.end(); it++) {
        outputNodes.push_back(it->first);
    }

    return outputNodes;
}

const std::map<int, opensees::ElementType>& OpenseesModel::getOutputElements() const
{
    return m_outputElements;
}

void OpenseesModel::setModelName(std::string modelName)
{
    m_modelName = modelName;
}

void OpenseesModel::addDivisionsBetweenNodes(int nodeA, int nodeB, std::vector<int> dividedNodes)
{
    m_divisionsBetweenNodes.insert_or_assign(std::make_pair(nodeA, nodeB), dividedNodes);
}

void OpenseesModel::addOutputElement(int elementTag, opensees::ElementType elementType)
{
    m_outputElements.insert(std::make_pair(elementTag, elementType));
}

void OpenseesModel::toTclFile()
{
    std::stringstream modelTcl;

    modelTcl << "model BasicBuilder -ndm 3 -ndf 6\n";

    for (auto it = m_nodes.begin(); it != m_nodes.end(); it++) {
        modelTcl << it->second->getOpenseesCommand();
    }

    for (auto it = m_masses.begin(); it != m_masses.end(); it++) {
        modelTcl << it->second->getOpenseesCommand();
    }

    for (auto it = m_contraintsSP.begin(); it != m_contraintsSP.end(); it++) {
        modelTcl <<it->second->getOpenseesCommand();
    }

    for (auto it = m_contraintsDiaphragm.begin(); it != m_contraintsDiaphragm.end(); it++) {
        modelTcl << it->second->getOpenseesCommand();
    }

    for (auto it = m_geometricTransformation.begin(); it != m_geometricTransformation.end(); it++) {
        modelTcl << it->second->getOpenseesCommand();
    }

    for (auto it = m_materials.begin(); it != m_materials.end(); it++) {
        modelTcl << it->second->getOpenseesCommand();
    }

    for (auto it = m_sections.begin(); it != m_sections.end(); it++) {
        modelTcl << it->second->getOpenseesCommand();
    }

    for (auto it = physicalModel::Building::getInstance().m_lineElements.begin(); it != physicalModel::Building::getInstance().m_lineElements.end(); it++) {
        
        for (auto elementTag : it->second->getAnalyticalElementTags()) {

            auto element = m_beamColumnElements[elementTag].get();
            modelTcl << element->getOpenseesCommand();
        }
    }

    for (auto it = physicalModel::Building::getInstance().m_areaElements.begin(); it != physicalModel::Building::getInstance().m_areaElements.end(); it++) {

        for (auto elementTag : it->second->getAnalyticalElementTags()) {

            auto element = m_quadrilateralElements[elementTag].get();
            modelTcl << element->getOpenseesCommand();
        }
    }

    std::ofstream outFile(m_modelName + ".tcl");
    if (outFile.is_open()) {
        outFile << modelTcl.str();
        outFile.close();
    }
    else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }

    createLoadingTclFiles();
}

void OpenseesModel::createLoadingTclFiles()
{
    for (const auto analysis : m_analyses) {
        auto analysisTcl = analysis->getOpenseesCommand();

        std::ofstream outFile(analysis->getAnalysisName() + ".tcl");
        if (outFile.is_open()) {
            outFile << analysisTcl;
            outFile.close();
        }
        else {
            std::cerr << "Unable to open file for writing." << std::endl;
        }

        analysis->perform();
    }
}

void OpenseesModel::analyze()
{
    for (const auto analysis : m_analyses) {
        analysis->perform();
    }
}