#pragma once

#include <iostream>
#include <vector>

#include "Object.h"

namespace opensees
{
	enum ElementType
	{
		Frame,
		Area
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

