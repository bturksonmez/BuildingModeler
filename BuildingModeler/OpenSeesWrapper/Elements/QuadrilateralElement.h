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

		QuadrilateralElement(int elementTag, std::vector<int> nodeTags, std::shared_ptr<Section> section);
	public:
		QuadrilateralElement() = delete;
		~QuadrilateralElement() {}

		int getINodeTag() const;
		int getJNodeTag() const;
        int getKNodeTag() const;
		int getLNodeTag() const;
		QuadrilateralElementType getQuadrilateralElementType() const;
	};
}
