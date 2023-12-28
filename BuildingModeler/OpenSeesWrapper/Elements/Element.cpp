#include "Element.h"

using namespace opensees;

Element::Element(int elementTag, std::vector<int> nodeTags, std::shared_ptr<Section> section) : m_elementTag(elementTag), m_nodeTags(nodeTags), m_section(section)
{
}

int Element::getElementTag() const
{
	return m_elementTag;
}

ElementType Element::getElementType() const
{
	return m_elementType;
}

const std::vector<int>& Element::getNodeTags() const
{
	return m_nodeTags;
}

const std::shared_ptr<Section> Element::getSection() const
{
	return m_section;
}
