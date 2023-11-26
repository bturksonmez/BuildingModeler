#include "ElasticBeamColumnElement.h"

using namespace std;
using namespace opensees;

ElasticBeamColumnElement::ElasticBeamColumnElement(int elementTag, vector<int> nodeTags, shared_ptr<Section> section, shared_ptr<GeometricTransformation> transf) 
	: BeamColumnElement(elementTag, nodeTags, section, transf)
{
	m_beamColumnElementType = BeamColumnElementType::ELASTIC;
}

string ElasticBeamColumnElement::getOpenseesCommand() const
{
	string command;

	shared_ptr<Material> mat = m_section->getMaterial();

	command = "element elasticBeamColumn " + to_string(m_elementTag) + " " + to_string(m_nodeTags[0]) + " " + to_string(m_nodeTags[1])
		+ " " + to_string(m_section->getA()) + " " + to_string(mat->getE()) + " " + to_string(mat->getE()) + " " + to_string(m_section->getJ())
		+ " " + to_string(m_section->getIyy()) + " " + to_string(m_section->getIzz()) + " " + to_string(m_transf->getTransfTag());

	command += ("\n");

	return command;
}

