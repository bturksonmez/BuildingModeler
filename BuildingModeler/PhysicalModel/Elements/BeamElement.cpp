#include "BeamElement.h"

using namespace physicalModel;

BeamElement::BeamElement(int elementTag, std::vector<int> jointTags, std::vector<std::shared_ptr<Section>> sections,
	std::vector<std::shared_ptr<SectionModifiers>> sectionModifiers, LineElementFormulation lineElementFormulation)
	: LineElement(elementTag, jointTags, sections, sectionModifiers, lineElementFormulation)
{
	m_lineElementType = LineElementType::BEAM;
}