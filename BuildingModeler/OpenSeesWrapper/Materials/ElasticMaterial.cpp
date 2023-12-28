#include "ElasticMaterial.h"

using namespace opensees;

ElasticMaterial::ElasticMaterial(int materialTag, double E, double G, double rho) : Material(materialTag, E, G, rho)
{
	m_materialType = MaterialType::ELASTIC;
}

std::string ElasticMaterial::getOpenseesCommand() const
{
	std::string commandLine;

	commandLine = "uniaxialMaterial Elastic " + std::to_string(m_materialTag) + " " + std::to_string(m_E);

	commandLine = commandLine + "\n";

	return commandLine;
}
