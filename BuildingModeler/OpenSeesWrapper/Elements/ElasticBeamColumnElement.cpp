#include "ElasticBeamColumnElement.h"

using namespace opensees;

ElasticBeamColumnElement::ElasticBeamColumnElement(int elementTag, std::vector<int> nodeTags, std::shared_ptr<Section> section, std::vector<double> modifiers, std::shared_ptr<GeometricTransformation> transf)
	: BeamColumnElement(elementTag, nodeTags, section, modifiers, transf)
{
	m_beamColumnElementType = BeamColumnElementType::ELASTIC;
}

std::string ElasticBeamColumnElement::getOpenseesCommand() const
{
	std::string command;

	std::shared_ptr<Material> mat = m_section->getMaterial();

	command = "element elasticBeamColumn " + std::to_string(m_elementTag) + " " + std::to_string(m_nodeTags[0]) + " " + std::to_string(m_nodeTags[1])
		+ " " + std::to_string(m_section->getA() * m_modifiers[0]) + " " + std::to_string(mat->getE()) + " " + std::to_string(mat->getE()) + " " + std::to_string(m_section->getJ() * m_modifiers[3])
		+ " " + std::to_string(m_section->getIyy() * m_modifiers[1]) + " " + std::to_string(m_section->getIzz() * m_modifiers[2]) + " " + std::to_string(m_transf->getTransfTag());

	command += ("\n");

	return command;
}

