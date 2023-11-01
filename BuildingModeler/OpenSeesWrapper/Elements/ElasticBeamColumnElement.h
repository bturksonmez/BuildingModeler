#pragma once

#include "BeamColumnElement.h"

namespace opensees
{
	class ElasticBeamColumnElement : public BeamColumnElement
	{
	public:
		ElasticBeamColumnElement(int elementTag, std::vector<std::shared_ptr<Node>> nodes, std::shared_ptr<Section> section, std::shared_ptr<GeometricTransformation> transf);
		ElasticBeamColumnElement() = delete;
		~ElasticBeamColumnElement() {}

		std::string getOpenseesCommand() const override;
	};
}
