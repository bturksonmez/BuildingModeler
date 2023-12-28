#include "ElasticMaterial.h"

using namespace physicalModel;

ElasticMaterial::ElasticMaterial(int materialTag, double E, double G, double rho) : Material(materialTag, E, G, rho)
{
	m_materialType = MaterialType::ELASTIC;
}
