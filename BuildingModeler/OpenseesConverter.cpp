#include "OpenseesConverter.h"

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
    
    auto nodes = getNodesForLineElement(element);
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

std::vector<int> OpenseesConverter::getNodesForLineElement(const physicalModel::LineElement* element)
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