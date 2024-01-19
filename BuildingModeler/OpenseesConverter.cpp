#include "OpenseesConverter.h"

using namespace buildingModeler;

void OpenseesConverter::toNodeAndMass(const physicalModel::Joint* joint)
{
    auto nodeTag = joint->getJointTag();
    if (nodeExists(nodeTag)) {
        // To do: exception
    }
    else {
        opensees::OpenseesModel::getInstance().m_nodes[nodeTag] = std::make_unique<opensees::Node>(nodeTag, joint->getCoords());

        auto translationalMass = joint->getTranslationalMass();
        auto rotationalMass = joint->getRotationalMass();
        opensees::OpenseesModel::getInstance().m_masses[nodeTag] = std::make_unique<opensees::Mass>(nodeTag, translationalMass, rotationalMass);
    }
}

bool OpenseesConverter::nodeExists(int nodeTag)
{
    if (opensees::OpenseesModel::getInstance().m_nodes.find(nodeTag) != opensees::OpenseesModel::getInstance().m_nodes.end()) {
        return true;
    }

    return false;
}