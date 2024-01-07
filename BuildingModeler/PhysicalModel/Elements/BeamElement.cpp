#include "BeamElement.h"

using namespace physicalModel;

BeamElement::BeamElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, LineElementFormulation lineElementFormulation)
	: LineElement(elementTag, jointTags, section, lineElementFormulation)
{
	m_lineElementType = LineElementType::BEAM;
}