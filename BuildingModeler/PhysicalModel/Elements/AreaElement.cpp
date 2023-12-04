#include "AreaElement.h"

using namespace std;
using namespace physicalModel;

AreaElement::AreaElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, AreaElementFormulation areaElementFormulation)
	: m_elementTag(elementTag), m_jointTags(jointTags), m_section(section)
{
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

AreaElementType AreaElement::getAreaElementType() const
{
	return m_areaElementType;
}

AreaElementFormulation AreaElement::getAreaElementFormulation() const
{
	return m_areaElementFormulation;
}
