#include "ElasticMembranePlateSection.h"

using namespace opensees;

ElasticMembranePlateSection::ElasticMembranePlateSection(int sectionTag, std::shared_ptr<Material> material, double thickness) : Section(sectionTag, material)
{
	m_sectionType = SectionType::ELASTICMEMBRANEPLATE;
	m_thickness = thickness;
}

double ElasticMembranePlateSection::getThickness() const
{
	return m_thickness;
}

std::string ElasticMembranePlateSection::getOpenseesCommand() const
{
	std::string commandLine;

	double nu = m_material->getE() / m_material->getG() / 2.0 - 1;

	commandLine = "section ElasticMembranePlateSection " + std::to_string(m_sectionTag) + " " + std::to_string(m_material->getE()) + " " + std::to_string(nu)
		+ " " + std::to_string(m_thickness) + " " + std::to_string(m_material->getRho());

	commandLine = commandLine + "\n";

	return commandLine;
}