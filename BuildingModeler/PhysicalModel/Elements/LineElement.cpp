#include "LineElement.h"
#include "../Building.h"

using namespace physicalModel;

LineElement::LineElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, LineElementFormulation lineElementFormulation)
	: m_elementTag(elementTag), m_jointTags(jointTags), m_lineElementFormulation(lineElementFormulation)
{
	m_length = calculateLength(jointTags[0], jointTags[1]);
	m_segmentLengths.push_back(m_length);
	m_segmentRelativeLengths.push_back(1.0);
	m_sections.push_back(section);
	m_sectionModifiers.push_back(std::make_shared<SectionModifiers>());
}

double LineElement::calculateLength(int jointI, int jointJ)
{
	utility::Vector3 pointI = physicalModel::Building::getInstance().getJoint(jointI)->getCoords();
	utility::Vector3 pointJ = physicalModel::Building::getInstance().getJoint(jointJ)->getCoords();
	utility::Vector3 dirVec = pointJ - pointI;

	return dirVec.norm2();
}

void LineElement::setSegmentRelativeLengths(std::vector<double> segmentRelativeLengths)
{
	m_segmentRelativeLengths = segmentRelativeLengths;

	m_segmentLengths.resize(segmentRelativeLengths.size());
	for (size_t i = 0; i < m_segmentLengths.size(); ++i) {
		m_segmentLengths[i] = m_length * m_segmentRelativeLengths[i];
	}

	m_sections.resize(segmentRelativeLengths.size());
	for (size_t i = 1; i < m_sections.size(); ++i) {
		m_sections[i] = m_sections[0];
	}

	m_sectionModifiers.resize(segmentRelativeLengths.size());
	for (size_t i = 1; i < m_sectionModifiers.size(); ++i) {
		m_sectionModifiers[i] = m_sectionModifiers[0];
	}
}

void LineElement::setSection(int segmentNo, std::shared_ptr<Section> section)
{
	m_sections[segmentNo] = section;
}

void LineElement::setSectionModifiers(int segmentNo, std::shared_ptr<SectionModifiers> sectionModifier)
{
	m_sectionModifiers[segmentNo] = sectionModifier;
}

void LineElement::addAnalyticalNodeTag(int analyticalNodeTag)
{
	m_analyticalNodeTags.push_back(analyticalNodeTag);
}

void LineElement::addAnalyticalElementTag(int analyticalElementTag)
{
	m_analyticalElementTags.push_back(analyticalElementTag);
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

double LineElement::getElementLength() const
{
	return m_length;
}

const std::vector<double>& LineElement::getSegmentLengths() const
{
	return m_segmentLengths;
}

const std::vector<double>& LineElement::getSegmentRelativeLengths() const
{
	return m_segmentRelativeLengths;
}

const std::shared_ptr<Section> LineElement::getSection(int segmentNo) const
{
	return m_sections[segmentNo];
}

const std::shared_ptr<SectionModifiers> LineElement::getSectionModifiers(int segmentNo) const
{
	return m_sectionModifiers[segmentNo];
}

std::vector<int> LineElement::getAnalyticalNodeTags() const
{
	return m_analyticalNodeTags;
}

const std::vector<int>& LineElement::getAnalyticalElementTags() const
{
	return m_analyticalElementTags;
}

LineElementType LineElement::getLineElementType() const
{
	return m_lineElementType;
}

LineElementFormulation LineElement::getLineElementFormulation() const
{
	return m_lineElementFormulation;
}
