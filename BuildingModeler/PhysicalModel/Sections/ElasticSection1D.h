#pragma once

#include "Section.h"

namespace physicalModel
{
	class ElasticSection1D : public Section
	{
	public:
		ElasticSection1D(int sectionTag, std::shared_ptr<Material> material, Shape* shape);
		ElasticSection1D() = delete;
		~ElasticSection1D() {}
	};
}
