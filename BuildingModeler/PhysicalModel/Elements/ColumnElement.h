#pragma once

#include "LineElement.h"

namespace physicalModel
{
	class ColumnElement : public LineElement
	{
	public:
		ColumnElement(int elementTag, std::vector<int> jointTags, std::vector<std::shared_ptr<Section>> sections,
			std::vector<std::shared_ptr<SectionModifiers>> sectionModifiers, LineElementFormulation lineElementFormulation);
		ColumnElement() = delete;
		~ColumnElement() {}
	};
}
