#include "Element.h"

using namespace std;
using namespace opensees;

Element::Element(int elementTag, vector<int> nodeTags, shared_ptr<Section> section) : m_elementTag(elementTag), m_nodeTags(nodeTags), m_section(section)
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

const vector<int>& Element::getNodeTags() const
{
	return m_nodeTags;
}

const shared_ptr<Section> Element::getSection() const
{
	return m_section;
}
