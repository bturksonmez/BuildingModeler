#include "Element.h"

using namespace std;
using namespace opensees;

Element::Element(int elementTag, vector<shared_ptr<Node>> nodes, shared_ptr<Section> section) : m_elementTag(elementTag), m_section(section)
{
	for (auto node : nodes) {
		node->addConnectedElements(shared_ptr<Element>(this));
		m_nodes.push_back(node);
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
	vector<shared_ptr<Node>> sp(m_nodes.size());
	transform(m_nodes.begin(), m_nodes.end(), sp.begin(), [](const weak_ptr<Node>& wp) { return wp.lock(); });

	return sp;
}

const shared_ptr<Section> Element::getSection() const
{
	return m_section;
}

const vector<shared_ptr<Load>>& Element::getElementLoads() const
{
	return m_elementLoads;
}

void Element::addElementLoad(shared_ptr<Load> elementLoad)
{
	m_elementLoads.push_back(elementLoad);
}