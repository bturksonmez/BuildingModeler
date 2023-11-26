#pragma once

#include "Section.h"

namespace physicalModel
{
	class ElasticSection2D : public Section
	{
	private:
		double m_thickness;

	public:
		ElasticSection2D(int sectionTag, std::shared_ptr<Material> material, double thickness);
		~ElasticSection2D() {}

		double getThickness() const;
	};
}