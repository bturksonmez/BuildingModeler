#include "ElasticSection2D.h"

using namespace std;
using namespace physicalModel;

ElasticSection2D::ElasticSection2D(int sectionTag, shared_ptr<Material> material, double thickness) : Section(sectionTag, material)
{
	m_sectionType = SectionType::ELASTIC2D;
	m_thickness = thickness;
}

double ElasticSection2D::getThickness() const
{
	return m_thickness;
}