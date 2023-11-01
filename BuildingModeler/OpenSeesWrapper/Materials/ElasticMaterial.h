#pragma once

#include "Material.h"

namespace opensees
{
	class ElasticMaterial : Material
	{
	public:
		ElasticMaterial(int materialTag, double rho, double E, double G);
		ElasticMaterial() = delete;
		~ElasticMaterial() {}

		std::string getOpenseesCommand() const override;
	};
}
