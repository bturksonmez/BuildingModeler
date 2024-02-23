#include "AreaElement.h"

using namespace physicalModel;

AreaElement::AreaElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, AreaElementFormulation areaElementFormulation)
	: m_elementTag(elementTag), m_jointTags(jointTags), m_section(section)
{
	m_meshable = false;
	m_surroundingLineElementTags.resize(4, -1);
}

void AreaElement::addSurroundingLineElement(int index, int surroundingLineElementTag)
{
	m_surroundingLineElementTags[index] = surroundingLineElementTag;
}

void AreaElement::mesh(bool meshable, int n1 = -1, int n2 = -1)
{
	m_meshable = meshable;
	m_n1 = n1;
	m_n2 = n2;
}

void AreaElement::setMeshable(bool meshable)
{
	m_meshable = meshable;
}

int AreaElement::getElementTag() const
{
	return m_elementTag;
}

int AreaElement::getIJointTag() const
{
	return m_jointTags[0];
}

int AreaElement::getJJointTag() const
{
	return m_jointTags[1];
}

int AreaElement::getKJointTag() const
{
	return m_jointTags[2];
}

int AreaElement::getLJointTag() const
{
	return m_jointTags[3];
}

const std::vector<int> AreaElement::getJointTags() const
{
	return m_jointTags;
}

std::pair<int, int> AreaElement::getMeshDivisions() const
{
	return { m_n1, m_n2 };
}

std::vector<int> AreaElement::getSurroundingLineElementTags()
{
	return m_surroundingLineElementTags;
}

const std::shared_ptr<Section> AreaElement::getSection() const
{
	return m_section;
}

AreaElementType AreaElement::getAreaElementType() const
{
	return m_areaElementType;
}

AreaElementFormulation AreaElement::getAreaElementFormulation() const
{
	return m_areaElementFormulation;
}
