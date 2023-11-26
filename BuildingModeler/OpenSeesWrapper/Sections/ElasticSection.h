#pragma once

#include "Section.h"

namespace opensees
{
	class ElasticSection : public Section
	{
	public:
		ElasticSection(int sectionTag, std::shared_ptr<Material> material, double A, double Iyy, double Izz, double J);
		ElasticSection() = delete;
		~ElasticSection() {}

		std::string getOpenseesCommand() const override;
	};
}
