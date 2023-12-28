#include "ElasticSection2D.h"

using namespace physicalModel;

ElasticSection2D::ElasticSection2D(int sectionTag, std::shared_ptr<Material> material, double thickness) : Section(sectionTag, material)
{
	m_sectionType = SectionType::ELASTIC2D;
	m_thickness = thickness;
}

double ElasticSection2D::getThickness() const
{
	return m_thickness;
}