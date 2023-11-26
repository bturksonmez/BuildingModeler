#pragma once

#include "Material.h"

namespace physicalModel
{
	class ElasticMaterial : public Material
	{
	public:
		ElasticMaterial(int materialTag, double E, double G, double rho);
		ElasticMaterial() = delete;
		~ElasticMaterial() {}
	};
}
#pragma once
