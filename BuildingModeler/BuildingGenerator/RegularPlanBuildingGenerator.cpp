#include "RegularPlanBuildingGenerator.h"

using namespace buildingGenerator;

RegularPlanBuildingGenerator::RegularPlanBuildingGenerator(const Parameters& parameters, bool symmetricInXDir, bool symmetricInYDir) :
	m_parameters(parameters), m_symXDir(symmetricInXDir), m_symYDir(symmetricInYDir)
{
	m_symBothDir = m_symXDir && m_symYDir;
}

void RegularPlanBuildingGenerator::build()
{

}