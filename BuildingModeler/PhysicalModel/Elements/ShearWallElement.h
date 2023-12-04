#pragma once

#include "AreaElement.h"

namespace physicalModel
{
	class ShearWallElement : public AreaElement
	{
	public:
		ShearWallElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, AreaElementFormulation areaElementFormulation);
		ShearWallElement() = delete;
		~ShearWallElement() {}
	};
}
