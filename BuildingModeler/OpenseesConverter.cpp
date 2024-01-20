#include "OpenseesConverter.h"

using namespace buildingModeler;

void OpenseesConverter::toNodeMassConstraint(const physicalModel::Joint* joint)
{
    auto nodeTag = joint->getJointTag();
    if (nodeExists(nodeTag)) {
        // To do: exception
    }
    else {
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
}

void OpenseesConverter::toMaterial(const physicalModel::Material* material)
{
    auto materialTag = material->getMaterialTag();
    if (materialExists(materialTag)) {
        // To do: exception
    }
    else {
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
}

void OpenseesConverter::toSection(const physicalModel::Section* section)
{
    auto sectionTag = section->getSectionTag();
    if (sectionExists(sectionTag)) {
        // To do: exception
    }
    else {
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
}

bool OpenseesConverter::nodeExists(int nodeTag)
{
    if (opensees::OpenseesModel::getInstance().m_nodes.find(nodeTag) != opensees::OpenseesModel::getInstance().m_nodes.end()) {
        return true;
    }

    return false;
}

bool OpenseesConverter::materialExists(int nodeTag)
{
    if (opensees::OpenseesModel::getInstance().m_materials.find(nodeTag) != opensees::OpenseesModel::getInstance().m_materials.end()) {
        return true;
    }

    return false;
}

bool OpenseesConverter::sectionExists(int nodeTag)
{
    if (opensees::OpenseesModel::getInstance().m_sections.find(nodeTag) != opensees::OpenseesModel::getInstance().m_sections.end()) {
        return true;
    }

    return false;
}