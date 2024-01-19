#include "OpenseesModel.h"

using namespace opensees;

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