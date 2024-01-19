#pragma once

#include "AreaElement.h"

namespace physicalModel
{
	class SlabElement : public AreaElement
	{
	public:
		SlabElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, AreaElementFormulation areaElementFormulation);
		SlabElement() = delete;
		~SlabElement() {}
	};
}
