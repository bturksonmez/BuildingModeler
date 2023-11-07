#include "BeamColumnElement.h"

using namespace std;
using namespace opensees;

BeamColumnElement::BeamColumnElement(int elementTag, vector<shared_ptr<Node>> nodes, shared_ptr<Section> section, shared_ptr<GeometricTransformation> transf) : Element(elementTag, nodes, section)
{
	m_transf = transf;

	m_elementType = ElementType::BEAMCOLUMN;
}

const shared_ptr<Node> BeamColumnElement::getINode() const
{
	return m_nodes[0].lock();
}

const shared_ptr<Node> BeamColumnElement::getJNode() const
{
	return m_nodes[1].lock();
}

const shared_ptr<GeometricTransformation> BeamColumnElement::getGeometricTransf() const
{
	return m_transf;
}

BeamColumnElementType BeamColumnElement::getBeamColumnElementType() const
{
	return m_beamColumnElementType;
}