#include "Load.h"

using namespace std;
using namespace opensees;

LoadType Load::getLoadType() const
{
	return m_loadType;
}

const vector<double>& Load::getLoadVector() const
{
	return m_loadVector;
}