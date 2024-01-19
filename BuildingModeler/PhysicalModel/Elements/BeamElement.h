#pragma once

#include "LineElement.h"

namespace physicalModel
{
	class BeamElement : public LineElement
	{
	public:
		BeamElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, LineElementFormulation lineElementFormulation);
		BeamElement() = delete;
		~BeamElement() {}
	};
}
