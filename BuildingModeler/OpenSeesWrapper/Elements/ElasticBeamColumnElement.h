#pragma once

#include "BeamColumnElement.h"

namespace opensees
{
	class ElasticBeamColumnElement : public BeamColumnElement
	{
	public:
		ElasticBeamColumnElement(int elementTag, std::vector<int> nodeTags, std::shared_ptr<Section> section, std::vector<double> modifiers, std::shared_ptr<GeometricTransformation> transf);
		ElasticBeamColumnElement() = delete;
		~ElasticBeamColumnElement() {}

		std::string getOpenseesCommand() const override;
	};
}
