#include "LineElement.h"

using namespace physicalModel;

LineElement::LineElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, LineElementFormulation lineElementFormulation)
	: m_elementTag(elementTag), m_jointTags(jointTags), m_lineElementFormulation(lineElementFormulation)
{
	m_segmentLengths.push_back(1.0);
	m_sections.push_back(section);
	m_sectionModifiers.push_back(std::make_shared<SectionModifiers>());
}

void LineElement::setSegmentLengths(std::vector<double> segmentLengths)
{
	m_segmentLengths = segmentLengths;
	m_sections.resize(segmentLengths.size());
	m_sectionModifiers.resize(segmentLengths.size());
}

void LineElement::setSection(int segmentNo, std::shared_ptr<Section> section)
{
	m_sections[segmentNo] = section;
}

void LineElement::setSectionModifiers(int segmentNo, std::shared_ptr<SectionModifiers> sectionModifier)
{
	m_sectionModifiers[segmentNo] = sectionModifier;
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

const std::vector<double>& LineElement::getSegmentLengths() const
{
	return m_segmentLengths;
}

const std::shared_ptr<Section> LineElement::getSection(int segmentNo) const
{
	return m_sections[segmentNo];
}

const std::shared_ptr<SectionModifiers> LineElement::getSectionModifiers(int segmentNo) const
{
	return m_sectionModifiers[segmentNo];
}

LineElementType LineElement::getLineElementType() const
{
	return m_lineElementType;
}

LineElementFormulation LineElement::getLineElementFormulation() const
{
	return m_lineElementFormulation;
}
