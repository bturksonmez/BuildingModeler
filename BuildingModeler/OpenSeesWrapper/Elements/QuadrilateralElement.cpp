#include "QuadrilateralElement.h"

using namespace std;
using namespace opensees;

QuadrilateralElement::QuadrilateralElement(int elementTag, vector<shared_ptr<Node>> nodes, shared_ptr<Section> section) : Element(elementTag, nodes, section)
{
	m_elementType = ElementType::QUADRILATERAL;
}

const shared_ptr<Node> QuadrilateralElement::getINode() const
{
	return m_nodes[0];
}

const shared_ptr<Node> QuadrilateralElement::getJNode() const
{
	return m_nodes[1];
}

const shared_ptr<Node> QuadrilateralElement::getKNode() const
{
	return m_nodes[2];
}

const shared_ptr<Node> QuadrilateralElement::getLNode() const
{
	return m_nodes[3];
}

QuadrilateralElementType QuadrilateralElement::getQuadrilateralElementType() const
{
	return m_quadrilateralElementType;
}