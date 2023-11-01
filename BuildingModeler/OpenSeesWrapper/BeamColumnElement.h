#pragma once

#include "Element.h"
#include "GeometricTransformation.h"

namespace opensees
{
	enum class BeamColumnElementType
	{
		ELASTIC,
		ELASTICTIMOSHENKO,
		BEAMWITHHINGES,
		DISPBASED,
		FORCEBASED
	};

	class BeamColumnElement : public Element
	{
	protected:
		std::shared_ptr<GeometricTransformation> m_transf;
		BeamColumnElementType m_beamColumnElementType;

		BeamColumnElement(int elementTag, std::vector<std::shared_ptr<Node>> nodes, std::shared_ptr<Section> section, std::shared_ptr<GeometricTransformation> transf);
	public:
		BeamColumnElement() = delete;
		~BeamColumnElement() {}

		const std::shared_ptr<Node> getINode() const;
		const std::shared_ptr<Node> getJNode() const;
		const std::shared_ptr<GeometricTransformation> getGeometricTransf() const;
		BeamColumnElementType getBeamColumnElementType() const;
	};
}
