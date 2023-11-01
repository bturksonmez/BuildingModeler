#pragma once

#include "../Object.h"
#include "../Node.h"
#include "../Sections/Section.h"

namespace opensees
{
	enum class ElementType
	{
		ZEROLENGTH,
		BEAMCOLUMN,
		QUADRILATERAL
	};

	class Element : Object
	{
	protected:
		int m_elementTag;
		ElementType m_elementType;
		std::vector<std::shared_ptr<Node>> m_nodes;
		std::shared_ptr<Section> m_section;

		Element(int elementTag, std::vector<std::shared_ptr<Node>> nodes, std::shared_ptr<Section> section);

	public:
		Element() = delete;
		~Element() {}

		int getElementTag() const;
		ElementType getElementType() const;
		const std::vector<std::shared_ptr<Node>>& getNodes() const;
		const std::shared_ptr<Section> getSection() const;
	};
}

