#pragma once

#include "LineElement.h"

namespace physicalModel
{
	class ColumnElement : public LineElement
	{
	public:
		ColumnElement(int elementTag, std::vector<int> jointTags, std::shared_ptr<Section> section, LineElementFormulation lineElementFormulation);
		ColumnElement() = delete;
		~ColumnElement() {}

		double calculateChordRotation(std::string analysisTag, size_t dofRot, bool fromIJoint, size_t timeStep) override;
	};
}
