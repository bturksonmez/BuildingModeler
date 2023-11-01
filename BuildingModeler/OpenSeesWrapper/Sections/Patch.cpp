#include "Patch.h"

using namespace std;
using namespace opensees;

int Patch::getMaterialTag() const
{
	return m_materialTag;
}

PatchType Patch::getPatchType() const
{
	return m_patchType;
}