#pragma once

#include "Object.h"
#include "Node.h"

namespace opensees
{
	enum class ElementType
	{
		ZEROLENGTH,
		BEAMCOLUMN,
		QUAD
	};

	class Element : Object
	{
	protected:
		int m_elementTag;
		ElementType m_elementType;
		std::vector<std::shared_ptr<Node>> m_nodes;

		Element(int elementTag, std::vector<std::shared_ptr<Node>> nodes);

	public:
		Element() = delete;
		~Element() {}

		int getElementTag() const;
		ElementType getElementType() const;
		const std::vector<std::shared_ptr<Node>>& getNodes() const;
	};
}

