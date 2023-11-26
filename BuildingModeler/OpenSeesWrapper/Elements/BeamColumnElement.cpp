#include "BeamColumnElement.h"

using namespace std;
using namespace opensees;

BeamColumnElement::BeamColumnElement(int elementTag, vector<int> nodeTags, shared_ptr<Section> section, shared_ptr<GeometricTransformation> transf) : Element(elementTag, nodeTags, section)
{
	m_transf = transf;

	m_elementType = ElementType::BEAMCOLUMN;
}

int BeamColumnElement::getINodeTag() const
{
	return m_nodeTags[0];
}

int BeamColumnElement::getJNodeTag() const
{
	return m_nodeTags[1];
}

const shared_ptr<GeometricTransformation> BeamColumnElement::getGeometricTransf() const
{
	return m_transf;
}

BeamColumnElementType BeamColumnElement::getBeamColumnElementType() const
{
	return m_beamColumnElementType;
}