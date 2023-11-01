#include "ElasticSection.h"

using namespace std;
using namespace opensees;

ElasticSection::ElasticSection(int sectionTag, shared_ptr<Material> material, double A, double Iyy, double Izz, double J) : Section(sectionTag, material, A, Iyy, Izz, J)
{
	m_sectionType = SectionType::ELASTIC;
}

string ElasticSection::getOpenseesCommand() const
{
	string commandLine;

	commandLine = "section Elastic " + to_string(m_sectionTag) + " " + to_string(m_material->getE()) + " " + to_string(m_A) 
		+ " " + to_string(m_Izz) + " " + to_string(m_Iyy) + " " + to_string(m_material->getG()) + " " + to_string(m_J);

	commandLine = commandLine + "\n";

	return commandLine;
}