#include "ShearWallElement.h"

using namespace physicalModel;

ShearWallElement::ShearWallElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, AreaElementFormulation areaElementFormulation)
	: AreaElement(elementTag, jointTags, section, areaElementFormulation)
{
	m_areaElementType = AreaElementType::SHEARWALL;
}

double ShearWallElement::calculateAxialForceGlobalZ(std::string analysisTag, size_t timeStep, size_t edgeNo)
{
	return 1;
}

double ShearWallElement::calculateShearForceGlobalX(std::string analysisTag, size_t timeStep, size_t edgeNo)
{
	return 1;
}

double ShearWallElement::calculateShearForceGlobalY(std::string analysisTag, size_t timeStep, size_t edgeNo)
{
	return 1;
}

double ShearWallElement::calculateMomentGlobalXX(std::string analysisTag, size_t timeStep, size_t edgeNo)
{
	return 1;
}

double ShearWallElement::calculateMomentGlobalYY(std::string analysisTag, size_t timeStep, size_t edgeNo)
{
	return 1;
}

double ShearWallElement::calculateDR(std::string analysisTag, size_t timeStep, size_t edgeNo)
{
	return 1;
}

double ShearWallElement::calculateChordRotationGlobalX(std::string analysisTag, size_t timeStep, size_t edgeNo)
{
	return 1;
}

double ShearWallElement::calculateChordRotationGlobalZ(std::string analysisTag, size_t timeStep, size_t edgeNo)
{
	return 1;
}