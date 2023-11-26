#include "ElasticSection1D.h"

using namespace std;
using namespace physicalModel;

ElasticSection1D::ElasticSection1D(int sectionTag, shared_ptr<Material> material, double A, double Iyy, double Izz, double J) : Section(sectionTag, material, A, Iyy, Izz, J)
{
	m_sectionType = SectionType::ELASTIC1D;
}