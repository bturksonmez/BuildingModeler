#include "ElasticSection.h"

using namespace opensees;

ElasticSection::ElasticSection(int sectionTag, std::shared_ptr<Material> material, double A, double Iyy, double Izz, double J) : Section(sectionTag, material, A, Iyy, Izz, J)
{
	m_sectionType = SectionType::ELASTIC;
}

std::string ElasticSection::getOpenseesCommand() const
{
	std::string commandLine;

	commandLine = "section Elastic " + std::to_string(m_sectionTag) + " " + std::to_string(m_material->getE()) + " " + std::to_string(m_A) 
		+ " " + std::to_string(m_Izz) + " " + std::to_string(m_Iyy) + " " + std::to_string(m_material->getG()) + " " + std::to_string(m_J);

	commandLine = commandLine + "\n";

	return commandLine;
}