#include "BeamColumnElement.h"

using namespace opensees;

BeamColumnElement::BeamColumnElement(int elementTag, std::vector<int> nodeTags, std::shared_ptr<Section> section, std::shared_ptr<GeometricTransformation> transf) : Element(elementTag, nodeTags, section)
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

const std::shared_ptr<GeometricTransformation> BeamColumnElement::getGeometricTransf() const
{
	return m_transf;
}

BeamColumnElementType BeamColumnElement::getBeamColumnElementType() const
{
	return m_beamColumnElementType;
}