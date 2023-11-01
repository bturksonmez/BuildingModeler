#include "Element.h"

using namespace std;
using namespace opensees;

Element::Element(int elementTag, vector<shared_ptr<Node>> nodes, shared_ptr<Section> section) : m_elementTag(elementTag), m_nodes(nodes), m_section(section)
{
	for (auto node : nodes) {
		node->addConnectedElements(shared_ptr<Element>(this));
	}
}

int Element::getElementTag() const
{
	return m_elementTag;
}

ElementType Element::getElementType() const
{
	return m_elementType;
}

const vector<shared_ptr<Node>>& Element::getNodes() const
{
	return m_nodes;
}

const shared_ptr<Section> Element::getSection() const
{
	return m_section;
}