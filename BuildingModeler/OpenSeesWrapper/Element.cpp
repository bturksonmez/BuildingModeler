#include "Element.h"

using namespace std;
using namespace opensees;

Element::Element(int elementTag, vector<shared_ptr<Node>> nodes) : m_elementTag(elementTag), m_nodes(nodes)
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