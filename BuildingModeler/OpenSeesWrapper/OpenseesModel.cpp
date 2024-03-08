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

void OpenseesModel::toTclFile()
{
    std::stringstream modelTcl;

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

    for (auto it = m_beamColumnElements.begin(); it != m_beamColumnElements.end(); it++) {
        modelTcl << it->second->getOpenseesCommand();
    }

    for (auto it = m_quadrilateralElements.begin(); it != m_quadrilateralElements.end(); it++) {
        modelTcl << it->second->getOpenseesCommand();
    }

    std::ofstream outFile("model.tcl");
    if (outFile.is_open()) {
        outFile << modelTcl.str();
        outFile.close();
    }
    else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
}