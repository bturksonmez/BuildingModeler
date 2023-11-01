#include "Material.h"

using namespace std;
using namespace opensees;

int Material::getMaterialTag() const
{
	return m_materialTag;
}

MaterialType Material::getMaterialType() const
{
	return m_materialType;
}

double Material::getE() const
{
	return m_E;
}

double Material::getG() const
{
	return m_G;
}

double Material::getRho() const
{
	return m_rho;
}