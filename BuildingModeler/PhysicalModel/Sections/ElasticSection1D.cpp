#include "ElasticSection1D.h"

using namespace physicalModel;

ElasticSection1D::ElasticSection1D(int sectionTag, std::shared_ptr<Material> material, double A, double Iyy, double Izz, double J) : Section(sectionTag, material)
{
	m_sectionType = SectionType::ELASTIC1D;

	m_A = A;
	m_Iyy = Iyy;
	m_Izz = Izz;
	m_J = J;
}