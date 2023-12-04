#include "LineElement.h"

using namespace std;
using namespace physicalModel;

LineElement::LineElement(int elementTag, std::vector<int> jointTags, std::vector<std::shared_ptr<Section>> sections,
	std::vector<std::shared_ptr<SectionModifiers>> sectionModifiers, LineElementFormulation lineElementFormulation)
	: m_elementTag(elementTag), m_jointTags(jointTags), m_sections(sections), m_sectionModifiers(sectionModifiers),
	m_lineElementFormulation(lineElementFormulation)
{
}

int LineElement::getElementTag() const
{
	return m_elementTag;
}

int LineElement::getIJointTag() const
{
	return m_jointTags[0];
}

int LineElement::getJJointTag() const
{
	return m_jointTags[1];
}

LineElementType LineElement::getLineElementType() const
{
	return m_lineElementType;
}

LineElementFormulation LineElement::getLineElementFormulation() const
{
	return m_lineElementFormulation;
}
