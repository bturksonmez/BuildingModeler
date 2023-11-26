#pragma once

#include "Material.h"

namespace opensees
{
	class ElasticMaterial : public Material
	{
	public:
		ElasticMaterial(int materialTag, double E, double G, double rho);
		ElasticMaterial() = delete;
		~ElasticMaterial() {}

		std::string getOpenseesCommand() const override;
	};
}
