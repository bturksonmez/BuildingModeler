#include "BeamColumnElement.h"

using namespace std;
using namespace opensees;

BeamColumnElement::BeamColumnElement(int elementTag, int iNode, int jNode, int transfTag) : Element(elementTag)
{
	m_iNode = iNode;
	m_jNode = jNode;
	m_transfTag = transfTag;

	m_elementType = ElementType::BeamColumn;
}