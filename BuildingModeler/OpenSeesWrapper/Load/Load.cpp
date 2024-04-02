#include "Load.h"

using namespace opensees;

LoadType Load::getLoadType() const
{
	return m_loadType;
}

const std::vector<double>& Load::getLoadVector() const
{
	return m_loadVector;
}

Load& Load::operator*=(const double c)
{
	for (auto& loadVal : this->m_loadVector) {
		loadVal *= c;
	}

	return *this;
}