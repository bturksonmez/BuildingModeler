#include "ElasticMembranePlateSection.h"

using namespace std;
using namespace opensees;

ElasticMembranePlateSection::ElasticMembranePlateSection(int sectionTag, shared_ptr<Material> material, double thickness) : Section(sectionTag, material)
{
	m_sectionType = SectionType::ELASTICMEMBRANEPLATE;
	m_thickness = thickness;
}

double ElasticMembranePlateSection::getThickness() const
{
	return m_thickness;
}

string ElasticMembranePlateSection::getOpenseesCommand() const
{
	string commandLine;

	double nu = m_material->getE() / m_material->getG() / 2.0 - 1;

	commandLine = "section ElasticMembranePlateSection " + to_string(m_sectionTag) + " " + to_string(m_material->getE()) + " " + to_string(nu)
		+ " " + to_string(m_thickness) + " " + to_string(m_material->getRho());

	commandLine = commandLine + "\n";

	return commandLine;
}