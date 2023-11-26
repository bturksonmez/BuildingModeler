#pragma once

#include "../OpenseesObject.h"
#include "../Sections/Section.h"

namespace opensees
{
	enum class ElementType
	{
		ZEROLENGTH,
		BEAMCOLUMN,
		QUADRILATERAL
	};

	class Element : public OpenseesObject
	{
	protected:
		int m_elementTag;
		ElementType m_elementType;
		std::vector<int> m_nodeTags;
		std::shared_ptr<Section> m_section;

		Element(int elementTag, std::vector<int> nodes, std::shared_ptr<Section> section);

	public:
		Element() = delete;
		~Element() {}

		int getElementTag() const;
		ElementType getElementType() const;
		const std::vector<int>& getNodeTags() const;
		const std::shared_ptr<Section> getSection() const;
	};
}

