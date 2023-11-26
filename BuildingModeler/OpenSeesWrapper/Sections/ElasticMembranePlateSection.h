#pragma once

#include "Section.h"

namespace opensees
{
	class ElasticMembranePlateSection : public Section
	{
	private:
		double m_thickness;

	public:
		ElasticMembranePlateSection(int sectionTag, std::shared_ptr<Material> material, double thickness);
		~ElasticMembranePlateSection() {}

		double getThickness() const;

		std::string getOpenseesCommand() const override;
	};
}