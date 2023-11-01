#include "ElasticBeamColumnElement.h"

using namespace std;
using namespace opensees;

ElasticBeamColumnElement::ElasticBeamColumnElement(int elementTag, vector<shared_ptr<Node>> nodes, shared_ptr<Section> section, shared_ptr<GeometricTransformation> transf) 
	: BeamColumnElement(elementTag, nodes, section, transf)
{
	m_beamColumnElementType = BeamColumnElementType::ELASTIC;
}

string ElasticBeamColumnElement::getOpenseesCommand() const
{
	string command;

	shared_ptr<Material> mat = m_section->getMaterial();

	command = "element elasticBeamColumn " + to_string(m_elementTag) + " " + to_string(m_nodes[0]->getNodeTag()) + " " + to_string(m_nodes[1]->getNodeTag())
		+ " " + to_string(m_section->getA()) + " " + to_string(mat->getE()) + " " + to_string(mat->getE()) + " " + to_string(m_section->getJ())
		+ " " + to_string(m_section->getIyy()) + " " + to_string(m_section->getIzz()) + " " + to_string(m_transf->getTransfTag());

	command += ("\n");

	return command;
}

