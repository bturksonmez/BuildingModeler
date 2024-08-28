#include "ShearWallElement.h"
#include "../Building.h"

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

const std::vector<int> ShearWallElement::getBottomAnalyticalElements() const
{
	std::vector<int> bottomElements;

	int n1 = 1;
	if (m_meshable) {

		if (m_surroundingLineElementTags[0] != -1) {
			n1 = physicalModel::Building::getInstance().getLineElement(m_surroundingLineElementTags[0])->getSegmentLengths().size();
		}
		else {
			n1 = m_n1;
		}
	}

	for (int i = 0; i < n1; ++i) {
		bottomElements.push_back(m_analyticalElementTags[i]);
	}
	
	return bottomElements;
}

const std::vector<int> ShearWallElement::getTopAnalyticalElements() const
{
	std::vector<int> topElements;

	int n1 = 1;
	if (m_meshable) {

		if (m_surroundingLineElementTags[0] != -1) {
			n1 = physicalModel::Building::getInstance().getLineElement(m_surroundingLineElementTags[0])->getSegmentLengths().size();
		}
		else {
			n1 = m_n1;
		}
	}

	for (int i = m_analyticalElementTags.size() - n1; i < m_analyticalElementTags.size(); ++i) {
		topElements.push_back(m_analyticalElementTags[i]);
	}

	return topElements;
}