#include "SlabElement.h"

using namespace physicalModel;

SlabElement::SlabElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, AreaElementFormulation areaElementFormulation)
	: AreaElement(elementTag, jointTags, section, areaElementFormulation)
{
	m_areaElementType = AreaElementType::SLAB;
}