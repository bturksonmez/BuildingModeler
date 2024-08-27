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

		double calculateChordRotation(std::string analysisTag, size_t dofRot, bool fromIJoint, size_t timeStep) override;
	};
}
