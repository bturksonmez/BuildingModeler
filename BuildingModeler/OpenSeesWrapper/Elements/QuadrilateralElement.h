#pragma once

#include "Element.h"

namespace opensees
{
	enum class QuadrilateralElementType
	{
		MEMBRANE,
		SHELL
	};

	class QuadrilateralElement : public Element
	{
	protected:
		QuadrilateralElementType m_quadrilateralElementType;

		QuadrilateralElement(int elementTag, std::vector<std::shared_ptr<Node>> nodes, std::shared_ptr<Section> section);
	public:
		QuadrilateralElement() = delete;
		~QuadrilateralElement() {}

		const std::shared_ptr<Node> getINode() const;
		const std::shared_ptr<Node> getJNode() const;
        const std::shared_ptr<Node> getKNode() const;
		const std::shared_ptr<Node> getLNode() const;
		QuadrilateralElementType getQuadrilateralElementType() const;
	};
}
