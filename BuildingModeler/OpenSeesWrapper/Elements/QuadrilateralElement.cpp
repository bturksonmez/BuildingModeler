#include "QuadrilateralElement.h"

using namespace std;
using namespace opensees;

QuadrilateralElement::QuadrilateralElement(int elementTag, vector<int> nodeTags, shared_ptr<Section> section) : Element(elementTag, nodeTags, section)
{
	m_elementType = ElementType::QUADRILATERAL;
}

int QuadrilateralElement::getINodeTag() const
{
	return m_nodeTags[0];
}

int QuadrilateralElement::getJNodeTag() const
{
	return m_nodeTags[1];
}

int QuadrilateralElement::getKNodeTag() const
{
	return m_nodeTags[2];
}

int QuadrilateralElement::getLNodeTag() const
{
	return m_nodeTags[3];
}

QuadrilateralElementType QuadrilateralElement::getQuadrilateralElementType() const
{
	return m_quadrilateralElementType;
}