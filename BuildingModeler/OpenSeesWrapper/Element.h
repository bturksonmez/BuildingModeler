#pragma once

#include "Object.h"

namespace opensees
{
	enum class ElementType
	{
		ZeroLength,
		BeamColumn,
		Quad
	};

	class Element : Object
	{
	protected:
		int m_elementTag;
		ElementType m_elementType;

		Element(int elementTag) : m_elementTag(elementTag) {}

	public:
		~Element() {}

		int getElementTag() const;
		ElementType getElementType() const;
	};
}

