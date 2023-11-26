#pragma once

#include "../OpenseesObject.h"
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

	class Element : public OpenseesObject
	{
	protected:
		int m_elementTag;
		ElementType m_elementType;
		std::vector<std::weak_ptr<Node>> m_nodes;
		std::shared_ptr<Section> m_section;
		std::vector<std::shared_ptr<Load>> m_elementLoads;

		Element(int elementTag, std::vector<std::shared_ptr<Node>> nodes, std::shared_ptr<Section> section);

	public:
		Element() = delete;
		~Element() {}

		int getElementTag() const;
		ElementType getElementType() const;
		const std::vector<std::shared_ptr<Node>>& getNodes() const;
		const std::shared_ptr<Section> getSection() const;
		const std::vector<std::shared_ptr<Load>>& getElementLoads() const;
		void addElementLoad(std::shared_ptr<Load> elementLoad);
	};
}

