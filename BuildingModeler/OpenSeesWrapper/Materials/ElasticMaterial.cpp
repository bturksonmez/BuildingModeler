#include "ElasticMaterial.h"

using namespace std;
using namespace opensees;

ElasticMaterial::ElasticMaterial(int materialTag, double rho, double E, double G) : Material(materialTag, E, G, rho)
{
	m_materialType = MaterialType::ELASTIC;
}

string ElasticMaterial::getOpenseesCommand() const
{
	string commandLine;

	commandLine = "uniaxialMaterial Elastic " + to_string(m_materialTag) + " " + to_string(m_E);

	commandLine = commandLine + "\n";

	return commandLine;
}
