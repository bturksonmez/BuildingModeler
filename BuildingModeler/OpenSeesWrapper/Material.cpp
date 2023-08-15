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